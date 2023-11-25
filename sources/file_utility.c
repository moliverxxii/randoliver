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
    FILE* image_file = fopen(file_name, "wb+");

    //HEADER
    init_header(image_file, image->width, image->height);

    //DATA
    write_image(image, image_file);
    return image_file;
}

void
init_header(FILE* file, int width, int height)
{
    fseek(file, 0, SEEK_SET);

    //File info
    char magic_number[2] =
    {0x42, 0x4D};
    int app_signature = 0X32326F6D;
    uint data_offset = HEADER_SIZE;
    uint dib_header_size = 40;
    uint image_size = 3 * width * height;
    uint file_size = data_offset + image_size;

    //Image info
    int compression = 0;
    uint depth = 0x00180001;
    int res_x = RES;
    int res_y = RES;
    int color_number = 0;
    int important_colours = 0;

    //FormatHeader
    fwrite(magic_number, 1, 2, file);
    fwrite(&file_size, 4, 1, file);
    fwrite(&app_signature, 4, 1, file);
    fwrite(&data_offset, 4, 1, file);

    //DIBHeader
    fwrite(&dib_header_size, 4, 1, file);
    fwrite(&width, 4, 1, file);
    fwrite(&height, 4, 1, file);
    fwrite(&depth, 4, 1, file);
    fwrite(&compression, 4, 1, file);
    fwrite(&image_size, 4, 1, file);
    fwrite(&res_x, 4, 1, file);
    fwrite(&res_y, 4, 1, file);
    fwrite(&color_number, 4, 1, file);
    fwrite(&important_colours, 4, 1, file);

}

void
write_image(const image_t* image, FILE* image_file)
{
    //Writes an image into a windows BITMAP stream.
    fseek(image_file, HEADER_SIZE, SEEK_SET);
    int x;
    int y;
    for(y = 0; y < image->height; ++y)
    {
        for(x = 0; x < image->width; ++x)
        {
            fwrite(image->image[x][y], sizeof(uchar), 3, image_file);
        }
    }
}

int
get_width(FILE* image_file)
{
    //Gets the width of the BITMAP array from a WINDOWS BITMAP stream.
    fseek(image_file, 18, SEEK_SET);
    return get_int(image_file);
}

int
get_height(FILE* image_file)
{
    //Gets the height of the BITMAP array from a WINDOWS BITMAP stream.
    fseek(image_file, 22, SEEK_SET);
    return get_int(image_file);
}

int
get_int(FILE* file)
{
    //Returns an int from the current file.
    int output = 0;
    int input;
    char i;
    for(i = 0; i < 4; ++i)
    {
        input = fgetc(file);
        output += input << 8 * i;

    }
    return output;
}

char*
bmp_extension(const char* input)
{
    //Returns the name for BMP file with the ".bmp" extension
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
    // Creates a string with a number appended to the end of the input.
    char* output = NULL;
    int length = strlen(input);
    int extension;
    char empty[0x100] = {0};
    length += 2 + sprintf(empty,"%d",number);
    output = malloc(length);
    sprintf(output, "%s %d", input, number);
    return output;
}
