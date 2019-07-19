/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    
    if(n >= 1){
        int first = 0;
        int last = n-1;
        int middle = (first+last)/2;
        
        while(first<=last){
            if(values[middle]<value)
                first = middle+1;
            else if(values[middle]>value)
                last = middle-1;
            else if (values[middle] == value)
                return true;
            
            middle = (first+last)/2;
        }
    }
    return false;
}



/**
 * Sorts array of n values.
 * This is bubble sort btw.
 */
void sort(int values[], int n)
{
    for(int i = 0; i<n;i++){
        for(int j = i+1; j<n; j++){
            if(values[i]>values[j]){
                int a = values[i];
                values[i] = values[j];
                values[j] = a;
            }
        }
    }
    return;
}
