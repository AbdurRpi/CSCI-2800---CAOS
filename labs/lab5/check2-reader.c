/* Checkpoint 2: run in a separate terminal from the writer. */

/*  gcc -Wall -Werror check2-reader.c -o check2-reader.out */

/* mkfifo /tmp/check2-reader */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

/* unsigned short is checked in main before receiving two-byte values. */

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
   while(1)
   {
      unsigned short value;
      int received = 0;
      while (received < sizeof(value))
      {
         int n = read(fd, (char *)&value + received,
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
         received += (int)n;
      }
      printf("CHILD: %sRead %u from pipe (2 bytes)\n", prefix, (unsigned int)value);
   }
}

int main(int argc, char **argv)
{
   if (sizeof(unsigned short) != 2)
   {
      fprintf(stderr, "ERROR: this program requires a two-byte unsigned short\n");
      return EXIT_FAILURE;
   }
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
   if (S_ISFIFO(info.st_mode) == 0)
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
   if (S_ISFIFO(info.st_mode) == 0)
   {
      fprintf(stderr, "ERROR: opened descriptor is not a FIFO\n");
      close_fd(fd);
      return EXIT_FAILURE;
   }

   int result = receive_values(fd, "");
   if (close_fd(fd) != EXIT_SUCCESS) result = EXIT_FAILURE;
   return result;
}
