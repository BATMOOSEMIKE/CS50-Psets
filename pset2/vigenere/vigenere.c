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


     for(int i = 0; i<strlen(argv[1]); i++){
         if(isalpha(argv[1][i]) == 0){
             printf("Sorry, you must enter a key that is purely alphabetical\n");
             return 1;
         }
     }

     string key = argv[1];





     printf("plaintext: ");
     string text = get_string();
     printf("ciphertext: ");

     int locationInKey = 0;


     for(int i = 0; i < strlen(text);i++)
     {
         locationInKey = locationInKey % strlen(key);


         if (islower(text[i]) && islower(key[locationInKey]))
         {
             int difference = (key[locationInKey])-97;
             printf("%c", ((text[i]-97+difference)%26)+97);
         }
         else if (isupper(text[i]) && islower(key[locationInKey]))
         {

             int difference = (key[locationInKey])-97;
             printf("%c", ((text[i]+difference-65)%26)+65);
         }
         else if (islower(text[i])&&isupper(key[locationInKey])){
             int difference = (key[locationInKey])-65;
             printf("%c", ((text[i]+difference-97)%26)+97);
         }
         else if (isupper(text[i]) && isupper(key[locationInKey]))
         {
             int difference = (key[locationInKey])-65;
             printf("%c", ((text[i]+difference-65)%26)+65);
         }

         if(isalpha(text[i])){
             locationInKey++;
         }
         else{
             printf("%c", text[i]);
         }



     }

    printf("\n");



 }