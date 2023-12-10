/*
 * fileUtil.c
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */
#include "main.h"

FILE*
init_image_file(char* name, image_t* image)
{
    //FILE CREATION AND NAME
    char* file_name = bmp_extension(name);
    if(!file_name)
    {
    	printf("file name error\n");
    }
    FILE* image_file = fopen(file_name, "wb+");
    if(!image_file)
    {
    	printf("image file error\n");
    }


    //HEADER
    init_header(image_file, image->width, image->height);

    //DATA
    write_image(image, image_file);
    return image_file;
}

const char bmp_generator_signature[sizeof(uint32_t)] = {'m', 'o', '2', '2'};

void
init_header(FILE* file, int width, int height)
{
    fseek(file, 0, SEEK_SET);
    bmp_header_t header =
    {
    	    {0x42, 0x4D},
    	    HEADER_SIZE + 3 * width * height,
    	    *(uint32_t*) bmp_generator_signature,
    	    HEADER_SIZE,

    	    40,
			width,
			height,
    	    0x00180001,
    	    0,
    	    3 * width * height,
    	    RES,
    	    RES,
    	    0,
    	    0
    };
    fwrite(&header, sizeof(header), 1, file);
}

void
write_image(const image_t* image, FILE* image_file)
{
    fseek(image_file, HEADER_SIZE, SEEK_SET);
    fwrite(*image->image, image->height * image->width * sizeof(colour_t), 1, image_file);
}

int
get_int(FILE* file)
{
    int output = 0;
    fread(&output, sizeof(int), 1, file);
    return output;
}

int
get_width(FILE* image_file)
{
    fseek(image_file, 18, SEEK_SET);
    return get_int(image_file);
}

int
get_height(FILE* image_file)
{
    fseek(image_file, 22, SEEK_SET);
    return get_int(image_file);
}

char*
bmp_extension(const char* input)
{
    char* output = NULL;
    int length = strlen(input);
    length += 4;
    output = malloc(length);
    strcpy(output, input);
    strcat(output, ".bmp");
    return output;
}

char*
num_extension(const char* input, int number)
{
    char* output = NULL;
    int length = strlen(input);
    char empty[0x100] = {0};
    length += 2 + sprintf(empty,"%d",number);
    output = malloc(length);
    sprintf(output, "%s %d", input, number);
    return output;
}
