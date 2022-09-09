#include "liblef.h"
#include <stdlib.h>
#include <stdio.h>

/* # Lista de futuros eventos# */

/* 
 * Cria uma LEF vazia 
 */
lef_t * cria_lef ()
{
    
    lef_t* lef;
    nodo_lef_t* nodo_lef;

    if (!(lef = malloc(sizeof(lef_t))))
        return NULL;

    nodo_lef_t* novo;
    if(!(novo = cria_nodo_lef())){
        free(lef);
        return NULL;
    }

    lef->Primeiro = NULL;
    
    return lef;

}

nodo_lef_t* cria_nodo_lef()
{
    nodo_lef_t* novo;
    if (!(novo = malloc(sizeof(nodo_lef_t))))
        return NULL;
    
    novo->evento = NULL;
    novo->prox = NULL;

    return novo;
}