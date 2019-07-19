#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    //printf("What's your name?\n");
    string name = get_string();
    printf("%c", toupper(name[0]));
    for(int i = 0; i<strlen(name);i++){
        if(isspace(name[i])){
            printf("%c", toupper(name[i+1]));
        }
    }
    printf("\n");

}

