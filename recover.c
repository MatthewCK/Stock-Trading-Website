#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

typedef uint8_t BYTE;

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover infile.raw\n");
        return 1;
    }
    
    //open file raw file to input and check for success
    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s. \n", argv[1]);
        return 2;
    }
    
    
    BYTE buffer[512];
    int found_jpg = 0;
    int file_number = 0;
    char file_name[8];
    //int count = 0;
    FILE* outptr;
    
    //read from infile and check if EOF
    while (fread(buffer, 1, 512, inptr) == 512)
    {
        //check if beginning of jpg
        // ///////////////////////
        // @ben note
        // I think you have a problem with this if statement, in the problem statement it says to check 
        // 0xff 0xd8 0xff are the first 3 bytes, you are doing that fine, but then it also says 
        // " Put another way, the fourth byte’s first four bits are 1110" and you don't have a check on the 4th byte
        // there is an easy way to check for what the value of the first 4 bytes are.  
        // 
        // Hint1: You are going to need the & operator (different than &&)
        // Hint2: 0xe0 in binary is "11100000"
        // ////////////////////////
        
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xe0) == 0xe0)
        {
               // found first jpg?
               if (found_jpg == 0)
               {
                   sprintf(file_name,"%03d.jpg", file_number);
                   outptr = fopen(file_name, "w");
                   found_jpg = 1;
                  
                    // write block to outfile
                    fwrite(buffer, 1, 512, outptr);
                    
                    
                    
               }
               else
               {   
                   //new jpg so close last one
                   fclose(outptr);
                   
                   //increment file number
                   file_number += 1;
                   sprintf(file_name,"%03d.jpg", file_number);
                   
                   //open new jpg file for write
                   outptr = fopen(file_name, "w");
                   
                    // write block to outfile
                    fwrite(buffer, 1, 512, outptr);
               }
        }
        else
        {
            if (found_jpg == 0)
            {
                //no jpg yet so don't write anything
                continue;
            }
            else
            {
                //middle of a jpg, write the next block
                fwrite(buffer, 1, 512, outptr);
            }
    
        }
        
    }
    
    //printf("%i", count);
    fclose(inptr);
    fclose(outptr);
    
    return 0;
}
    
