
#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include "decode.h"
#include "common.h"
#include "types.h"

/*Function definition*/
Status do_decoding(DecodeInfo *decInfo)
{
    if(open_image_file(decInfo) == e_success)
    {
        printf("INFO: File opened successfully\n");
        if(skip_bmp_file(decInfo) == e_success)
        {
            printf("INFO: bmp file skipped successfully\n");
            if(decode_magic_string(decInfo) == e_success)
            {
                printf("INFO: Magic string decoded successfully\n");
                if(decode_secret_file_extn_size(decInfo) == e_success)
                {
                    printf("INFO: Secret file extension size decoded successfully\n");
                    if(decode_secret_file_extn(decInfo) == e_success)
                    {
                        printf("INFO: Secret file extension decoded successfully\n");
                        if(decode_secret_file_size(decInfo) == e_success)
                        {
                            //printf("Decoded secret file size: %d\n", decInfo->size_secret_file);

                            printf("INFO: Secret file size decoded successfully\n");
                            if(decode_secret_file_data(decInfo) == e_success)
                            {
                                printf("INFO: Secret file data decoded successfully\n");
                            }
                            else
                            {
                                printf("ERROR: Secret file data not decoded\n");
                                return e_failure;
                            }
                        }
                        else
                        {
                            printf("ERROR: Secret file size not decoded\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("ERROR: Secret file extension not decoded\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("ERROR: Secret file extension size not decoded\n");
                    return e_failure;
                }
            }
            else
            {
                printf("ERROR: Magic string not decoded\n");
                return e_failure;
            }
        }
        else
        {
            printf("ERROR: BMP file not skipped\n");
            return e_failure;
        }
    }
    else
    {
        printf("ERROR: File not opened\n");
        return e_failure;
    }
    return e_success;
}

/* Get File pointers for i/p and o/p files */
Status open_image_file(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");
    if(decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        return e_failure;
    }

    decInfo->fptr_output = fopen(decInfo->output_fname, "wb");
    if(decInfo->fptr_output == NULL)
    {
        perror("fopen");
        return e_failure;
    }

    return e_success;
}



/*Skip the bmp file*/
Status skip_bmp_file(DecodeInfo *decInfo)
{
    fseek(decInfo -> fptr_stego_image, 54, SEEK_SET);
    return e_success;
}

/*decode magic string*/
Status decode_magic_string(DecodeInfo *decInfo)
{
    int mlen = strlen(MAGIC_STRING);
    char Magic_string[mlen + 1];
    char image_buffer[8];

    for (int i = 0; i < mlen; ++i) {
        if (fread(image_buffer, 8, 1, decInfo->fptr_stego_image) != 1)
            return e_failure;
        Magic_string[i] = decode_byte_from_lsb(image_buffer);
    }
    Magic_string[mlen] = '\0';

   /*printf("Extracted magic: %s\n", Magic_string);
    printf("Expected magic: %s\n", MAGIC_STRING); */

    if (strcmp(MAGIC_STRING, Magic_string) == 0)
    {
       // printf("-----✅ Magic string matched✅-----\n");
        return e_success;
    }
    else
    {
        printf("ERROR: Magic string not match❌.\n");
        return e_failure;
    }
}

/* decode a byte into LSB of image data array */
char decode_byte_from_lsb(char *image_buffer)
{
    char ch = 0;
    for(int i=0; i<8; i++)
    {
        /*Extract the lsb from the each byte in the buffer*/
        char lsb = image_buffer[i] & 1;
        /*shift the decoded byte and add it in the lsb*/
        ch = ch | (lsb << i);
    }
    //printf("%c",ch);
    return ch;
}
/*decode secret file extension size*/
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    char image_buffer[32];
    fread(image_buffer, 32, 1, decInfo -> fptr_stego_image);
    decInfo -> extn_size = decode_int_from_lsb(image_buffer);
    return e_success;
}
/*decode int value from the LSB*/
int decode_int_from_lsb(char *image_buffer)
{
    int ch = 0;
    for(int i=0; i<32; i++)
    {
        /*Extract the lsb from the each byte in the buffer*/
        char lsb = image_buffer[i] & 1;
        /*shift the decoded byte and add it in the lsb*/
        ch = ch | (lsb << i);
    }
    //printf("%c",ch);
    return ch;
}
/*decode secret file extn*/
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char image_buffer[8];
    int extn_size = decInfo->extn_size;
    char extn[extn_size + 1];

    for(int i=0; i<extn_size; i++)
    {
        fread(image_buffer, 8, 1, decInfo -> fptr_stego_image);
        extn[i] = decode_byte_from_lsb(image_buffer);
    }
    extn[extn_size] = '\0';

    decInfo->output_fname = malloc(50);
    decInfo->output_fname[0] = '\0'; 
    strcat(decInfo->output_fname, "secret");
    strcat(decInfo->output_fname, extn);

    //printf("Output file name: %s\n", decInfo->output_fname);

    return e_success;

}
/*decode secret file size*/
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char image_buffer[32];
    fread(image_buffer, 32, 1, decInfo -> fptr_stego_image);
    decInfo -> size_secret_file = decode_int_from_lsb(image_buffer);
    
    return e_success;

}
/*decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char image_buffer[8];
    char ch;
    for(int i=0; i<decInfo->size_secret_file; i++)
    {
        fread(image_buffer, 8, 1, decInfo->fptr_stego_image);
        ch = decode_byte_from_lsb(image_buffer);
        fwrite(&ch, 1, 1, decInfo->fptr_output);
    }
    return e_success;
}

