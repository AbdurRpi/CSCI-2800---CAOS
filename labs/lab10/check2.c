/* compile via:
Bash$ gcc -Wall  -Werror checkpoint2.c -o checkpoint2
Bash$ ./checkpoint2 line input.txt another.txt
*/

#include <stdio.h>      /* perror only; file data uses open/read/write/close */
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

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

/* Called by one child. fd is the descriptor inherited from its parent.
   Return the match count on success or EXIT_FAILURE after reporting an error.
   EXIT_FAILURE can have the same numeric value as a valid match count. */
int search_file(int fd, const char *substring)
{
   /* NULL lets the first realloc behave like malloc. length counts file bytes. */
   char *buffer = NULL;
   int length = 0;
   unsigned int occurrences = 0;

   /* Read the file into one growing buffer. Each read asks for 4096 bytes. */
   while (1)
   {
      /* Keep the old pointer until allocation succeeds, so it can still be freed. */
      char *larger = realloc(buffer, length + 4096);
      if (larger == NULL)
      {
         perror("realloc() failed");
         free(buffer);
         close(fd);
         return EXIT_FAILURE;
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
         return EXIT_FAILURE;
      }
      /* A return value of zero means end-of-file. */
      if (bytes_read == 0)
      {
         break;
      }
      length += bytes_read;
   }

   /* The entire file is in memory, so this child no longer needs its descriptor. */
   if (close(fd) == -1)
   {
      perror("close() failed");
      free(buffer);
      return EXIT_FAILURE;
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

      /* Combine the context and newline into one output record. */
      /* end is exclusive. The extra byte holds the output newline, not a NUL. */
      int output_length = end - start + 1;
      char *output = malloc(output_length);
      if (output == NULL)
      {
         perror("malloc() failed");
         free(buffer);
         return EXIT_FAILURE;
      }
      memcpy(output, buffer + start, end - start);
      *(output + output_length - 1) = '\n';
      int result = write_all(STDOUT_FILENO, output, output_length);
      free(output);
      if (result == -1)
      {
         perror("write() failed");
         free(buffer);
         return EXIT_FAILURE;
      }
      occurrences++;
   }

   free(buffer);
   /* Only eight bits are available in the child exit status: 256 wraps to zero. */
   return (int)(occurrences % 256);
}

int main(int argc, char **argv)
{
   /* Argument 1 is the search text; arguments 2 onward are filenames.
      Reject newline searches so a match itself cannot cross a line boundary. */
   if (argc < 3 || **(argv + 1) == '\0' || strchr(*(argv + 1), '\n') != NULL)
   {
      const char *message = "USAGE: ./checkpoint2 <substring> <filename> ...\n";
      write_all(STDERR_FILENO, message, strlen(message));
      return EXIT_FAILURE;
   }

   int child_count = 0;
   int result = EXIT_SUCCESS;

   for (int i = 2; i < argc; i++)
   {
      /* The assignment requires the PARENT to open each file before fork. */
      int fd = open(*(argv + i), O_RDONLY);

      if (fd == -1)
      {
         perror(*(argv + i));
         result = EXIT_FAILURE;
         continue;
      }

      /* Both processes keep running here. The child receives its own descriptor
         referring to the same open file. Only the child will read from it. */
      pid_t pid = fork();

      if (pid == -1)
      {
         perror("fork() failed");
         close(fd);
         result = EXIT_FAILURE;
         continue;
      }

      /* Zero identifies the child. Exit here so it never forks more children. */
      if (pid == 0)
      {
         int child_status = search_file(fd, *(argv + 1));
         exit(child_status);
      }

      /* Only the parent reaches this point. Closing its copy leaves the
         child's descriptor open. Continue launching the remaining children. */
      child_count++;

      if (close(fd) == -1)
      {
         perror("close() failed");
         result = EXIT_FAILURE;
      }
   }

   /* -1 means wait for any child. Launch all children before waiting. */
   while (child_count > 0)
   {
      int status;
      /* -1 selects any child; 0 blocks until one finishes.
         Reaping each child prevents it from remaining a zombie. */
      pid_t waited = waitpid(-1, &status, 0);

      if (waited == -1)
      {
         if (errno == EINTR)
         {
            continue;
         }

         perror("waitpid() failed");
         return EXIT_FAILURE;
      }

      child_count--;

      if (WIFSIGNALED(status))
      {
         const char *message = "ERROR: child terminated by a signal\n";
         write_all(STDERR_FILENO, message, strlen(message));
         result = EXIT_FAILURE;
         
      }
      /* After a normal exit, WEXITSTATUS(status) gives the child's count.
         Nonzero match counts are normal, not parent failures. */
         printf("Child %ld returned %d\n", (long)waited, WEXITSTATUS(status));
   }

   return result;
}
