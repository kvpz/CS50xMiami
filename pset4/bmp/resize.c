/**
 * copy.c (previous existing file used to write this program)
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Copies a BMP piece by piece, just because.
 * -------------------------------------------------------------
 * Kevin Perez
 * 7/22/16
 * resize.c
 * 
 * Implementation of this file began with an exact copy of copy.c.
 * 
 * The goal of this program is to resize a 24-bit uncompressed BMP by a 
 * factor of n (i.e. multiplying the width & height by n). It can be assumed
 * that the size of "infile" will not exceed (2^32 - 1). Assume n >= (int)1.
 * 
 * Process of implementation:
 * - Decide which of the fields in BITMAPFILEHEADER & BITMAPINFOHEADER might
 *  need to be modified.
 * - Consider if padding will need to be added or subtracted to scanlines.
 *  (note: fread does not read padding bytes)
 * 
 * BITMAPFILEHEADER::bfSize needs to be increased. The amount of bytes taken by
 * a BMP file is 14 + 40 + (width_pixels * height_pixel) * 3
 * 
 * Usage: ./resize n infile outfile
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int getPadding(int biWidth, int RGBTripleSize)
{
    return (4 - (biWidth * RGBTripleSize % 4)) % 4;
}

void changeBMPMetaData( BITMAPFILEHEADER* bf, BITMAPINFOHEADER* bi, 
                        int* newPadding, const int resize_value)
{
    // Resize image if n != 1
    if(resize_value != 1)
    {
        bi->biWidth *= resize_value;
        bi->biHeight *= resize_value;
        // determine padding for scanlines
        *newPadding = getPadding(bi->biWidth, sizeof(RGBTRIPLE));
        int total_padding_bytes = *newPadding * abs(bi->biHeight);
        int new_resolution = abs(bi->biWidth * bi->biHeight);
        bf->bfSize = new_resolution * 3 + 54 + total_padding_bytes; // include padding
        bi->biSizeImage = new_resolution * 3 + total_padding_bytes;
    }
}

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize n infile outfile\n");
        return 1;
    }
    
    int resize_value = atoi(argv[1]);
    if(resize_value < 1 || resize_value > 100)
    {
        if (resize_value < 0)
            printf("n, the resize factor, must be an integer.\n");
        else
            printf("n, the resize factor, must satisfy 0 < n <= 100.\n");
        return EXIT_FAILURE;
    }
    
    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];

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

    //int original_biHeight = bi.biHeight;
    int original_biWidth = bi.biWidth;
    int originalPadding = getPadding(bi.biWidth, sizeof(RGBTRIPLE));
    int newPadding = 0;
    changeBMPMetaData( &bf, &bi , &newPadding, resize_value);
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    fpos_t previousLine; // Used to print a scanline again to resize height
    fgetpos(inptr, &previousLine);
    int lineHeight = 0;
    
    // iterate over infile's scanlines
    int biHeight = abs(bi.biHeight);
    for (int i = 0; i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < original_biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;
            
            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // write RGB triple to outfile
            for(int itr = 0; itr < resize_value; ++itr)
            {
                
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                printf("0x%x%x%x ", triple.rgbtBlue, triple.rgbtGreen, triple.rgbtRed);   
            }
        }

        
        // skip over padding, if any
        fseek(inptr, originalPadding, SEEK_CUR);
        
        // then add it back (to demonstrate how)
        for (int k = 0; k < newPadding; k++)
        {
            fputc(0x00, outptr);
        }
                
        ++lineHeight;
        if(lineHeight < resize_value)
            fsetpos(inptr, &previousLine);
        else
        {
            //fgetpos(inptr, &previousLine);
            lineHeight = 0;
        }
        printf("\n");
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
