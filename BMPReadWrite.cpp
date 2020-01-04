// FileReadTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include "BMPReadWrite.h"

void printPixelData(BMPImageData bmpimage) {

    int width = bmpimage.info->image_width;
    int height = bmpimage.info->image_height - 1;

    for (int i = height; i >= 0; i--)
    {

        //Every ROW
        for (int j = 0; j < width; j++)
        {

            std::cout << "( XY[" << j << ":" << i << "] ";

            std::cout << "[R:" << (short)bmpimage.pixel_data[i][j].r << "|" << "G:" << (short)bmpimage.pixel_data[i][j].g << "|" << "B:" << (short)bmpimage.pixel_data[i][j].b << "] )\t";


        }

        std::cout << std::endl;
    }

}

void freeBMPData(BMPImageData bmpfile) {

    //Free all pixels

    for (size_t i = 0; i < bmpfile.info->image_height; i++)
    {
        delete[] bmpfile.pixel_data[i];
    }

    delete[] bmpfile.pixel_data;
    free(bmpfile.header);
    free(bmpfile.info);

    bmpfile.header = NULL;
    bmpfile.info = NULL;
    bmpfile.pixel_data = NULL;

}

void writeBMPFile(const char* filename, BMPImageData* buffer) {

    std::ofstream outf;

    outf.open(filename, std::ios::binary);

    //Check for error
    if (outf.fail())
    {
        std::cerr << "Error writing file" << std::endl;
        exit(1);
    }
    
    //Write header
    outf.write((char*)&buffer->header->signature,2);
    outf.write((char*)&buffer->header->filesize, 4);
    outf.write((char*)&buffer->header->res1, 2);
    outf.write((char*)&buffer->header->res2, 2);
    outf.write((char*)&buffer->header->pixel_offset, 4);

    //Write info
    outf.write((char*)&buffer->info->info_size, 4);
    outf.write((char*)&buffer->info->image_width, 4);
    outf.write((char*)&buffer->info->image_height, 4);
    outf.write((char*)&buffer->info->planes, 2);
    outf.write((char*)&buffer->info->bits_per_pixel, 2);
    outf.write((char*)&buffer->info->compression, 4);
    outf.write((char*)&buffer->info->image_size, 4);
    outf.write((char*)&buffer->info->x_pix_per_m, 4);
    outf.write((char*)&buffer->info->y_pix_per_m, 4);
    outf.write((char*)&buffer->info->colors_in_ct, 4);
    outf.write((char*)&buffer->info->important_cc, 4);
   

    //Write pixel data
    int padding = (4 - (buffer->info->image_width * 3) % 4) % 4;
    
    for (int i = 0; i < buffer->info->image_height; i++)
    {
        for (int j = 0; j < buffer->info->image_width; j++)
        {
            outf.write((char*)&buffer->pixel_data[i][j].b, 1);
            outf.write((char*)&buffer->pixel_data[i][j].g, 1);
            outf.write((char*)&buffer->pixel_data[i][j].r, 1);
        }

        outf.write((char*)"\0", padding);
    }
    outf.write((char*)"\0", padding);

    //EOF
    outf.write((char*)"\0", 1);
    outf.close();
}

void readBMPFile(const char* filename, BMPImageData *buffer) {

    std::ifstream f;
   
    f.open(filename, std::ios::binary | std::ios::in);

    //Check for error
    if (f.fail())
    {
        std::cerr << "Error opening file" << std::endl;
        exit(1);
    }

    //Get length of file
    f.seekg(0, std::ios::end);
    std::streampos length = f.tellg();
    f.seekg(0, std::ios::beg);

    buffer->header = (bmpheader*)malloc(sizeof(bmpheader));
    buffer->info = (bmpinfo*)malloc(sizeof(bmpinfo));
    

    f.seekg(0, std::ios::beg);
    
    //Read header part
    f.read((char*)&buffer->header->signature,2);
    f.read((char*)&buffer->header->filesize, 4);
    f.read((char*)&buffer->header->res1, 2);
    f.read((char*)&buffer->header->res2, 2);
    f.read((char*)&buffer->header->pixel_offset, 4);

    //Read info part
    f.read((char*)&buffer->info[0], sizeof(bmpinfo));
 
    f.seekg(buffer->header->pixel_offset,std::ios::beg);

    //Read pixel data
    char* pixels = new char[buffer->header->filesize - buffer->header->pixel_offset];
    f.read(&pixels[0], buffer->header->filesize - buffer->header->pixel_offset);

    //Create the pixel matrix of RGB values
    buffer->pixel_data = new pixel_color*[buffer->info->image_height];

    for (size_t i = 0; i < buffer->info->image_height; i++)
    {
       
        buffer->pixel_data[i] = new pixel_color [buffer->info->image_width];
    }

    //Every row in a BMP is aligned to 4 bytes. It is padded out if not even to 4;
    int padding = (4 - (buffer->info->image_width * 3) % 4) % 4;

    //Pixel data starts at 54 bytes offset(should be found in header);
    int buffer_pos = 0;

    //Cols
    for (int i = 0; i < buffer->info->image_height; i++) {

        //Rows
        for (size_t j = 0; j < buffer->info->image_width; j++)
        {

            //Flip BGR to RGB
            buffer->pixel_data[i][j].r = (unsigned char)pixels[buffer_pos + 2];    
            buffer->pixel_data[i][j].g = (unsigned char)pixels[buffer_pos + 1];
            buffer->pixel_data[i][j].b = (unsigned char)pixels[buffer_pos];

            buffer_pos += 3;
        }

        buffer_pos += padding;

    }

    delete[] pixels;
}

