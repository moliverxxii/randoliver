/*
 * util.c
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */
#include "main.h"

int saturator(int input,int min, int max){
 /* Ecrête le signal entre "min" et "max"
  *
  */
 if(input<min) return min;
 else if(input>max) return max;
 else return input;
}

int modulo(int input, int modulo){
	// Renvoie le VRAI reste de la division euclidienne de "input" par "modulo".
	//modulo must be different to 0;
	int output = input;
	int multiplier = modulo;
	if( ((modulo>0) & (input>0)) | ((modulo<0) & (input<0)) )
	{
		multiplier *= -1;
	}

	while(output>=modulo | output<0)
	{
		output += multiplier;
	}
	return output;

}

void ok(int num){
	//Affiche un signal ok numéroté.
	printf("OK n°%d\n",num);
}

void okP(int num, void* pointer){
	/*Affiche un signal ok numéroté, tout en vérifiant la disponibilité en mémoire
	 * du pointeur.
	 */

	if(pointer == NULL)
	{
		printf("merde n°%d\n",num);
	}
	else ok(num);
}
