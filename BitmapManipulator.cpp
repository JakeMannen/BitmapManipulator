
#include <iostream>
#include "BMPReadWrite.h"

int main()
{

    BMPImageData bmp;

    //Read file to memory
    readBMPFile("4kimage.bmp", &bmp);

    
    //printPixelData(bmp);


    //Simple manipulation of pixel colors

    //Cols in pixel matrix
    for (int i = 0; i < bmp.info->image_height; i++)
    {
        //Rows in pixel matrix
        for (int j = 0; j < bmp.info->image_width; j++)
        {

            short r = bmp.pixel_data[i][j].r;
            short g = bmp.pixel_data[i][j].g;
            short b = bmp.pixel_data[i][j].b;

            if (r > 200) {
                r = 100;
            }
            if (g > 200) {
                g = 120;
            }
            if (b < 100) {
                b = 110;
            }

            bmp.pixel_data[i][j].r = r;
            bmp.pixel_data[i][j].g = g;
            bmp.pixel_data[i][j].b = b;

        }
    }

    //Write back to file
    writeBMPFile("4kimage_mod.bmp", &bmp);

    //Release memory
    freeBMPData(bmp);
}