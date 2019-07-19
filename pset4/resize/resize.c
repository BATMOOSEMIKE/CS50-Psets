/**
 * Copies a BMP piece by piece, just because.
 */

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize factor infile outfile\n");
        return 1;
    }

    // remember filenames
    int factor = atoi(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    if(factor<1 || factor>100){
      printf("The factor must be in the range from 1 to 100\n");
      return 1;
    }

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
    BITMAPFILEHEADER newbf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    BITMAPINFOHEADER newbi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    newbf = bf;
    newbi = bi;

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    newbi.biWidth = bi.biWidth*factor;
    newbi.biHeight = bi.biHeight*factor;
    
    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int newpadding = (4 - (newbi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    newbi.biSizeImage = newbi.biWidth * abs(newbi.biHeight)*sizeof(RGBTRIPLE)+newpadding *abs(newbi.biHeight);
    newbf.bfSize = newbf.bfOffBits + newbi.biSizeImage;

    // write outfile's BITMAPFILEHEADER
    fwrite(&newbf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&newbi, sizeof(BITMAPINFOHEADER), 1, outptr);

    

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
      int rowcounter = 0;

      while(rowcounter < factor){

        //iterate over all the pixels in each line
        for(int j = 0; j<bi.biWidth;j++){

          RGBTRIPLE triple;

          int pixelcount = 0;

          fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

          while(pixelcount<factor){
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            pixelcount++;
          }

        }

        //add new padding
        for(int a = 0; a<newpadding;a++){
          fputc(0x00, outptr);
        }
        
        long offset = (bi.biWidth * sizeof(RGBTRIPLE));

        //move back to the beginning of the scanline
        if(rowcounter<(factor-1)){
          fseek(inptr, -offset, SEEK_CUR);
        }

        rowcounter++;

      }



        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);
        

        
/*
        // then add it back (to demonstrate how)
        for (int k = 0; k < newpadding; k++)
        {
            fputc(0x00, outptr);
        }
*/

        


    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
