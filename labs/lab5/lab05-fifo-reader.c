/* Checkpoint 2: run in a separate terminal from the writer. */
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

/* A pipe is a byte stream: collect a complete two-byte value before printing. */
static int receive_values(int fd, const char *prefix)
{
   for (;;)
   {
      uint16_t value;
      size_t received = 0;
      while (received < sizeof(value))
      {
         ssize_t n = read(fd, (char *)&value + received,
                          sizeof(value) - received);
         if (n == -1)
         {
            if (errno == EINTR) continue;
            perror("read() failed");
            return EXIT_FAILURE;
         }
         if (n == 0)
         {
            if (received == 0) return EXIT_SUCCESS;
            fprintf(stderr, "ERROR: incomplete value received\n");
            return EXIT_FAILURE;
         }
         received += (size_t)n;
      }
      printf("%sRead %u from pipe (2 bytes)\n", prefix, (unsigned int)value);
   }
}

int main(int argc, char **argv)
{
   if (argc != 2)
   {
      fprintf(stderr, "USAGE: %s <fifo-path>\n", *argv);
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
      fd = open(*(argv + 1), O_RDONLY);
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

   int result = receive_values(fd, "");
   if (close_fd(fd) != EXIT_SUCCESS) result = EXIT_FAILURE;
   return result;
}
