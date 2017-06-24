#include <cs50.h>
#include <stdio.h>
#include <math.h>

int digits[16];
int arr1[16];
int arr2[16];
int x = 0;

int main(void)
{
    printf("CC Number: ");
    long n = get_long_long();
    long m = n;
//separates cc to digits
    for(int i = 0; i < 16; i++)
        {
            digits[i] = n % 10;
            n = n / 10;
        
        }

//separates digits to just every other 1    
    for(int i = 1; i < 16; i = i + 2)
    {
        arr1[x] = digits[i];
        x++;
        
    }
    
    x = 0;
    
//separates digits to just every other 2    
    for(int i = 0; i < 16; i = i + 2)
    {
        arr2[x] = digits[i];
        x++;
        
    }
    
//multiplies each in arr1 by 2     
    for(int i = 0; i < 16; i++)
    {
        arr1[i] = arr1[i] * 2;
        
    }
    
    int sum = 0;
    
// sums products' digits    
    for(int i = 0; i < 16; i++)
    {
        sum = sum + (arr1[i] % 10);
        arr1[i] = arr1[i] / 10;
        sum = sum + (arr1[i] % 10);
        
    }
    
// sums with arr2    
    for(int i = 0; i < 16; i++)
    {
        sum = sum + arr2[i];
    }

int count = 0;
n=m;
while (n != 0)
{
    n /= 10;
    count++;
}

n=m;

if((sum % 10) == 0 )
{
    //printf("%i", count)
    if (count == 15)
    {
        printf("AMEX\n");
    }
    
    else if (count == 13)
    {
        printf("VISA\n");
    }
    
    else if (count == 16)
    {
        long e = pow(10,15);
        
       // printf("%ld", e);

        if (((n / e) % 10) == 4)
        {
            printf("VISA\n");
        }
        
        e = pow(10,14);

        
        if ((n / e) == 51 || (n / e) == 52 || (n / e) == 53 || (n / e) == 54 || (n / e) == 55)
        {
            printf("MASTERCARD\n");
        }
    }
    
    else
    {
        printf("INVALID\n");
    }
    
    
}
//printf("%i", sum);
if (sum % 10 != 0)
{
    printf("INVALID\n");
}

return 0;

}
