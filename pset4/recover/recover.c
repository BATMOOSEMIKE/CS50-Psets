/**
 * Copies a BMP piece by piece, just because.
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

    int jpgcount = 0;
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover infile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[1];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    unsigned char buffer[512];

    char filename[10];
    FILE *img;

    while(fread(&buffer, 512, 1, inptr)){

      if(buffer[0]==0xff && buffer[1]==0xd8 && buffer[2] == 0xff && (buffer[3]&0xf0)==0xe0){
        if(jpgcount > 0){
          fclose(img);
        }

        sprintf(filename, "%03i.jpg", jpgcount);
        img = fopen(filename, "w");

        jpgcount++;

      }

      if(jpgcount>0){
        fwrite(&buffer, 512, 1, img);
      }

    }


    // close infile
    fclose(inptr);
    fclose(img);

    // success
    return 0;
}
