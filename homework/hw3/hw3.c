/* HW3: MIPS mini assembler. */
/* gcc -Wall -Werror hw3.c */
/* ./hw3.out hw3-input01.s */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>

#define START_ADDRESS 0x00400000

typedef struct trie {
   char valid;
   int address;                 /* Address stored at the last letter of a label */
   struct trie *next;           /* Next layer of 26 uppercase letters */
} trie_t;

typedef struct instruction {
   char *line;                  /* Owns the memory containing this instruction */
   char *operation;
   char *first;
   char *second;
   char *third;
   int address;
   struct instruction *next;    /* Next source instruction, in file order */
} instruction_t;


int add_label(trie_t *trie, const char *word, int address){ /* Same traversal as HW2 add_word; save an address at the final letter */
   for (int i = 0; *(word + i) != '\0'; i++) {
      int index = *(word + i) - 'A';
      trie_t *current = trie + index;

      if (*(word + i + 1) == '\0') {
         current->valid = 1;
         current->address = address;
      } else {
         if (current->next == NULL) {
            current->next = calloc(26, sizeof(trie_t));
            if (current->next == NULL) {
               perror("ERROR: calloc");
               return -1;
            }
         }
         trie = current->next;
      }
   }
   return 0;
}


int find_label(const trie_t *trie, const char *word){  /* uses similar logic for traversal as HW2 is_valid_word; return the label's address */
   for (int i = 0; *(word + i) != '\0'; i++) {
      int index = *(word + i) - 'A';
      const trie_t *current = trie + index;

      if (*(word + i + 1) == '\0') {
         if (current->valid){
            return current->address;
         } 
         break;
      }
      if (current->next == NULL) break;
      trie = current->next;
   }
   fprintf(stderr, "ERROR: label not found\n");
   return -1;
}

void free_trie(trie_t *trie){
   if (trie == NULL) return;
   for (int i = 0; i < 26; i++) {
      free_trie((trie + i)->next);
   }
   free(trie);
}

void free_instructions(instruction_t *head){
   while (head != NULL) {
      instruction_t *next = head->next;
      free(head->line);
      free(head);
      head = next;
   }
}

/* Read one complete line. Return 1 for a line, 0 for EOF, -1 for error */
int read_line(int fd, char **line)
{
   int capacity = 128;
   int length = 0;
   char *buffer = calloc(capacity, sizeof(char));
   if (buffer == NULL) {
      perror("ERROR: calloc");
      return -1;
   }

   while (1) {
      char byte;
      int r = read(fd, &byte, 1); /* Reading one byte returns only -1, 0, or 1 */
      if (r == -1) {
         perror("ERROR: read");
         free(buffer);
         return -1;
      }
      if (r == 0 && length == 0) {
         free(buffer);
         return 0;
      }
      if (r == 0 || byte == '\n') {
         *(buffer + length) = '\0';
         *line = buffer;
         return 1;
      }

      if (length + 1 == capacity) {
         capacity *= 2;
         char *larger = realloc(buffer, capacity);
         if (larger == NULL) {
            perror("ERROR: realloc");
            free(buffer);
            return -1;
         }
         buffer = larger;
      }
      *(buffer + length) = byte;
      length++;
   }
}


char *next_token(char **text){ /* Separate tokens at whitespace, commas, and memory-operand parentheses. */
   char *ptr = *text;
   while (*ptr != '\0' && (isspace(*ptr) || *ptr == ',' || *ptr == '(' || *ptr == ')')) {
      ptr++;
   }
   if (*ptr == '\0') {
      *text = ptr;
      return NULL;
   }

   char *token = ptr;
   while (*ptr != '\0' && !isspace(*ptr) && *ptr != ',' && *ptr != '(' && *ptr != ')') {
      ptr++;
   }
   if (*ptr != '\0') {
      *ptr = '\0';
      ptr++;
   }
   *text = ptr;
   return token;
}

