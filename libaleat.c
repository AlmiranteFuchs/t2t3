/*
 * Feito em C para a disciplina CI1001 - Programacao 1
 * Autor: B Fuchs
 *
 * Versao 1.0.0 de 8/09/2022
 */

#include "libaleat.h"

int aleat(x, y)
{
    return (rand() % (y - x + 1)) + x;
}

int start_seed()
{
    srand(time(NULL));
    return 0;
}