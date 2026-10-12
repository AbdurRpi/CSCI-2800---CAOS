/* Hw2 */
/*  gcc -Wall -Werror check3-writer.c -o check3-writer.out */
/* mkfifo /tmp/check3-writer */
/*  ./check3-writer.out -f output.txt */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAXWORDLENGTH 16

typedef struct trie{
   char valid;
   struct trie *next;
}
trie_t;

void print_trie_helper(const trie_t *trie, char *prefix){
   const trie_t *ptr = trie;

   for(int i = 0; i < 26; i++){ /* visit each of the 26 elements of this layer of the Trie */
      int len = strlen(prefix); 
      *(prefix + len) = 'A' + i; // append next letter
      if( (*(ptr + i)).valid )
         printf("==> %s\n", prefix);
      if((*(ptr + i)).next)
         print_trie_helper((*(ptr + i)).next, prefix); // recurse to the next level if present

      *(prefix + len) = '\0';
   }
}

void print_trie(const trie_t * trie){
   if (trie == NULL) return;
   char * prefix = calloc( MAXWORDLENGTH + 1, sizeof(char));
   if ( prefix == NULL) {perror("calloc() failed"); exit(EXIT_FAILURE);}
   print_trie_helper( trie, prefix);
}

void add_word(trie, word){
   for(int i = 0; *(prefix + i) < 26; i++){
      if((*(ptr + i)).valid){
         *(trie + i).next
         return prefix.valid; 
      }
      
   }
}

void load_dictionary(filename, trie){
   int fd = open(filename, O_RDONLY);
   if (fd == -1) {perror("open failed"); return EXIT_FAILURE;}
   int n = read(filename, *prefix, sizeof(char));
   if( n == -1){perror("read() failed");}
   for((*(word) == prefix.valid)){
      upper(word);
      add_word(trie, word);
      close(filename);
   }
}

void read_candidate(read_fifo){ // continue here where you left off.


}

void make_response(trie, candidate){

}


int main(int argc, ** argv){
   if (argv != 4){
      fprintf("Invalid arguments <dictionary-file> <read-fifo> <write-fifo>");
      return EXIT_SUCCESS;
   }
}