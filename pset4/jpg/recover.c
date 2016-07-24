/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 * 
 * The first three bytes from JPEGs are: 0xff 0xd8 0xff. 
 * The fourth byte, meanwhile, is either 0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 
 * 0xe6, 0xe7, 0xe8, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, of 0xef. Note 
 * that the fourth byte's first 4 bits are 1110.
 * 
 * A photo that’s 1 MB (i.e., 1,048,576 B) thus takes up 
 * 1048576 ÷ 512 = 2048 "blocks" on a CF card. But so does a photo that’s, 
 * say, one byte smaller (i.e., 1,048,575 B)! The wasted space is called 
 * "slack space." 
 * 
 * Each time you find a signature, you can open a new file for writing and 
 * start filling that file with bytes from my CF card, closing that file only 
 * once you encounter another signature. Moreover, rather than read my 
 * CF card’s bytes one at a time, you can read 512 of them at a time into a 
 * buffer for efficiency’s sake. Thanks to FAT, you can trust that JPEGs' 
 * signatures will be "block-aligned." That is, you need only look for those 
 * signatures in a block’s first four bytes.
 * 
 * The last byte of a JPEG might not fall at the very end of a block.
 * 
 * Trailing 0s of the corrupt JPEG should appear in the refurbished one. 
 * 
 * check50 2015.fall.pset4.recover recover.c
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
//#include <inttypes.h>

const char JPEGBytes[] = {0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 
                          0xe5, 0xe6, 0xe7, 0xe8, 
                          0xe8, 0xe9, 0xea, 0xeb, 
                          0xec, 0xed, 0xee, 0xef};

typedef struct
{
  uint8_t first;
  uint8_t second;
  uint8_t third;
}__attribute__((__packed__))
JPEGIdentifier;

int getFileSize(FILE * file)
{
  int fileSize = 0;
  fseek(file, 0 , SEEK_END);
  fileSize = ftell(file);
  rewind(file);
  return fileSize;
}

int main(void)
{
    FILE * infile = fopen("card.raw", "r");
    if(infile == NULL)
    {
      printf("File could not be opened\n");
      return 2; // inability to compare
    }
    
    size_t infileSize = getFileSize(infile);
    
    unsigned int blockCounter = 0;
    unsigned int JPEGFileCounter = 0;  // Used for naming 
    FILE * outfile = NULL;
    char JPEGName[7];        // stores JPG file names
    
    while(blockCounter < infileSize / 512)
    {
      uint8_t JPEG[512]; // store image bytes
      fread(&JPEG, 1, 512, infile);
      
      if (JPEG[0] == 0xff && JPEG[1] == 0xd8 && JPEG[2] == 0xff) // new image
      {
        if(outfile != NULL)  // Done here to avoid previous image backtrace error
          fclose(outfile);
        
        sprintf(JPEGName,"%03d.jpg", JPEGFileCounter); // creating file name
        outfile = fopen(JPEGName, "w");
        fwrite(&JPEG, 1, 512, outfile); 
        ++JPEGFileCounter;
      }
      else if (outfile != NULL)
      {
          fwrite(&JPEG, 1, 512, outfile);
      } 
      
      ++blockCounter;
    }
    
    fclose(infile);
    return 0;
}