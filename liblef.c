#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "liblef.h"
#include "libmundo.h"
#include "libfila.h"
#include "libaleat.h"

/* # Lista de futuros eventos# */

/* # Prototypes # */
nodo_lef_t *cria_nodo_lef();
lef_t *destroi_lef(lef_t *l);
void destroi_nodo_lef(nodo_lef_t *nodo);
void destroi_chegada(void *dados);

/*
 * Cria uma LEF vazia
 */
lef_t *cria_lef()
{

    lef_t *lef;
    if (!(lef = malloc(sizeof(lef_t))))
        return NULL;

    nodo_lef_t *novo;
    if (!(novo = cria_nodo_lef()))
    {
        free(lef);
        return NULL;
    }

    lef->Primeiro = novo;

    return lef;
}

/*
 * Destroi a LEF, sempre retorna NULL.
 * Para evento eh necessario executar a funcao destroidados para liberar os dados
 * que representam o evento.
 */
lef_t *destroi_lef(lef_t *l)
{
    nodo_lef_t *nodo = l->Primeiro;
    nodo_lef_t *aux;
    while (nodo != NULL)
    {
        aux = nodo;
        nodo = nodo->prox;
        destroi_nodo_lef(aux);
    }
    free(l);
    return NULL;
}

/*
 * Adiciona um evento na primeira posicao da LEF. Uma nova copia
 * da estrutura evento sera feita.
 * Retorna 1 em caso de sucesso, 0 caso contrario.
 */
int adiciona_inicio_lef(lef_t *l, evento_t *evento)
{
    /* Se já existe evento empurre ele pra cima */
    if (l->Primeiro->evento != NULL)
    {
        nodo_lef_t *novo;
        if (!(novo = cria_nodo_lef()))
            return 0;

        novo->prox = l->Primeiro;
        l->Primeiro = novo;

        memcpy(l->Primeiro->evento, evento, sizeof(evento_t));

        return 1;
    }

    if ((memcpy(l->Primeiro->evento, evento, sizeof(evento_t))))
    {
        return 1;
    }
    return 0;
}

/*
 * Adiciona um evento de acordo com o valor evento->tempo na LEF.
 * Uma nova copia da estrutura evento sera feita
 * Retorna 1 em caso de sucesso, 0 caso contrario.
 */
int adiciona_ordem_lef(lef_t *l, evento_t *evento)
{
    nodo_lef_t *local = l->Primeiro;

    if (l->Primeiro->prox == NULL)

        while (local->prox != NULL)
        {
            if (local->prox->evento->tempo > evento->tempo)
            {
                /* Posição atual que é necessário inserir o novo nodo */
                /* Troca os ponteiros dos nodos */
                nodo_lef_t *novo;
                if (!(novo = cria_nodo_lef()))
                {
                    return 0;
                }
                novo->prox = local->prox;
                local->prox = novo;

                /* Copia o evento para o novo nodo */
                if ((memcpy(novo->evento, evento, sizeof(evento_t))))
                {
                    return 1;
                }
                return 0;
            }
            local = local->prox;
        }
    return 0;
}

/*
 * Retorna o primeiro evento da LEF. A responsabilidade por desalocar
 * a memoria associado eh de quem chama essa funcao
 */
evento_t *obtem_primeiro_lef(lef_t *l) { return l->Primeiro->evento; }

/*
 * ####################
 *  Funcoes auxiliares
 * ####################
 */

nodo_lef_t *cria_nodo_lef()
{
    nodo_lef_t *novo;
    if (!(novo = malloc(sizeof(nodo_lef_t))))
        return NULL;

    novo->evento = NULL;
    novo->prox = NULL;

    return novo;
}

/* Funcao que libera evento->dados */ /*TESTAR*/
void destroi_nodo_lef(nodo_lef_t *nodo)
{
    if (nodo->evento->destroidados != NULL)
        nodo->evento->destroidados(nodo->evento->dados);
    free(nodo->evento);
    free(nodo);
}

/*
 * ####################
 * Funções Auxliares
 * ####################
 */
