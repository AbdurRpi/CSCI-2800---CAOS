/* Checkpoint 3: 
   Compile: gcc -Wall -Wextra -Werror -pthread checkpoint3.c -o checkpoint3
   Run:     ./checkpoint3 line input.txt another.txt
*/

#include <stdio.h>      /* perror, main-thread messages, and flushing those messages */
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>     /* int32_t guarantees a 32-bit returned count */
#include <pthread.h>

/* pthread_create accepts one argument, so group the two strings together.
   These strings belong to argv and remain valid until all threads are joined. */
struct thread_args
{
   const char *filename;
   const char *substring;
};

/* write can write fewer bytes than requested. Finish the remaining bytes. */
int write_all(int fd, const char *text, size_t length)
{
   while (length > 0)
   {
      int written = write(fd, text, length);
      /* EINTR means a signal interrupted the call; retry the remaining data. */
      if (written < 0 && errno == EINTR)
      {
         continue;
      }
      if (written <= 0)
      {
         return -1;
      }
      /* Move past the bytes already written and reduce the remaining count. */
      text += written;
      length -= written;
   }
   return 0;
}

/* Each worker calls this after opening its own file.
   Return a full 32-bit match count, or -1 for a file/search error. */
int search_file(int fd, const char *substring, const char *filename)
{
   /* NULL lets the first realloc behave like malloc. length counts file bytes. */
   char *buffer = NULL;
   int length = 0;
   int occurrences = 0;

   /* Read the file into one growing buffer. Each read asks for 4096 bytes. */
   while (1)
   {
      if (length > SIZE_MAX - 4096)
      {
         errno = EOVERFLOW;
         perror("file is too large");
         free(buffer);
         close(fd);
         return -1;
      }
      /* Keep the old pointer until allocation succeeds, so it can still be freed. */
      char *larger = realloc(buffer, length + 4096);
      if (larger == NULL)
      {
         perror("realloc() failed");
         free(buffer);
         close(fd);
         return -1;
      }
      buffer = larger;

      /* Append new bytes after those already read. No string terminator is needed. */
      int bytes_read = read(fd, buffer + length, 4096);
      if (bytes_read < 0 && errno == EINTR)
      {
         continue;
      }
      if (bytes_read == -1)
      {
         perror("read() failed");
         free(buffer);
         close(fd);
         return -1;
      }
      /* A return value of zero means end-of-file. */
      if (bytes_read == 0)
      {
         break;
      }
      length += bytes_read;
   }

   /* The entire file is in memory, so this thread no longer needs its descriptor. */
   if (close(fd) == -1)
   {
      perror("close() failed");
      free(buffer);
      return -1;
   }

   int substring_length = strlen(substring);
   if (substring_length > length)
   {
      free(buffer);
      return 0;
   }

   /* i is a possible match start. Advance by one to include overlapping matches. */
   for (int i = 0; i <= length - substring_length; i++)
   {
      /* j counts how many characters match at this starting position. */
      int j = 0;
      while (j < substring_length)
      {
         /* Compare lowercase values without changing the original file bytes.
            The unsigned char casts make calls to tolower valid for all bytes. */
         int a = tolower((unsigned char)*(buffer + i + j));
         int b = tolower((unsigned char)*(substring + j));
         if (a != b)
         {
            break;
         }
         j++;
      }

      /* If comparison stopped early, try the next starting position. */
      if (j != substring_length)
      {
         continue;
      }

      /* Walk backward up to 16 bytes, stopping at a newline or file start. */
      int start = i;
      int before = 0;
      while (start > 0 && before < 16)
      {
         if (*(buffer + start - 1) == '\n')
         {
            break;
         }
         start--;
         before++;
      }

      /* Walk forward up to 16 bytes after the match. */
      int end = i + substring_length;
      int after = 0;
      while (end < length && after < 16)
      {
         if (*(buffer + end) == '\n')
         {
            break;
         }
         end++;
         after++;
      }

      /* Build one record: filename, colon, space, snippet, and newline.
         end is exclusive, so end - start is the snippet length. */
      int filename_length = strlen(filename);
      int output_length = filename_length + 2 + (end - start) + 1;
      char *output = malloc(output_length);
      if (output == NULL)
      {
         perror("malloc() failed");
         free(buffer);
         return -1;
      }
      memcpy(output, filename, filename_length);
      *(output + filename_length) = ':';
      *(output + filename_length + 1) = ' ';
      memcpy(output + filename_length + 2, buffer + start, end - start);
      *(output + output_length - 1) = '\n';
      int result = write_all(STDOUT_FILENO, output, output_length);
      free(output);
      if (result == -1)
      {
         perror("write() failed");
         free(buffer);
         return -1;
      }
      if (occurrences == INT32_MAX)
      {
         const char *message = "ERROR: match count exceeds a 32-bit integer\n";
         write_all(STDERR_FILENO, message, strlen(message));
         free(buffer);
         return -1;
      }
      occurrences++;
   }

   free(buffer);
   /* Threads can return a pointer to a full integer: do not reduce modulo 256. */
   return occurrences;
}