/* First pass: save labels and source instructions, counting expanded sizes  */
int load_instructions(int fd, trie_t *trie, instruction_t **head){
   instruction_t *tail = NULL;
   int address = START_ADDRESS;

   while (1) {
      char *line = NULL;
      int r = read_line(fd, &line);
      if (r == -1){
         return -1;
      } 
      if (r == 0){
         return 0;
      } 

      char *comment = strchr(line, '#');
      if (comment != NULL) *comment = '\0';

      char *ptr = line;
      while (isspace(*ptr)) ptr++;
      if (*ptr == '\0') {
         free(line);
         continue;
      }

      char *colon = strchr(ptr, ':');
      if (colon != NULL) {
         *colon = '\0';
         if (add_label(trie, ptr, address) == -1) {
            free(line);
            return -1;
         }
         ptr = colon + 1;
      }

      instruction_t *current = calloc(1, sizeof(instruction_t));
      if (current == NULL) {
         perror("ERROR: calloc");
         free(line);
         return -1;
      }
      current->line = line;
      current->operation = next_token(&ptr);
      current->first = next_token(&ptr);
      current->second = next_token(&ptr);
      current->third = next_token(&ptr);
      current->address = address;

      if (*head == NULL){
         *head = current;
      } 
      else tail->next = current;
      tail = current;

      address += 4;
      char *op = current->operation;
      if (strcmp(op, "bge") == 0 || strcmp(op, "bgt") == 0 || strcmp(op, "ble") == 0 || strcmp(op, "blt") == 0) {
         address += 4;          /* These produce slt plus a branch */
      }
   }
}


int register_number(const char *name){ /* Translated the register names used on the MIPS reference card. */
   if (name == NULL) return 0;
   if (strcmp(name, "$at") == 0) return 1;
   if (strcmp(name, "$gp") == 0) return 28;
   if (strcmp(name, "$sp") == 0) return 29;
   if (strcmp(name, "$fp") == 0 || strcmp(name, "$s8") == 0) return 30;
   if (strcmp(name, "$ra") == 0) return 31;
   if (strcmp(name, "$zero") == 0 || strcmp(name, "$0") == 0) return 0;

   if (*name == '$' && *(name + 1) != '\0' && *(name + 2) != '\0') {
      int number = *(name + 2) - '0';
      if (*(name + 1) == 'v') return 2 + number;
      if (*(name + 1) == 'a') return 4 + number;
      if (*(name + 1) == 's') return 16 + number;
      if (*(name + 1) == 'k') return 26 + number;
      if (*(name + 1) == 't') {
         if (number < 8) return 8 + number;
         return 16 + number;
      }
   }

   
   return 0;  /* Supplied input04 used 7777 as a register so the expected field is zero. */
}

/* Unsigned values keep shifts into bit 31. */
unsigned int make_r(unsigned int rs, unsigned int rt, unsigned int rd, unsigned int shift, unsigned int function){ 
   unsigned int word = rs << 21;
   word |= rt << 16;
   word |= rd << 11;
   word |= shift << 6;
   word |= function;
   return word;
}

unsigned int make_i(unsigned int opcode, unsigned int rs, unsigned int rt, int immediate){
   unsigned int word = opcode << 26;
   unsigned int bits = immediate; /* Negative values retain their low bits */
   word |= rs << 21;
   word |= rt << 16;
   word |= bits & 0xFFFFu; /* mask to preserve only the lowest 16 bits*/
   return word;
}

/* Print exactly 32 digits, most significant bit first, then a newline. */
int print_binary(unsigned int word){
   unsigned int one = 1;
   for (int i = 31; i >= 0; i--) {
      char bit = '0';
      if ((word & (one << i)) != 0) bit = '1';
      if (putchar(bit) == EOF) {
         perror("ERROR: output");
         return -1;
      }
   }
   if (putchar('\n') == EOF) {
      perror("ERROR: output");
      return -1;
   }
   return 0;
}

