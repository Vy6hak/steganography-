#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include "encode.h"
#include "common.h"
#include "types.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    //printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    //printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s❌\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s❌\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s❌\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}
Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo) == e_success)
    {
        printf("INFO: Files opened successfully\n");

            // FIX: set file size now
        encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
        if(check_capacity(encInfo)== e_success)
        {
            printf("INFO: Capacity checked passed\n");
            // Reset pointer after capacity check (important)
            fseek(encInfo->fptr_src_image, 0, SEEK_SET);
            if(copy_bmp_header(encInfo->fptr_src_image, encInfo ->fptr_stego_image) == e_success)
            {
                printf("INFO: Header copied successfully\n");
                if(encode_magic_string(MAGIC_STRING,encInfo) == e_success)
                {
                    printf("INFO: Magic string encoded\n");
                    if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo) == e_success)
                    {
                        printf("INFO: Encoded secret file extension size\n");
                        if(encode_secret_file_extn(encInfo -> extn_secret_file, encInfo) == e_success)
                        {
                            printf("INFO: Encoded secret file extension\n");
                            if(encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_success)
                            {
                                printf("INFO: Encoded secret file size\n");
                                if(encode_secret_file_data(encInfo) == e_success)
                                {
                                    printf("INFO: Encoded secret file data\n");
                                    if(copy_remaining_img_data(encInfo -> fptr_src_image, encInfo -> fptr_stego_image)==e_success)
                                    {
                                        printf("INFO: Reamaining image data copied\n");
                                        fclose(encInfo -> fptr_src_image);
                                        fclose(encInfo -> fptr_stego_image);
                                        return e_success;
                                    }
                                    else
                                    {
                                        printf("ERROR: Copy remianing data failed\n");
                                        return e_failure;
                                    }
                                }
                                else
                                {
                                    printf("ERROR: Encode secret file data failed\n");
                                    return e_failure;
                                }
                            }
                            else
                            {
                                printf("ERROR: Encode secret file size failed\n");
                                return e_failure;
                            }
                        }
                        else
                        {
                            printf("ERROR: Encode secret file extension failed\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("ERROR: Encode secret file extension failed\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("ERROR: Encode magic string failed\n");
                    return e_failure;
                }
            }
            else
            {
                printf("ERROR: Copy bmp header failed\n");
                return e_failure;
            }
        }
        else
        {
            printf("ERROR: Check capacity failed\n");
            return e_failure;
        }
    }
    else
    {
        printf("ERROR: Opening file failed\n");
        return e_failure;
    }
}
Status check_capacity(EncodeInfo *encInfo)
{
    //get image size
    uint image_capacity = get_image_size_for_bmp(encInfo -> fptr_src_image);

    //get secret file size
    uint secret_file_size = get_file_size(encInfo ->fptr_secret);

    //calculate total data to hide
    uint magic_string_size = strlen(MAGIC_STRING);
    uint extension_size = strlen(encInfo -> extn_secret_file);
    uint secrete_data = get_file_size(encInfo -> fptr_secret);
    uint encoding_things = 8 * (magic_string_size + extension_size + sizeof(uint) + secrete_data);


    // Debug prints
   /* printf("INFO: Image size = %u\n", image_capacity);
    printf("INFO: Secret file size = %u\n", secret_file_size);
    printf("INFO: Total data to hide = %u\n", encoding_things); */


    if(image_capacity >= encoding_things)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

uint get_file_size(FILE *fptr)
{
    if(fptr == NULL)
    {
        printf("error opening the file\n");
        return 0;
    }
    fseek(fptr, 0, SEEK_END);
    uint size = ftell(fptr);
    rewind(fptr);
    return size;
}
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char buffer[54];
    //move file fptr to the 0th index(start of file)
    fseek(fptr_src_image, 0, SEEK_SET);
    //read 54 bytes from the src image
    fread(buffer, 54, 1, fptr_src_image);
    //write 54 bytes to the dest image
    fwrite(buffer, 54, 1, fptr_dest_image);
    return e_success;
}
// encoding the magic string
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
   // printf("len %ld",strlen(magic_string));
    if(encode_data_to_image(magic_string, strlen(magic_string),encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}
// fun() to encode data to image
Status encode_data_to_image(const char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    // read 8 bytes from src image
    char image_buffer[8];
    for(int i=0; i<size; i++)
    {
        fread(image_buffer, 8, 1, fptr_src_image);

        // fun() to encode the byte
        encode_byte_to_lsb(data[i], image_buffer);
        //write 8 bytes to the dest image
        fwrite(image_buffer, 8, 1, fptr_stego_image);
    }
    return e_success;
}
// encode the size of the secret.txt into the stego image
Status encode_secret_file_extn_size(uint extn_size, EncodeInfo *encInfo)
{
    char str[32];
    //read 32 bytes from the src image
    fread(str, 32, 1, encInfo ->fptr_src_image);

    //encode extns size into the LSB of buffer
    encode_int_to_lsb(extn_size, str);

    fwrite(str, 32, 1, encInfo -> fptr_stego_image);

    return e_success;
}
/*encode int to lsb*/
Status encode_int_to_lsb(int size, char * image_buffer)
{
    for(int i=0; i<32; i++)
    {
        unsigned int bit = (size >> i) & 1;
        image_buffer[i] = ((image_buffer[i] & ~(1)) | bit);
    }
    return e_success;
}
/* Encode a byte into LSB of image data array */
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i=0; i<8; i++)
    {
        int bit = (data >> i) & 1;
        image_buffer[i] = ((image_buffer[i] & (~1)) | bit);
    }
    return e_success;
}

/* Encode secret file extenstion */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
   if(encode_data_to_image(file_extn, strlen(file_extn), encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
   {
    return e_success;
   }
   else
   {
    return e_failure;
   }
}
/* Encode secret file size */
Status encode_secret_file_size(int file_size, EncodeInfo *encInfo)
{
    char str[32] ={0};
    //read 32 bytes from the src image
    fread(str, 32, 1, encInfo -> fptr_src_image);

    encode_int_to_lsb(file_size, str);
    // write 32 bytes to the dest image
    fwrite(str, 32, 1, encInfo -> fptr_stego_image);
    return e_success;
}
/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    int i = 0;
    char ch;
    char secret_file_data[encInfo -> size_secret_file];
    while((ch  = fgetc(encInfo->fptr_secret)) != EOF && i< encInfo ->size_secret_file)
    {
        secret_file_data[i] = ch;
        i++;
    }
    if(encode_data_to_image(secret_file_data, encInfo -> size_secret_file, encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}
/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char buffer[1024];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), fptr_src)) > 0)
    {
        if (fwrite(buffer, 1, bytes_read, fptr_dest) != bytes_read)
        {
            return e_failure;
        }
    }
    return e_success;
}

   