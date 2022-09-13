/* Definições para programa principal
 * Feito em C para a disciplina CI1001 - Programacao 1
 * Autor: B Fuchs
 *
 * Versao 1.0.0 de 7/09/2022
 */

#include "libconjunto.h"
#include "libfila.h"

/* Define um mundo */
typedef struct mundo
{
    int tempo_atual;      /* Tempo atual da simulação, representa 15 minutos de tempo real */
    int nro_pessoas;      /* Número de pessoas na simuação */
    int nro_locais;       /* Número de locais na simulação */
    int nro_rumores;      /* Número de rumores na simulação */
    rumor_t *rumores;     /* Rumores que estão circulando no mundo */
    pessoa_t *pessoas;    /* Vetor de pessoas */
    local_t *locais;      /* Vetor de locais */
    coor_t tamanho_mundo; /* Tamanho máximo do mundo */
} mundo_t;

/* Define uma pessoa */
typedef struct pessoa
{
    int id_pessoa;       /* Id da pessoa */
    int extroversao;     /* De 0 até 100, define o grau de dissêminação de rumores */
    int paciencia;       /* De 0 até 100, define o grau de permanência */
    int idade;           /* De 18 até 100, idade afeta velocidade de deslocamento */
    conjunto_t *rumores; /* Conjunto de rumores que a pessoa conhece */
} pessoa_t;

/* Define um local */
typedef struct local
{
    int id_local;    /* Id do local */
    int capacidade;  /* Capacidade máxima de pessoas no local */
    int nro_pessoas; /* Número de pessoas no local */
    fila_t *fila;    /* Fila de pessoas no local */
} local_t;

/* Define um rumor */
typedef struct rumor
{
    int id_rumor; /* Id do rumor */
} rumor_t;

/* Define uma coordenada */
typedef struct coordenada
{
    int x; /* Coordenada x */
    int y; /* Coordenada y */
} coordenada_t;

/*
 * # Funções #
 */

/* Cria mundo */
mundo_t *cria_mundo(int nro_pessoas, int nro_locais, int nro_rumores, int max_x, int max_y);

/* Destroi mundo */
void destroi_mundo(mundo_t *m);

/* Cria pessoa */
pessoa_t *cria_pessoa(int id_pessoa, int extroversao, int paciencia, int idade);

/* Destroi pessoa */
pessoa_t *destroi_pessoa(pessoa_t *p);

/* Cria local */
local_t *cria_local(int id_local, int capacidade);

/* Destroi local */
local_t *destroi_local(local_t *l);

/* Cria rumor */
rumor_t *cria_rumor(int id_rumor);

/* Destroi rumor */
rumor_t *destroi_rumor(rumor_t *r);

/* Cria coordenada */
coor_t *cria_coordenada(int x, int y);

/* Destroi coordenada */
coor_t *destroi_coordenada(coor_t *c);

/* Adiciona pessoa ao mundo */
int adiciona_pessoa(mundo_t *m, pessoa_t *p);

/* Adiciona local ao mundo */
int adiciona_local(mundo_t *m, local_t *l);

/* Adiciona rumor ao mundo */
int adiciona_rumor(mundo_t *m, rumor_t *r);

/* Adiciona rumor a pessoa */
int adiciona_rumor_pessoa(pessoa_t *p, rumor_t *r);

/* Procura local a partir de id_local */
local_t *procura_local(mundo_t *m, int id_local);
