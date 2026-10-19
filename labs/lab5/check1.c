/* lab05-checkpoint1-ai.c */
/*   gcc -Wall -Werror check1.c    */

/* Checkpoint 1: parent sends two-byte integers to its child. */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>


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
static int send_values(int fd, const char *prefix)
{
   int result = EXIT_SUCCESS;

   while (1)
   {
      printf("%sEnter positive integer (0 to exit): ", prefix);
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
      int sent = 0;
      while (sent < sizeof(value))
      {
         int n = write(fd, (const char *)&value + sent,
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
         sent += (int)n;
      }
      if (result == EXIT_FAILURE) break;
      printf("%sWrote %u to pipe (2 bytes)\n", prefix, (unsigned int)value);
   }

   return result;
}

static int receive_values(int fd, const char *prefix)
{
   while (1)
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
      printf("%sRead %u from pipe (2 bytes)\n", prefix, (unsigned int)value);
   }
}

int main(void)
{
   if (sizeof(unsigned short) != 2)
   {
      fprintf(stderr, "ERROR: this program requires a two-byte unsigned short\n");
      return EXIT_FAILURE;
   }
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
