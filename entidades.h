/* Definições para programa principal
 * Feito em C para a disciplina CI1001 - Programacao 1
 * Autor: B Fuchs
 *
 * Versao 1.0.0 de 7/09/2022
 */
#ifndef ENTIDADES_H
#define ENTIDADES_H

#include "libconjunto.h"
#include "libfila.h"

/* Define uma pessoa */
typedef struct pessoa
{
    int id_pessoa;       /* Id da pessoa */
    int extroversao;     /* De 0 até 100, define o grau de dissêminação de rumores */
    int paciencia;       /* De 0 até 100, define o grau de permanência */
    int idade;           /* De 18 até 100, idade afeta velocidade de deslocamento */
    conjunto_t *rumores; /* Conjunto de rumores que a pessoa conhece */
} pessoa_t;

/* Define uma coordenada */
typedef struct coordenada
{
    int x; /* Coordenada x */
    int y; /* Coordenada y */
} coordenada_t;

/* Define um local */
typedef struct local
{
    int id_local;             /* Id do local */
    int capacidade;           /* Capacidade máxima de pessoas no local */
    conjunto_t *pessoa_t;     /* Conjunto de pessoas no local */
    fila_t *fila_t;           /* Fila de pessoas no local */
    coordenada_t localizacao; /* Localização do local no mundo */
} local_t;


/* Define um mundo */
typedef struct mundo
{
    int tempo_atual;            /* Tempo atual da simulação, representa 15 minutos de tempo real */
    int nro_pessoas;            /* Número de pessoas na simuação */
    int nro_locais;             /* Número de locais na simulação */
    int nro_rumores;            /* Número de rumores na simulação */
    conjunto_t *rumores;        /* Rumores que estão circulando no mundo */
    pessoa_t *pessoas;          /* Vetor de pessoas */
    local_t *locais;            /* Vetor de locais */
    coordenada_t tamanho_mundo; /* Tamanho máximo do mundo */
} mundo_t;
#endif /* ENTIDADES_H */