/* Second pass: encode one source instruction, or both words of a pseudo. */
int assemble_instruction(const instruction_t *current, const trie_t *trie)
{
   const char *op = current->operation;
   int first = register_number(current->first);
   int second = register_number(current->second);
   unsigned int word = 0;

   if (strcmp(op, "j") == 0 || strcmp(op, "jal") == 0) {
      int address = find_label(trie, current->first);
      if (address == -1) return -1;
      unsigned int opcode = 2;
      if (strcmp(op, "jal") == 0) opcode = 3;
      unsigned int target = address;
      word = opcode << 26;
      word |= (target >> 2) & 0x03FFFFFFu;
   } else if (strcmp(op, "jr") == 0) {
      word = make_r(first, 0, 0, 0, 8);
   } else if (strcmp(op, "move") == 0) {
      word = make_r(second, 0, first, 0, 32);
   } else if (strcmp(op, "li") == 0) {
      int immediate = atoi(current->second);
      /* The supplied output07 uses addi for positive and negative li. */
      word = make_i(8, 0, first, immediate);
   } else if (strcmp(op, "beq") == 0 || strcmp(op, "bne") == 0 ||
              strcmp(op, "bge") == 0 || strcmp(op, "bgt") == 0 ||
              strcmp(op, "ble") == 0 || strcmp(op, "blt") == 0) {
      int address = find_label(trie, current->third);
      if (address == -1) return -1;
      int branch_address = current->address;
      int opcode = 4;
      int rs = first;
      int rt = second;

      if (strcmp(op, "beq") != 0 && strcmp(op, "bne") != 0) {
         /* slt $at,left,right; reverse comparison for ble and bgt. */
         if (strcmp(op, "ble") == 0 || strcmp(op, "bgt") == 0) {
            word = make_r(second, first, 1, 0, 42);
         } else {
            word = make_r(first, second, 1, 0, 42);
         }
         if (print_binary(word) == -1) return -1;
         branch_address += 4;  /* The branch is the SECOND generated word */
         rs = 1;               /* $at */
         rt = 0;               /* $zero must occupy rt */
      }

      if (strcmp(op, "bne") == 0 || strcmp(op, "blt") == 0 ||
          strcmp(op, "bgt") == 0) opcode = 5;
      int offset = (address - (branch_address + 4)) / 4;
      word = make_i(opcode, rs, rt, offset);
   } else if (strcmp(op, "sll") == 0 || strcmp(op, "srl") == 0) {
      int function = 0;
      if (strcmp(op, "srl") == 0) function = 2;
      word = make_r(0, second, first, atoi(current->third), function);
   } else if (strcmp(op, "lw") == 0 || strcmp(op, "sw") == 0 ||
              strcmp(op, "sb") == 0 || strcmp(op, "sh") == 0) {
      int opcode = 35;
      if (strcmp(op, "sw") == 0) opcode = 43;
      if (strcmp(op, "sb") == 0) opcode = 40;
      if (strcmp(op, "sh") == 0) opcode = 41;
      int base = register_number(current->third);
      word = make_i(opcode, base, first, atoi(current->second));
   } else if (strcmp(op, "addi") == 0 || strcmp(op, "andi") == 0 ||
              strcmp(op, "ori") == 0 || strcmp(op, "subi") == 0) {
      int opcode = 8;
      int immediate = atoi(current->third);
      if (strcmp(op, "andi") == 0) opcode = 12;
      if (strcmp(op, "ori") == 0) opcode = 13;
      if (strcmp(op, "subi") == 0) immediate = -immediate;
      word = make_i(opcode, second, first, immediate);
   } else {
      int function = 0;
      if (strcmp(op, "add") == 0) function = 32;
      else if (strcmp(op, "sub") == 0) function = 34;
      else if (strcmp(op, "and") == 0) function = 36;
      else if (strcmp(op, "or") == 0) function = 37;
      else if (strcmp(op, "nor") == 0) function = 39;
      else {
         fprintf(stderr, "ERROR: unsupported instruction\n");
         return -1;
      }
      int third = register_number(current->third);
      word = make_r(second, third, first, 0, function);
   }
   return print_binary(word);
}

int main(int argc, char **argv)
{
   setvbuf(stdout, NULL, _IONBF, 0);
   if (argc != 2) {
      fprintf(stderr, "ERROR: expected one assembly filename\n");
      return EXIT_FAILURE;
   }

   int fd = open(*(argv + 1), O_RDONLY);
   if (fd == -1) {
      perror("ERROR: open");
      return EXIT_FAILURE;
   }

   trie_t *trie = calloc(26, sizeof(trie_t));
   if (trie == NULL) {
      perror("ERROR: calloc");
      close(fd);
      return EXIT_FAILURE;
   }

   instruction_t *head = NULL;
   int loaded = load_instructions(fd, trie, &head);
   int closed = close(fd);
   if (loaded == -1 || closed == -1) {
      if (loaded != -1) perror("ERROR: close");
      free_instructions(head);
      free_trie(trie);
      return EXIT_FAILURE;
   }

   const instruction_t *current = head;
   while (current != NULL) {
      if (assemble_instruction(current, trie) == -1) {
         free_instructions(head);
         free_trie(trie);
         return EXIT_FAILURE;
      }
      current = current->next;
   }

   int printed = puts("end");
   free_instructions(head);
   free_trie(trie);
   if (printed == EOF) {
      perror("ERROR: output");
      return EXIT_FAILURE;
   }
   return EXIT_SUCCESS;
}
