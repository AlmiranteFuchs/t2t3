/* Definições para programa principal
 * Feito em C para a disciplina CI1001 - Programacao 1
 * Autor: B Fuchs
 *
 * Versao 1.0.0 de 7/09/2022
 */
#include "libconjunto.h"
#include "libfila.h"
#include "liblef.h"

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
    int id_local;             /* Id do local */
    int capacidade;           /* Capacidade máxima de pessoas no local */
    conjunto_t *pessoas;      /* Conjunto de pessoas no local */
    fila_t *fila;             /* Fila de pessoas no local */
    coordenada_t localizacao; /* Localização do local no mundo */
} local_t;

/* Define uma coordenada */
typedef struct coordenada
{
    int x; /* Coordenada x */
    int y; /* Coordenada y */
} coordenada_t;

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

/*
 * # Funções #
 */

mundo_t *cria_mundo(int nro_pessoas, int nro_locais, int nro_rumores, int max_x, int max_y, lef_t *lef); /* Cria mundo */
local_t *procura_local(int id_local, local_t *locais);                                                   /* Procura local a partir de id_local */
pessoa_t *procura_pessoa(int id_pessoa, pessoa_t *pessoas);                                              /* Procura pessoa a partir de id_pessoa */
void destroi_mundo(mundo_t *m);                                                                          /* Destroi mundo */
int local_cheio(local_t *local);                                                                         /* Checa se local está cheio */

/* Informações do mundo */
void imprime_mundo(mundo_t *m);         /* Imprime mundo */
void imprime_rumores_mundo(mundo_t *m); /* Imprime rumores do mundo */
void imprime_locais_mundo(mundo_t *m);  /* Imprime locais do mundo */
void imprime_pessoas_mundo(mundo_t *m); /* Imprime pessoas do mundo */
void imprime_tempo_mundo(mundo_t *m);   /* Imprime tempo do mundo */

/* Informações de busca nas entidades*/
local_t *procura_local(int id_local, local_t *locais);      /* Procura local a partir de id_local */
pessoa_t *procura_pessoa(int id_pessoa, pessoa_t *pessoas); /* Procura pessoa a partir de id_pessoa */
int local_cheio(local_t *local);                            /* Checa se local está cheio */

/* Operações com Entidades */
int adiciona_pessoa_fila(fila_t *fila, int id_pessoa); /* Adiciona pessoa na fila */
int adiciona_rumor_pessoa(pessoa_t *p, int id_rumor);  /* Adiciona rumor a pessoa */