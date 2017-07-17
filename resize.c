/**
 * resizes bitmaps
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./copy infile outfile\n");
        return 1;
    }

    // remember filenames
    int n = atoi(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // keep old padding and width and height
    int p_old = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int width_old = bi.biWidth;
    int height_old = bi.biHeight;

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // update outfile's BITMAPINFOHEADER
    bi.biWidth *= n;
    bi.biHeight *= n;
    
     // determine padding for scanlines
    int p_new =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    bi.biSizeImage = (bi.biWidth * sizeof(RGBTRIPLE) + p_new) * abs(bi.biHeight);
    
     // update outfile's BITMAPFILEHEADER
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    
 

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

   
    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(height_old); i < biHeight; i++)
    {
        RGBTRIPLE tarray[bi.biWidth];
        int c = 0;
    // iterate over pixels in scanline
        for (int j = 0; j < width_old; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            
            
            //resize horizontally into array
            for (int k = 0; k < n; k++)
            {
                tarray[c] = triple;
                c++;
            
            }
        }
      
        for (int a = 0; a < n; a++)
        {
        // write RGB triple to outfile
            for (int b = 0; b < bi.biWidth; b++ )
            {
                 fwrite(&tarray[b], sizeof(RGBTRIPLE), 1, outptr);     
            }
        
            for (int k = 0; k < p_new; k++)
            {
                fputc(0x00, outptr);
            }
            
        }
        // skip over padding, if any
        fseek(inptr, p_old, SEEK_CUR);

        // then add it back (to demonstrate how)
        
      
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
