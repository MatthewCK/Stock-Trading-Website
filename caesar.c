#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>

int main (int argc, string argv[])
{
    // only accepts two command line arguments
    if (argc != 2)
    {
        printf("Error: This program only accepts one command line argument.\n");
        return 1;
    }
    else
    {
        int k = atoi(argv[1]) % 26;
        
        printf("plaintext: ");
        string s = get_string();
        printf("ciphertext: ");
        
        // uses key to change plaintext to ciphertext
        for (int i = 0, n = strlen(s); i < n; i++ )
        {
            int c = s[i];
            for (int j = 0; j < k; j++)
            {
                if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
                {
                   if (c == 90 || c == 122)
                    c = c - 26;
                    c++; 
                }
                else 
                    break;
            }
            printf("%c", c);
        }
        
        printf("\n");
        return 0;
    }
}