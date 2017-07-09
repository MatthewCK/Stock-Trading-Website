/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>
#include <stdio.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // TODO: implement a searching algorithm
    int fir = 0;
    int mid = (n-1)/2;
    int last = n - 1;
    while (fir <= last)
    {
        if (values[mid] == value)
            return true;
        
        if (values[mid] < value)
            fir = mid + 1;
        else
            last = mid - 1;
            
        mid = (last + fir) / 2;
        
    }
    
    return false;
}



/*void swap(int *v, int *b)
{
    int temp = *v;
    *v = *b;
    *b = temp;
}
*/


/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement a sorting algorithm
    
    for (int i = 0; i < (n - 1); i++)
    {
        int min = i;
        int j = 0;
        
        for (j = i; j < n; j++)
        {
            if (values[j] < values[min])
                min = j;
        }
        
        
        if (min != i)
        {
            int temp = values[min];
            values[min] = values[i];
            values[i] = temp;
        }
    }
}
