#pragma once

typedef struct t_bmpheader {

    unsigned short signature;
    unsigned int filesize;
    unsigned short res1;
    unsigned short res2;
    unsigned int pixel_offset;

} bmpheader;


typedef struct t_bmpinfo {

    unsigned int info_size;
    unsigned int image_width;
    unsigned int image_height;
    unsigned short planes;
    unsigned short bits_per_pixel;
    unsigned int compression;
    unsigned int image_size;
    unsigned int x_pix_per_m;
    unsigned int y_pix_per_m;
    unsigned int colors_in_ct;
    unsigned int important_cc;
    
    /*
    int r_bitmask;
    int g_bitmask;
    int b_bitmask;
    int a_bitmask;
    int color_space_type;
    int color_space_endpoints;
    int r_gamma;
    int g_gamma;
    int b_gamma;
    int intent;
    int icc_profile_data;
    int icc_profile_size;
    int reserved;
    */

} bmpinfo;


typedef struct t_pixel {

    unsigned char r;
    unsigned char g;
    unsigned char b;

} pixel_color;

typedef struct t_bmp_image {

    bmpheader* header;
    bmpinfo* info;
    pixel_color** pixel_data;

}BMPImageData;

        
void readBMPFile(const char* filename, BMPImageData* buffer);
void writeBMPFile(const char* filename, BMPImageData* buffer);
void freeBMPData(BMPImageData imagedata);