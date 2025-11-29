#ifndef DECODE_H
#define DECOD_H


#include<stdio.h>
#include "types.h" // Contains user defined types

typedef struct _DecodeInfo
{
    /* stego input info*/
    char *stego_image_fname;
    FILE *fptr_stego_image;


    char *output_fname;
    FILE *fptr_output;
    uint extn_size;
    uint size_secret_file;

} DecodeInfo;

/* Decoding function prototypes*/

/*Read and validate decode ags from argv*/
Status read_and_validate_decode_args(int argc, char *argv[], DecodeInfo *decInfo);

/*Perform the decoding*/
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_image_file(DecodeInfo *decInfo);

/*Skip the bmp file*/
Status skip_bmp_file(DecodeInfo *decInfo);

/*decode magic string*/
Status decode_magic_string(DecodeInfo *decInfo);

/*decode secret file extension size*/
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/*decode secret file extn*/
Status decode_secret_file_extn(DecodeInfo *decInfo);

/*decode secret file size*/
Status decode_secret_file_size(DecodeInfo *decInfo);

/*decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* decode a byte into LSB of image data array */
char decode_byte_from_lsb(char *image_buffer);

/*decode int value from the LSB*/
int decode_int_from_lsb(char *image_buffer);


#endif