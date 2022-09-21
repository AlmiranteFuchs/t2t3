#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "libaleat.h"
#include "liblef.h"
#include "entidades.h"

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

    lef->Primeiro = NULL;

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
    nodo_lef_t *novo;
    evento_t *novo_evento;
    
    /* Aloca espaço para evento */
    if (!(novo_evento = (evento_t *)malloc(sizeof(evento_t))))
        return 0;
    
    /* Preenche evento */
    novo_evento->tempo = evento->tempo;
    novo_evento->tipo = evento->tipo;
    novo_evento->dados = evento->dados;
    novo_evento->tamanho = evento->tamanho;
    novo_evento->dados = malloc(evento->tamanho);

    /* Copia */
    memcpy(novo_evento->dados, evento->dados, evento->tamanho);

    /* Cria um nodo */
    if (!(novo = cria_nodo_lef()))
        return 0;

    /* Preenche nodo */
    novo->evento = novo_evento;
    novo->prox = NULL;

    /* Se não for o primeiro nodo */
    if (l->Primeiro != NULL)
    {
        novo->prox = l->Primeiro;
        l->Primeiro = novo;
    }
    else
    {
        l->Primeiro = novo;
    }

    return 1;
}

/*
 * Adiciona um evento de acordo com o valor evento->tempo na LEF.
 * Uma nova copia da estrutura evento sera feita
 * Retorna 1 em caso de sucesso, 0 caso contrario.
 */
int adiciona_ordem_lef(lef_t *l, evento_t *evento)
{
    nodo_lef_t *nodo, *nodo_ante, *nodo_atual;
    evento_t *novo_evento;
    
    if ((novo_evento = (evento_t*) malloc (sizeof(evento_t)))) {
        novo_evento->tempo = evento->tempo;
        novo_evento->tipo = evento->tipo;
        novo_evento->tamanho = evento->tamanho;
        novo_evento->dados = malloc (evento->tamanho);
        memcpy (novo_evento->dados, evento->dados, evento->tamanho);
    } else return 0;

    if (!(nodo = malloc (sizeof (nodo_lef_t)))) return 0;
    nodo_atual = l->Primeiro;
    if (nodo_atual == NULL) {
        free(novo_evento->dados);
        free (novo_evento);
        free(nodo);
        adiciona_inicio_lef (l, evento);
        return 0;
    }
    while (nodo_atual->prox != NULL && nodo_atual->evento->tempo < evento->tempo) {
        nodo_ante = nodo_atual;
        nodo_atual = nodo_atual->prox;
    }
    if (nodo_atual == l->Primeiro && nodo_atual->evento->tempo >= evento->tempo) {
        free(novo_evento->dados);
        free (novo_evento); 
        free (nodo); 
        adiciona_inicio_lef (l, evento);
        return 0;
    } else if (nodo_atual->prox == NULL) {
        nodo->evento = novo_evento;
        nodo_atual->prox = nodo;
        nodo->prox = NULL;
    } else { 
        nodo->evento = novo_evento; 
        nodo_ante->prox = nodo; 
        nodo->prox = nodo_atual;
    }
    return 1;
}

/*
 * Retorna o primeiro evento da LEF. A responsabilidade por desalocar
 * a memoria associado eh de quem chama essa funcao
 */
evento_t *obtem_primeiro_lef(lef_t *l) { 
    nodo_lef_t *novo;
    evento_t *evento;
    
    if (!l->Primeiro) return NULL;

    evento = l->Primeiro->evento; 
    novo = l->Primeiro;

    l->Primeiro = l->Primeiro->prox;
    free (novo);
    return evento;
 }

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
