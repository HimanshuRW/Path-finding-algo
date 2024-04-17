#include <iostream>
#include <conio.h>
#include <stdio.h>
using namespace std;

#define RED  "\u001b[31m"
#define RESET "\u001b[0m"
#define WALL 219
// #define WALL '█'
// #define WALL "\u001b[31m HELLO \u001b[0m" // working
// #define WALL "\u001b[31m █ \u001b[0m"

int main(){
    for (int i = 0; i < 300; i++)
    {
        char a = i;
        printf("%d : %c \n",i,i);
    }
    return 0;
}