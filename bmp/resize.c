/**
 * resize.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Copies a BMP piece by piece, just because.
 */
 
 
 /*Implement a program that enlarges 
 24-bit uncompressed BMPs by a factor of n.*/
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember filenames
    int n = atoi(argv[1]);
    char* infile = argv[2];
    char* outfile = argv[3];
    
    
    if (n < 0 || n > 100)
    {
        printf("Please enter a number between 1 and 100\n");
    }
    

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
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
  
    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    //Define the width and height of the image.
    int old_width = bi.biWidth;
    int old_height = bi.biHeight;
    
    //Update the image width and height by a factor of n. 
    bi.biWidth = bi.biWidth * n;
    bi.biHeight = bi.biHeight * n;

     // determine padding for scanlines
    int old_padding =  (4 - (old_width * sizeof(RGBTRIPLE)) % 4) % 4;
    int new_padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
   
    // calculation of biSizeImage and bfSize in the new image file.
    bi.biSizeImage = abs(bi.biHeight) * ((bi.biWidth * sizeof (RGBTRIPLE)) + new_padding);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); 
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    
    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    // iterate over the rows of the image.
    for (int row = 0, biHeight = abs(old_height); row < biHeight; row++)
    {   
        //iterate over n pixels to add n rows
        for (int pixel = 0; pixel < n; pixel++)
        {
            //Go back to the start of the row and repeat the copy process for n times. 
             fseek(inptr, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 
            (row * (old_width * sizeof(RGBTRIPLE) + old_padding)), SEEK_SET);
            
            // iterate over the columns of the image.
            for (int col = 0; col < old_width; col++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                
                //iterate over n pixels to add n columns
                for (int px = 0; px < n; px++)
                { 
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE) , 1, outptr);
                }
                
            }
            
            // skip over padding, if any.
            // SEEK_CUR: the current position of the file pointer
            fseek(inptr, old_padding, SEEK_CUR);
      
            //add the new padding to each scanline
            for (int k = 0; k < new_padding; k++)
            {    
            
                fputc(0x00, outptr);
            }
            
        }
        
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