/* Same void-pointer argument/return pattern as multiple-threads-v1.c. */
void *search_thread(void *arg)
{
   struct thread_args *data = arg;
   const char *filename = data->filename;
   const char *substring = data->substring;
   free(data);   /* Each worker owns its own argument allocation. */

   /* Return heap memory, never the address of a local stack variable.
      Main retrieves this pointer with pthread_join and frees it. */
   int *count = malloc(sizeof(int));
   if (count == NULL)
   {
      perror("malloc() failed");
      return NULL;
   }
   *count = -1;

   /* Unlike Checkpoint 2, the WORKER opens the file. All threads share the
      process descriptor table, so main must not close the worker's file. */
   int fd = open(filename, O_RDONLY);
   if (fd == -1)
   {
      perror(filename);
      return count;
   }

   /* search_file reads and closes fd, including its error paths. */
   *count = search_file(fd, substring, filename);
   return count;
}

int main(int argc, char **argv)
{
   if (argc < 3 || **(argv + 1) == '\0' || strchr(*(argv + 1), '\n') != NULL)
   {
      const char *message = "USAGE: ./checkpoint3 <substring> <filename> ...\n";
      write_all(STDERR_FILENO, message, strlen(message));
      return EXIT_FAILURE;
   }

   /* Store actual thread IDs, using pointer arithmetic as in the lecture. */
   int files = argc - 2;
   pthread_t *tids = calloc((size_t)files, sizeof(pthread_t));
   if (tids == NULL)
   {
      perror("calloc() failed");
      return EXIT_FAILURE;
   }

   int created = 0;
   int result = EXIT_SUCCESS;
   unsigned long total = 0;

   for (int i = 2; i < argc; i++)
   {
      /* A separate allocation prevents threads from sharing a changing
         loop variable or a reused argument structure. */
      struct thread_args *data = malloc(sizeof(struct thread_args));
      if (data == NULL)
      {
         perror("malloc() failed");
         result = EXIT_FAILURE;
         break;
      }
      data->filename = *(argv + i);
      data->substring = *(argv + 1);

      /* Flush before create so this message is visible before this worker
         can print a snippet, even when output is redirected to a file. */
      printf("MAIN: creating child thread for \"%s\"\n", data->filename);
      if (fflush(stdout) == EOF)
      {
         perror("fflush() failed");
         free(data);
         result = EXIT_FAILURE;
         break;
      }
      int rc = pthread_create(tids + created, NULL, search_thread, data);
      if (rc != 0)
      {
         /* pthread functions return an error number; they do not set errno. */
         printf("MAIN: ERROR: pthread_create() failed: %s\n", strerror(rc));
         free(data);   /* No worker exists to free it when creation fails. */
         result = EXIT_FAILURE;
         continue;
      }
      created++;
   }

   /* Create everyone first, then join, so files can be searched concurrently.
      Unlike waitpid, pthread_join needs a specific thread ID. */
   for (int i = 0; i < created; i++)
   {
      void *returned = NULL;
      int rc = pthread_join(*(tids + i), &returned);
      if (rc != 0)
      {
         printf("MAIN: ERROR: pthread_join() failed: %s\n", strerror(rc));
         result = EXIT_FAILURE;
         continue;
      }

      int *count = returned;
      if (count == NULL)
      {
         result = EXIT_FAILURE;
         continue;
      }
      if (*count < 0)
      {
         result = EXIT_FAILURE;
      }
      else
      {
         /* Only main changes total, after join has made the result available. */
         total += (unsigned long)*count;
      }
      free(count);
   }

   printf("MAIN: all threads joined; %llu total occurrences of \"%s\"\n",total, *(argv + 1));
   free(tids);
   return result;
}
