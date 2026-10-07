/* Checkpoint 3: write two-byte values to a FIFO or a regular file. */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

/* unsigned short is checked in main before sending two-byte values. */

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

/* Read one line with scanf, checking digits before the value can overflow.
 * Return 1 for valid input, 0 for EOF, -1 for invalid input, -2 for an error.
 */
static int get_number(unsigned short *value)
{
   unsigned int number = 0;
   int digits = 0;
   int started = 0;
   int trailing_space = 0;
   int invalid = 0;
   int saw_character = 0;
   char c;
   int rc;

   while ((rc = scanf("%c", &c)) == 1)
   {
      saw_character = 1;
      if (c == '\n') break;

      if (c == ' ' || c == '\t' || c == '\r' || c == '\v' || c == '\f')
      {
         if (started) trailing_space = 1;
      }
      else if (c == '+' && !started)
      {
         started = 1;
      }
      else if (c >= '0' && c <= '9')
      {
         started = 1;
         digits = 1;
         if (trailing_space || number > 6553 ||
             (number == 6553 && c > '5'))
            invalid = 1;
         if (!invalid) number = number * 10 + (c - '0');
      }
      else
      {
         invalid = 1;
      }
   }

   if (rc == EOF && ferror(stdin)) return -2;
   if (rc == EOF && !saw_character) return 0;
   if (invalid || !digits) return -1;
   *value = (unsigned short)number;
   return 1;
}

/* One integer per input line. */
static int send_values(int fd, const char *filename)
{
   int result = EXIT_SUCCESS;

   while (1)
   {
      printf("Enter positive integer (0 to exit): ");
      fflush(stdout);
      unsigned short value;
      int rc = get_number(&value);
      if (rc == 0) break;
      if (rc == -2)
      {
         fprintf(stderr, "ERROR: input failed\n");
         result = EXIT_FAILURE;
         break;
      }
      if (rc == -1)
      {
         fprintf(stderr, "ERROR: enter an integer from 1 to 65535, or 0 to exit\n");
         continue;
      }
      if (value == 0) break;
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
      if (filename == NULL)
         printf("Wrote %u to pipe (2 bytes)\n", (unsigned int)value);
      else
         printf("Wrote %u to \"%s\" (2 bytes)\n",
                (unsigned int)value, filename);
   }

   return result;
}

int main(int argc, char **argv)
{
   if (sizeof(unsigned short) != 2)
   {
      fprintf(stderr, "ERROR: this program requires a two-byte unsigned short\n");
      return EXIT_FAILURE;
   }
   int file_mode = 0;
   char *path;

   if (argc == 2 && strcmp(*(argv + 1), "-f") != 0)
   {
      path = *(argv + 1);
   }
   else if (argc == 3 && strcmp(*(argv + 1), "-f") == 0)
   {
      file_mode = 1;
      path = *(argv + 2);
   }
   else
   {
      fprintf(stderr, "USAGE: %s {<fifo-name> | -f <filename>}\n", *argv);
      return EXIT_FAILURE;
   }
   if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
   {
      perror("signal() failed");
      return EXIT_FAILURE;
   }

   /* A FIFO must already exist. A regular output file may be new. */
   struct stat info;
   if (stat(path, &info) == -1)
   {
      if (file_mode == 0 || errno != ENOENT)
      {
         perror("stat() failed");
         return EXIT_FAILURE;
      }
   }
   else if ((file_mode == 0 && S_ISFIFO(info.st_mode) == 0) ||
            (file_mode == 1 && S_ISREG(info.st_mode) == 0))
   {
      fprintf(stderr, "ERROR: use a FIFO without -f, or a regular file with -f\n");
      return EXIT_FAILURE;
   }

   /* File mode creates or clears the output; FIFO mode waits for a reader. */
   int fd;
   do
   {
      if (file_mode == 1)
         fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0600);
      else
         fd = open(path, O_WRONLY);
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
   if ((file_mode == 0 && S_ISFIFO(info.st_mode) == 0) ||
       (file_mode == 1 && S_ISREG(info.st_mode) == 0))
   {
      fprintf(stderr, "ERROR: opened file has the wrong type\n");
      close_fd(fd);
      return EXIT_FAILURE;
   }

   int result;
   if (file_mode == 1)
      result = send_values(fd, path);
   else
      result = send_values(fd, NULL);
   if (close_fd(fd) != EXIT_SUCCESS) result = EXIT_FAILURE;
   return result;
}
