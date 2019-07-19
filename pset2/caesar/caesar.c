#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
 
 int main(int argc, string argv[])
 {
    
     if(argc != 2){
         printf("Sorry, that's not a valid input\n");
         return 1;
     }
     
     int k = atoi(argv[1]);
     
     printf("plaintext: ");
     string text = get_string();
     printf("ciphertext: ");
     
     for(int i = 0; i < strlen(text);i++)
     {
         if (islower(text[i]))
         {
             printf("%c", ((text[i]+k-97)%26)+97);
         }
         else if (isupper(text[i])){
             printf("%c", ((text[i]+k-65)%26)+65);
         }
         else printf("%c", text[i]);
     }
     
    printf("\n");
 
     
     
 }