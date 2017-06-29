#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    string s = get_string();
    
    printf("%c", toupper(s[0]));
    
    
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        
        if (s[i] == ' ')
        {
            printf("%c", toupper(s[i+1]));
        }
    }
    
    printf("\n");
    
  //  for (int i = 0; )
}