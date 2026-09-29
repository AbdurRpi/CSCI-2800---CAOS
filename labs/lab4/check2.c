/* compile via:
Bash$ gcc -Wall -Werror check3.c
Bash$ gcc -E -wall -werror simple.c <== preprocessor 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <ctype.h>


int main(int argc, char **argv)
{
   unsigned long uppercase = 0;
   unsigned long lowercase = 0;
   unsigned long digits = 0;
   unsigned long punctuation = 0;
   unsigned long lines = 0;
   int files_processed = 0;

   if (argc < 2)
   {
      fprintf(stderr, "USAGE: %s <filename> ...\n", argv[0]);
      return EXIT_FAILURE;
   }

   for (int i = 1; i < argc; i++)
   {
      printf("Processing \"%s\"...\n", argv[i]);

      int fd = open(argv[i], O_RDONLY);

      if (fd == -1)
      {
         perror("open() failed");
         continue;
      }

      files_processed++;

      char buffer[4096];
      ssize_t bytes_read;

      while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0)
      {
         for (ssize_t j = 0; j < bytes_read; j++)
         {
            unsigned char c = (unsigned char)buffer[j];

            if (isupper(c))
            {
               uppercase++;
            }

            if (islower(c))
            {
               lowercase++;
            }

            if (isdigit(c))
            {
               digits++;
            }

            if (ispunct(c))
            {
               punctuation++;
            }

            if (c == '\n')
            {
               lines++;
            }
         }
      }

      if (bytes_read == -1)
      {
         perror("read() failed");
      }

      if (close(fd) == -1)
      {
         perror("close() failed");
      }
   }

   printf("SUMMARY AFTER PROCESSING %lu LINES IN %d FILES:\n",
          lines, files_processed);
   printf("==> uppercase characters: %lu\n", uppercase);
   printf("==> lowercase characters: %lu\n", lowercase);
   printf("==> digit characters: %lu\n", digits);
   printf("==> punctuation characters: %lu\n", punctuation);

   return EXIT_SUCCESS;
}