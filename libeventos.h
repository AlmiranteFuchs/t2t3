/*
 * Feito em C para a disciplina CI1001 - Programacao 1
 * Autor: B Fuchs
 *
 * Versao 1.0.0 de 9/09/2022
 */

/*
 * ######################################################################################################################
 *  Diz respeito sobre definições de comportamento de eventos, suas criações e operações pois o liblef.h é intocável ;'(
 * ######################################################################################################################
 */

#include "liblef.h"

typedef enum /* Enum tipos de eventos*/
{
    CHEGADA,
    PARTIDA,
    DISSEMINACAO,
    FIM_SIMULACAO
} tipo_evento_t;

/*
 *  Tipos de eventos
 */

typedef struct chegada
{
    int id_pessoa;
    int id_local;
} chegada_t;

typedef struct saida
{
    int id_pessoa;
    int id_local;
} saida_t;

typedef struct disseminacao
{
    conjunto_t *rumores;
    int id_pessoa;
    int id_local;
} disseminacao_t;

typedef struct fim_simulacao
{
    int tempo;
} fim_simulacao_t;

/*
 *  Operações de eventos
 */

void trata_evento_chegada(int id_pessoa, int id_local, mundo_t *mundo, lef_t *lef);                              /* Trata evento de chegada */
void trata_evento_partida(int id_pessoa, int id_local, mundo_t *mundo, lef_t *lef);                              /* Trata evento de partida */
void trata_evento_disseminacao(int id_pessoa, int id_local, conjunto_t *cj_rumores, mundo_t *mundo, lef_t *lef); /* Trata evento de disseminação */
void trata_evento_fim_simulacao(mundo_t *mundo, lef_t *lef);                                                     /* Trata evento de fim de simulação */
void destroi_evento_chegada(evento_t *evento);                                                                   /* Destroi evento de chegada */
void destroi_evento_partida(evento_t *evento);                                                                   /* Destroi evento de partida */
void destroi_evento_disseminacao(evento_t *evento);                                                              /* Destroi evento de disseminação */
void destroi_evento_fim_simulacao(evento_t *evento);                                                             /* Destroi evento de fim de simulação */

evento_t *retirar_evento(lef_t *lef);
/*
 *  Definição de eventos
 */

evento_t *cria_evento_chegada(int id_pessoa, int id_local, int tempo);                              /* Cria evento de chegada */
evento_t *cria_evento_partida(int id_pessoa, int id_local, int tempo);                              /* Cria evento de partida */
evento_t *cria_evento_disseminacao(int id_pessoa, int id_local, conjunto_t *cj_rumores, int tempo); /* Cria evento de disseminação */
evento_t *cria_evento_fim_simulacao(int tempo);                                                     /* Cria evento de fim de simulação */