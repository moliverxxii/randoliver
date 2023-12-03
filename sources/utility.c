/*
 * util.c
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */
#include "main.h"

char marker_g = 0;

int saturator(int input, int min, int max)
{
    /* Ecrête le signal entre "min" et "max"
     *
     */
    if(input < min)
    {
        return min;
    }
    else if(input > max)
    {
        return max;
    }
    else
    {
        return input;
    }
}

int modulo(int input, int modulo)
{
    // Renvoie le VRAI reste de la division euclidienne de "input" par "modulo".
    //modulo must be different to 0;
    int output = input;
    int multiplier = modulo;
    if(((modulo > 0) & (input > 0)) | ((modulo < 0) & (input < 0)))
    {
        multiplier *= -1;
    }

    while(output >= modulo | output < 0)
    {
        output += multiplier;
    }
    return output;

}

void ok(int num)
{
    //Affiche un signal ok numéroté.
    printf("OK n°%d\n", num);
}

void okP(int num, void* pointer)
{
    /*Affiche un signal ok numéroté, tout en vérifiant la disponibilité en mémoire
     * du pointeur.
     */

    if(pointer == NULL)
    {
        printf("merde n°%d\n", num);
    }
    else
    {
        ok(num);
    }
}

void print_memory(void* pointer, size_t size)
{
	uint8_t* byte_p;
	for(byte_p = pointer; byte_p < (uint8_t*) pointer + size; ++byte_p)
	{
		printf("%02X ", *byte_p);

	}
	printf("\n");
}
