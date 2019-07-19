#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    float change = -1;
    do
    {
        printf("O hai! How much change is owed?\n");
        change = get_float();
    }
    while(change<0);
    
    int cents = round(change *100);
    
    int counter = 0;
    
    while(cents != 0)
    {
        if(cents >= 25)
        {
            cents -= 25;
            counter++;
        }
        
        else
        {
            if(cents >= 10)
            {
                cents -= 10;
                counter++;
            }
            
            else
            {
                if(cents >= 5)
                {
                    cents -= 5;
                    counter++;
                }
                else if(cents >=1)
                {
                    cents -= 1;
                    counter++;
                }
            }
        }
        
    }
    
    printf("%i\n", counter);
}
