#include <stdio.h>

//gcc -m32 -fno-stack-protector -z execstack unsafe.c -o unsafe

int main() {

    int number = 0;
    char name[10];

    printf("Welcome to our Lottery System!!!\n");
    printf("Please enter your name:\n");

    fgets(name, 20, stdin);

    printf("Welcome %s\n", name);

    printf("Checking your number.......\n");
    if(number == 0x534e4957) {
        printf("\033[32;1mYou did it!\033[0m\n");
        printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
    }
    else {
        printf("You lose! Better luck next time!\n");
    }
    return 0;
}

