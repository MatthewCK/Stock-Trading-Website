#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int b, s, h;
    while(true)
    {
    printf("Height: ");
    b = get_int();
    if (b > -1 && b < 24)
        break;
    }
    
    
    for(int i = 0; i < b; i = i + 1)
    {
        s = b - 1 - i;
        h = b - s;
        
        
        for(int x = 0; x < s; x = x + 1)
            printf(" ");
        for(int x = 0; x < h; x = x + 1)
            printf("#");
        
        printf("  ");
        
        for(int x = 0; x < h; x = x + 1)
            printf("#");
            
        printf("\n");
        
        
    }
}