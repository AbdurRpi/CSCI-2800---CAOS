/* Checkpoint 2: run in a separate terminal from the reader. */

/* gcc -Wall -Werror lab05-fifo-writer.c -o lab05-fifo-writer.out */

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <ctype.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

_Static_assert(sizeof(uint16_t) == 2, "This program requires two-byte values");

/* Report close failures without retrying a descriptor that may be closed. */
static int close_fd(int fd)
{
   if (close(fd) == -1)
   {
      perror("close() failed");
      return EXIT_FAILURE;
   }
   return EXIT_SUCCESS;
}

/* One integer per input line; validate before converting to uint16_t. */
static int send_values(int fd, const char *prefix)
{
   char *line = NULL;
   size_t capacity = 0;
   int result = EXIT_SUCCESS;

   for (;;)
   {
      printf("%sEnter positive integer (0 to exit): ", prefix);
      fflush(stdout);
      ssize_t length = getline(&line, &capacity, stdin);
      if (length == -1)
      {
         if (!feof(stdin))
         {
            perror("input failed");
            result = EXIT_FAILURE;
         }
         break;
      }

      /* Reject embedded null bytes rather than accepting a hidden suffix. */
      int invalid = 0;
      for (ssize_t i = 0; i < length; i++)
         if (*(line + i) == '\0') invalid = 1;

      char *end;
      errno = 0;
      long number = strtol(line, &end, 10);
      if (end == line || errno == ERANGE) invalid = 1;
      while (isspace((unsigned char)*end)) end++;
      if (*end != '\0' || number < 0 || number > 65535) invalid = 1;

      if (invalid)
      {
         fprintf(stderr, "ERROR: enter an integer from 1 to 65535, or 0 to exit\n");
         continue;
      }
      if (number == 0) break;

      uint16_t value = (uint16_t)number;
      size_t sent = 0;
      while (sent < sizeof(value))
      {
         ssize_t n = write(fd, (const char *)&value + sent,
                           sizeof(value) - sent);
         if (n == -1)
         {
            if (errno == EINTR) continue;
            perror("write() failed");
            result = EXIT_FAILURE;
            break;
         }
         if (n == 0)
         {
            fprintf(stderr, "ERROR: write() made no progress\n");
            result = EXIT_FAILURE;
            break;
         }
         sent += (size_t)n;
      }
      if (result == EXIT_FAILURE) break;
      printf("%sWrote %u to pipe (2 bytes)\n", prefix, (unsigned int)value);
   }

   free(line);
   return result;
}

int main(int argc, char **argv)
{
   if (argc != 2)
   {
      fprintf(stderr, "USAGE: %s <fifo-path>\n", *argv);
      return EXIT_FAILURE;
   }
   if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
   {
      perror("signal() failed");
      return EXIT_FAILURE;
   }

   /* Create this FIFO once with mkfifo before starting the two programs. */
   struct stat info;
   if (stat(*(argv + 1), &info) == -1)
   {
      perror("stat() failed (create the named pipe with mkfifo first)");
      return EXIT_FAILURE;
   }
   if (!S_ISFIFO(info.st_mode))
   {
      fprintf(stderr, "ERROR: path must name a FIFO, not a regular file\n");
      return EXIT_FAILURE;
   }

   /* Blocking here until the other end opens is normal. */
   int fd;
   do
   {
      fd = open(*(argv + 1), O_WRONLY);
   }
   while (fd == -1 && errno == EINTR);
   if (fd == -1)
   {
      perror("open() failed");
      return EXIT_FAILURE;
   }
   if (fstat(fd, &info) == -1)
   {
      perror("fstat() failed");
      close_fd(fd);
      return EXIT_FAILURE;
   }
   if (!S_ISFIFO(info.st_mode))
   {
      fprintf(stderr, "ERROR: opened descriptor is not a FIFO\n");
      close_fd(fd);
      return EXIT_FAILURE;
   }

   int result = send_values(fd, "");
   if (close_fd(fd) != EXIT_SUCCESS) result = EXIT_FAILURE;
   return result;
}
