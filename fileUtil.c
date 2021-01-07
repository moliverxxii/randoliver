/*
 * fileUtil.c
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */
#include "main.h"

FILE* initImageFile(char name[], int width, int height,image_t image)
{
    //FILE CREATION AND NAME
    char* fileName = bmpExtension(name);
    FILE* imageFile = fopen(fileName,"wb+");

    //HEADER
    initHeader(imageFile,width,height);

    //DATA
    writeImage(image,width,height,imageFile);
    printf("fichier à : %p\n",imageFile);
    return imageFile;
}

void initHeader(FILE* file,int width, int height)
{
    fseek(file,0,SEEK_SET);

    //File info
    char magicNumber[2] = {0x42,0x4D};
    int appSignature = 0X32326F6D;
    uint dataOffset = HEADER_SIZE;
    uint dibHeaderSize = 40;
    uint imageSize = 3*width*height;
    uint fileSize = dataOffset + imageSize;

    //Image info
    int compression = 0;
    uint depth = 0x00180001;
    int resX = RES;
    int resY = RES;
    int colorNumber = 0;
    int importantColors = 0;

    //FormatHeader
    fwrite(magicNumber,1,2,file);
    fwrite(&fileSize,4,1,file);
    fwrite(&appSignature,4,1,file);
    fwrite(&dataOffset,4,1,file);

    //DIBHeader
    fwrite(&dibHeaderSize,4,1,file);
    fwrite(&width,4,1,file);
    fwrite(&height,4,1,file);
    fwrite(&depth,4,1,file);
    fwrite(&compression,4,1,file);
    fwrite(&imageSize,4,1,file);
    fwrite(&resX,4,1,file);
    fwrite(&resY,4,1,file);
    fwrite(&colorNumber,4,1,file);
    fwrite(&importantColors,4,1,file);

}

void writeImage(image_t image, int width, int height, FILE* imageFile)
{
    //Writes an image into a windows BITMAP stream.
    fseek(imageFile,HEADER_SIZE,SEEK_SET);
    int x;
    int y;
    size_t feedback;
    for(y=0;y<height;++y)
    {
        for(x=0; x<width; ++x)
        {
            feedback = fwrite(image[x][y],sizeof(uchar),3,imageFile);
        }
    }
    printf("Image written into file.\n");
}


int getWidth(FILE* imageFile)
{
    //Gets the width of the BITMAP array from a WINDOWS BITMAP stream.
    fseek(imageFile,18,SEEK_SET);
    return getInt(imageFile);
}

int getHeight(FILE* imageFile)
{
    //Gets the height of the BITMAP array from a WINDOWS BITMAP stream.
    fseek(imageFile,22,SEEK_SET);
    return getInt(imageFile);
}

int getInt(FILE* file)
{
    //Returns an int from the current file.
    int output = 0;
    int input;
    char i;
    for(i=0; i<4;++i)
    {
        input = fgetc(file);
        output +=  input << 8*i;

    }
    return output;
}

char* bmpExtension(char input[])
{
    //Returns the name for BMP file with the ".bmp" extension
    char* output = NULL;
    int length = strlen(input);
    length +=4;
    output = malloc(length);
    strcpy(output,input);
    strcat(output,".bmp");
    return output;
}

char* numExtension(char input[], int number)
{
    // Creates a string with a number appended to the end of the input.
    char* output = NULL;
    int length = strlen(input);
    char digit;
    int k;
    length +=3;
    output = malloc(length);
    strcpy(output,input);
    for(k=length-1; k>=length-3; --k)
    {
        digit = number%10;
        number = number/10;
        output[k] = digit + 48;
    }
    return output;
}
