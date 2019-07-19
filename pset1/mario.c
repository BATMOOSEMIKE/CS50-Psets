#include <stdio.h>
#include <cs50.h>

int main(void)
{
    
    int height;
    do
    {
        printf("Height: ");
        height = get_int();
    }
    while(height < 0 || height>23);
    
    for (int i = 1; i <= height;i++)
    {
        int rownum = i;
        for(int x = 1; x <= height-rownum; x++){
            printf(" ");
        }
        for (int a = 1; a <= rownum+1; a++){
            printf("#");
        }
        printf("\n");
    }
  
    

}