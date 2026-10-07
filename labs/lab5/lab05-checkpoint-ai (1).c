/* Checkpoint 1: parent sends two-byte integers to its child. */
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

int main(void)
{
   /* SIGPIPE becomes a checked write error if the child disappears. */
   if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
   {
      perror("signal() failed");
      return EXIT_FAILURE;
   }
   if (setvbuf(stdout, NULL, _IONBF, 0) != 0)
   {
      fprintf(stderr, "setvbuf() failed\n");
      return EXIT_FAILURE;
   }

   int *pipefd = malloc(2 * sizeof(int));
   if (pipefd == NULL)
   {
      perror("malloc() failed");
      return EXIT_FAILURE;
   }
   if (pipe(pipefd) == -1)
   {
      perror("pipe() failed");
      free(pipefd);
      return EXIT_FAILURE;
   }

   pid_t p = fork();
   if (p == -1)
   {
      perror("fork() failed");
      close_fd(*pipefd);
      close_fd(*(pipefd + 1));
      free(pipefd);
      return EXIT_FAILURE;
   }

   if (p == 0)
   {
      int fd = *pipefd;
      int result = close_fd(*(pipefd + 1));
      free(pipefd);
      if (result == EXIT_SUCCESS)
         result = receive_values(fd, "CHILD: ");
      if (close_fd(fd) != EXIT_SUCCESS) result = EXIT_FAILURE;
      return result;
   }

   int fd = *(pipefd + 1);
   int result = close_fd(*pipefd);
   free(pipefd);
   if (result == EXIT_SUCCESS)
      result = send_values(fd, "PARENT: ");

   /* Close BEFORE waiting: the child needs EOF to finish reading. */
   if (close_fd(fd) != EXIT_SUCCESS) result = EXIT_FAILURE;

   int status;
   pid_t finished;
   do
   {
      finished = waitpid(p, &status, 0);
   }
   while (finished == -1 && errno == EINTR);

   if (finished == -1)
   {
      perror("waitpid() failed");
      return EXIT_FAILURE;
   }
   if (WIFSIGNALED(status))
   {
      fprintf(stderr, "CHILD: terminated by signal %d\n", WTERMSIG(status));
      result = EXIT_FAILURE;
   }
   else if (!WIFEXITED(status) || WEXITSTATUS(status) != EXIT_SUCCESS)
   {
      result = EXIT_FAILURE;
   }
   return result;
}
