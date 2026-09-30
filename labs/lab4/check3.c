/* compile via:
Bash$ gcc -Wall -Werror check3.c -o check3
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>

int process_file(char *filename)
{
   unsigned long uppercase = 0;
   unsigned long lowercase = 0;
   unsigned long digits = 0;
   unsigned long punctuation = 0;
   unsigned long lines = 0;
   pid_t child_pid = getpid();

   printf("CHILD %ld: Processing \"%s\"...\n",
          (long)child_pid, filename);

   int fd = open(filename, O_RDONLY);

   if (fd == -1)
   {
      fprintf(stderr, "CHILD %ld: open() failed: %s\n",
              (long)child_pid, strerror(errno));
      return EXIT_FAILURE;
   }

   char buffer[16];
   int bytes_read;

   while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0){
      for (int i = 0; i < bytes_read; i++){
         unsigned char c = (unsigned char)buffer[i];
         if (isupper(c)){
            uppercase++;
         }if (islower(c)){
            lowercase++;
         }if (isdigit(c)){
            digits++;
         }if (ispunct(c)){
            punctuation++;
         }if (c == '\n'){
            lines++;
         }
      }
   }

   int child_status = EXIT_SUCCESS;

   if (bytes_read == -1){
      fprintf(stderr, "CHILD %ld: read() failed: %s\n",
              (long)child_pid, strerror(errno));
      child_status = EXIT_FAILURE;
   }

   if (close(fd) == -1){
      fprintf(stderr, "CHILD %ld: close() failed: %s\n",
              (long)child_pid, strerror(errno));
      child_status = EXIT_FAILURE;
   }if (child_status == EXIT_FAILURE){
      return EXIT_FAILURE;
   }

   printf("CHILD %ld: Lines: %lu (\"%s\")\n",
          (long)child_pid, lines, filename);
   printf("CHILD %ld: Uppercase characters: %lu (\"%s\")\n",
          (long)child_pid, uppercase, filename);
   printf("CHILD %ld: Lowercase characters: %lu (\"%s\")\n",
          (long)child_pid, lowercase, filename);
   printf("CHILD %ld: Digit characters: %lu (\"%s\")\n",
          (long)child_pid, digits, filename);
   printf("CHILD %ld: Punctuation characters: %lu (\"%s\")\n",
          (long)child_pid, punctuation, filename);

   return EXIT_SUCCESS;
}

int main(int argc, char **argv){
   if (argc < 2){
      fprintf(stderr, "USAGE: %s <filename> ...\n", argv[0]);
      return EXIT_FAILURE;
   }if (setvbuf(stdout, NULL, _IONBF, 0) != 0){
      fprintf(stderr, "setvbuf() failed\n");
      return EXIT_FAILURE;
   }

   struct rlimit process_limit;

   if (getrlimit(RLIMIT_NPROC, &process_limit) == -1){
      perror("getrlimit() failed");
      return EXIT_FAILURE;
   }if (process_limit.rlim_cur == RLIM_INFINITY ||
       process_limit.rlim_cur > 64){
      process_limit.rlim_cur = 64;
   }if (setrlimit(RLIMIT_NPROC, &process_limit) == -1)
   {
      perror("setrlimit() failed");
      return EXIT_FAILURE;
   }

   int number_of_files = argc - 1;
   pid_t *children = calloc(number_of_files, sizeof(pid_t));

   if (children == NULL){
      perror("calloc() failed");
      return EXIT_FAILURE;
   }

   int children_created = 0;
   int overall_status = EXIT_SUCCESS;

   for (int i = 1; i < argc; i++){
      pid_t p = fork();

      if (p == -1){
         fprintf(stderr, "PARENT %ld: fork() failed for \"%s\": %s\n",
                 (long)getpid(), argv[i], strerror(errno));
         overall_status = EXIT_FAILURE;
         continue;
      }

      if (p == 0){
         free(children);
         int result = process_file(argv[i]);
         exit(result);
      }

      children[children_created] = p;
      children_created++;
   }if (children_created == number_of_files){
      printf("PARENT %ld: All input files have been assigned to child processes\n",
             (long)getpid());
   }else{
      printf("PARENT %ld: Only %d of %d input files were assigned\n",
             (long)getpid(), children_created, number_of_files);
   }

   for (int i = 0; i < children_created; i++){
      int status;
      pid_t finished;

      do
      {
         finished = waitpid(children[i], &status, 0);
      }
      while (finished == -1 && errno == EINTR);

      if (finished == -1)
      {
         fprintf(stderr, "PARENT %ld: waitpid() failed: %s\n",
                 (long)getpid(), strerror(errno));
         overall_status = EXIT_FAILURE;
         continue;
      }

      if (WIFEXITED(status))
      {
         int exit_status = WEXITSTATUS(status);

         printf("PARENT %ld: Child process %ld terminated with exit status %d\n",
                (long)getpid(), (long)finished, exit_status);

         if (exit_status != EXIT_SUCCESS)
         {
            overall_status = EXIT_FAILURE;
         }
      }
      else if (WIFSIGNALED(status))
      {
         printf("PARENT %ld: Child process %ld terminated by signal %d\n",
                (long)getpid(), (long)finished, WTERMSIG(status));
         overall_status = EXIT_FAILURE;
      }
   }

   free(children);
   return overall_status;
}
