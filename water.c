#include <cs50.h>
#include <stdio.h>


int main(void)
{
    
    printf("Minutes: ");
    int m = get_int();
    printf("Bottles: %i \n", m * 12);
    
}