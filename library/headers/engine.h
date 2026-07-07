/*
 * engine.h
 *
 *  Created on: 9 nov. 2025
 *      Author: moliver
 */
//traite les arguments d'entrees en tant que parametres globaux du programme
//donne les noms de fichiers racines/dossiers
//s'occupe des interaction avec le system d'exploitation.
//seulement les arguments en ligne de commande pour les autres modules.
//
//ne fait pas: decodages des fichiers, independant des fonctions de rendu.
#ifndef HEADERS_ENGINE_H_
#define HEADERS_ENGINE_H_

typedef struct engine_t engine_t;

engine_t* engine_init(int argc, const char* argv[]);
void      engine_free(engine_t* engine_p);

#endif /* HEADERS_ENGINE_H_ */
