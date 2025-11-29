/* Name : Vaishak S
   Batch : 25021B
   Date : 
   Description :
   */
#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
int main(int argc, char * argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    uint img_size; 

   // printf("starting program\n");

   OperationType op = check_operation_type(argv);

    if(op == e_encode)
    {
        printf("You have choosed encoding\n");
        if(read_and_validate_encode_args(argc, argv, &encInfo) == e_success)
        {
            printf("Read and Validate is Successfull✅\n");

            if(open_files(&encInfo) == e_success)
            {
                    // Test get_image_size_for_bmp
                img_size = get_image_size_for_bmp(encInfo.fptr_src_image);
                //printf("INFO: Image size = %u\n",img_size);

                // calling do encoding , proceed with encoding
                if(do_encoding(&encInfo) == e_success)
                {
                    printf("------------------------------\n");
                    printf("|::  ✅ Encoding successfull ✅  ::|\n");
                    printf("------------------------------\n");
                }
                else
                {
                    printf("❌ Encoding failed ❌.\n");
                }
            }
            else
            {
                printf("Error: opening files❌.\n");
                return 1;
            }
        }
        else
        {
            printf("Read and validate failed❌.\n");
            return 1;
        }
    }
    else if(op == e_decode)
    {
        printf("You have choosed decoding.\n");
        if(read_and_validate_decode_args(argc, argv, &decInfo) == e_success)
        {
            printf("Read and Validate is successfully✅\n");
            if (do_decoding(&decInfo) == e_success)
            {
                printf("-------------------------------\n");
                printf("|::  ✅ Decoding successfull ✅  ::|\n");
                printf("-------------------------------\n");
            }
            else  
            {
                printf("❌ Decoding failed ❌\n");
            }
        }
    }
    else
    {
        printf("Please enter the valid argument\n-e for encoding\n-d for decoding\n");
    }  
}
OperationType check_operation_type(char *argv[])
{
    if(argv[1] == NULL)
    {
       // printf("Please enter the valid argument\n-e for encoding\n-d for decoding\n");
        return e_unsupported;
    }
    if(strcmp(argv[1], "-e") == 0)
    {
        return e_encode;
    }
    else if(strcmp(argv[1], "-d") == 0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}
Status read_and_validate_encode_args(int argc, char *argv[], EncodeInfo *encInfo)
{
    // 1. minimum argument check
    if(argc != 4 && argc != 5)
    {
        printf("Usage: %s -e <bmp file> <txt file> [output bmp]\n", argv[0]);
        return e_failure;
    }
    //2. source image file must end with .bmp
    char *ret = strstr(argv[2], ".bmp");
    if(ret == NULL)
    {
        printf("Wrong extension passed, not .bmp file❌.\n");
        return e_failure;
        
    }
    else if(strcmp(ret,".bmp")!=0)
    {
        printf("Wrong extension passed, not .bmp file❌.\n");
        return e_failure;
    }
    else
    {
        printf("Passed file has .bmp extension✅.\n");
        encInfo -> src_image_fname = argv[2];
    }

    //3.secret file must be .txt
    char *res = strstr(argv[3], ".txt");
    if(res == NULL)
    {
        printf("Wrong extension passed, not .txt file❌.\n");
        return e_failure;
    }
    else if(strcmp(res, ".txt")!= 0)
    {
        printf("Wrong extension passed, not .txt file❌.\n");
        return e_failure;  
    }
    else
    {
        printf("Passed file has .txt extension✅\n");
        encInfo -> secret_fname = argv[3];
        strcpy(encInfo -> extn_secret_file, res);
    }

    //4. stego file must be .bmp
    //4. stego file must be .bmp
    if(argc == 5)
    {
        if(strstr(argv[4], ".bmp") == NULL)
        {
            printf("Wrong extension passed, not .bmp file❌.\n");
            return e_failure;
        }
        else
        {
            printf("Passed file has .bmp extension✅\n");
            encInfo -> stego_image_fname = argv[4];
        }
    }
    else // argc == 4
    { 
        printf("Output File not mentioned. Creating stego.bmp as default\n");
        encInfo -> stego_image_fname = "stego.bmp";
    }
    return e_success;
}
    /*Read and validate decode ags from argv*/
Status read_and_validate_decode_args(int argc, char *argv[], DecodeInfo *decInfo)
{
    //1. minimum args check
    if(argc != 3 && argc != 4)
    {
        printf("Usage: %s -e <bmp file> <txt file> [output bmp]\n", argv[0]);
        return e_failure;
    }


    //2. stego image file must end with .bmp
    char *ret = strstr(argv[2], ".bmp");
    if(ret == NULL || strcmp(ret, ".bmp") !=0)
    {
        printf("Wrong extension passed, not .bmp file❌.\n");
        return e_failure;
    }
    else 
    {
        printf("Passed file has .bmp extension✅\n");
        decInfo -> stego_image_fname = argv[2];
       // return e_success;
    }

    //3. Output file
    if(argc == 4)
    {
        decInfo->output_fname = argv[3];
    }
    
    else
    {
        decInfo->output_fname = "output";
    }
    return e_success;
}



