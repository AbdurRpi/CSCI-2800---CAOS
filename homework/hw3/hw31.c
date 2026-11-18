/* Hw3 */

/* gcc -Wall -Werror hw2.c -o hw2.out  */
/* mkfifo /tmp/hw2-read /tmp/hw2-write */

/* ./hw2r.out input.txt /tmp/hw2-read /tmp/hw2-write  */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <sys/wait.h>


#define MAXBITLENGTH 128

typedef struct trie{
   char valid;
   struct trie *next;
}
trie_t;


void Label(const trie_t *trie, char *prefix){

}

void Instruction(){

}

void ParsedLine(){

}

void Collections(){

}

int main(int argc, char **argv){
   setvbuf(stdout, NULL, _IONBF,0);
   if (argc != 2){
      fprintf(stderr, "");
      return EXIT_FAILURE;
   }
trie_t *trie = calloc(32), sizeof(trie_t);
if()



}