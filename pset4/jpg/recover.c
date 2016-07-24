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

int countZeroBytes(FILE* file)
{
  int zeroBytes = 0;
  while(fgetc(file) == 0)
    ++zeroBytes;
  return zeroBytes;
}

int countNonZeroBytes(FILE* file)
{
  int amountOfBytes = 0;
  while(fgetc(file) != 0)
    ++amountOfBytes;

  return amountOfBytes;
}

int findNextJPEG(FILE* file)
{
  int c;
  do
  {
    c = fgetc(file);
  } while(c != 0xff);
  ungetc(c, file);
  return 1;
}

char byteCheck(char byte)
{
  int arrSize = sizeof(JPEGBytes);
  for(int i = 0; i < arrSize; ++i)
  {
    if(byte == JPEGBytes[i])
      return byte;
  }
  return 0;
}
/*
void createJPEG(FILE* infile, char* fileName, uint8_t fourthMETAByte)
{
  FILE * outfile = fopen(fileName, "w");
  uint8_t meta[] = { 0xff, 0xd8, 0xff, 0xe0 };
  fwrite(meta, 1, 4, outfile);
  printf("Sizeof(fgetc): %d\n", fgetc(infile));
  uint8_t c = fgetc(infile);
  uint8_t previous;
  //JPEGIdentifier id;
  //fread(&id, sizeof(JPEGIdentifier), 1, outfile);
  while(c != 0xd9 && previous != 0xff)
  {
    //fwrite(&id, sizeof(JPEGIdentifier), 1, outfile);
    fputc(c, outfile);
    previous = c;
    //fwrite(c, 1, 1, outfile);
    c = fgetc(infile);
    printf("0x%x\n", c);
  }
  fputc(0xd9, outfile);
  printf("DONE PRINTING!\n");
  fclose(outfile);
}
*/
int main(void)
{
    FILE * infile = fopen("card.raw", "r");
    if(infile == NULL)
    {
      printf("File could not be opened\n");
      return 2; // inability to compare
    }
    
    size_t infileSize = getFileSize(infile);
    
    unsigned int readCounter = 0;
    unsigned int JPEGFileCounter = 0;
    FILE * outfile = NULL;
    char JPEGName[7];
    
    while(readCounter < infileSize/512)
    {
      uint8_t JPEG[512];
      fread(&JPEG, 1, 512, infile);
      if(JPEG[0] == 0xff && JPEG[1] == 0xd8 && JPEG[2] == 0xff)
      {
        sprintf(JPEGName,"%03d.jpg", JPEGFileCounter);
        outfile = fopen(JPEGName, "w");
        fwrite(&JPEG, 1, 512, outfile);
        ++JPEGFileCounter;
      }
      else if(outfile != NULL)
      {
        int open = 1;
        size_t bytesToWrite = 512;
        for(int i = 1; i < 512; ++i)
        {
          if(JPEG[i-1] == 0xff && JPEG[i] == 0xd9)
          {
            bytesToWrite = i+1+1; 
            fwrite(&JPEG, 1, bytesToWrite, outfile);
            fclose(outfile);
            open = 0;
            i = 512;
          }
        }
        if(open)
          fwrite(&JPEG, 1, bytesToWrite, outfile);
      } // else-if
      ++readCounter;
    }
    
    fclose(infile);
    return 0;
}
