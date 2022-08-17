#include <stdlib.h>
#include "libfila.h"

/* !# PROTOTYPES #! */
nodo_f_t *cria_nodo(int chave);
int retira_fila (fila_t* f, int* elemento);


/*
 *  !# Cria uma fila vazia e a retorna, se falhar retorna NULL. #!
*/
fila_t* cria_fila ()
{
     /* #Tenta alocar espaço para a fila type Struct  */
    fila_t *f;
    if (!(f = malloc(sizeof(fila_t))))
        return NULL;

    /* #Tenta alocar espaço para o prmeiro nodo da fila */
    nodo_f_t *novo;
    if(!(novo = cria_nodo(0))){
        free(f);
        return NULL;
    }

    /* # Faz com que Nodo criado seja a fim e ini */
    f->ini = novo;
    f->fim = novo;
    f->tamanho = 0;
    
    return f;
}

/*
 *  !# Remove todos os elementos da fila, libera espaco e devolve NULL. #!
 */
fila_t* destroi_fila (fila_t* f)
{
    /* # Somente para uso na func*/
    int batata = 0;
    while (retira_fila(f, &batata))
    {
        /* ;) */
    }

    /* # É necessário limpar o último elemento! */
    f->ini->chave = 0;
    free(f->ini);
    free(f);
    return NULL;
}

/*
 *  !# Retorna 1 se a fila esta vazia e 0 caso contrario. #!
 */
int fila_vazia (fila_t* f)
{
    /* # Verifica se a fila está vazia */
    if(f->fim == f->ini)
        return 1;
    return 0;
}

/*
 *  !# Retorna o tamanho da fila, isto eh, o numero de elementos presentes nela. #!
 */
int tamanho_fila (fila_t* f)
{
    return f->tamanho;
}

/*
 *  !# Insere o elemento no final da fila (politica FIFO). #!
 *  !# Retorna 1 se a operacao foi bem sucedida e 0 caso contrario. #!
 */
int insere_fila (fila_t* f, int elemento)
{
    /* # Não é necessário saber se está cheio, pois não há tamanho máximo */
    /* #Tenta alocar espaço para o novo nodo da fila */
    nodo_f_t *novo;
    if(!(novo = cria_nodo(elemento))){
        /* free(f); --> Matar a estrutura não é interessante se chegar no limite */
        return 0;
    }
    f->fim->prox = novo;
    f->fim = novo;
    f->tamanho++;
    return 1;
}

/*
 *  !# Remove o elemento do inicio da fila (politica FIFO) e o retorna. #!
 *  !# Retorna 1 se a operacao foi bem sucedida e 0 caso contrario. #!
 */
int retira_fila (fila_t* f, int* elemento)
{
    /* # É preciso verificar se fila vazia */
    if (fila_vazia(f))
        return 0;
    
    /* # A cabeça aponta para a fim */
    nodo_f_t *aux = f->ini;
    f->ini = aux->prox;

    *elemento = f->ini->chave;
    f->tamanho--;

    /* # Free em lixo, olá grégio */
    aux->chave = 0;
    aux->prox = NULL;
    free(aux);
    
    return 1;
}

/*
 *  !# Nodos #!
 */
nodo_f_t *cria_nodo(int chave)
{
    /* #Tenta criar um novo nodo */
    nodo_f_t *novo;
    if (!(novo = malloc(sizeof(nodo_f_t))))
        return NULL;

    /* # Preenche o nodo criado */
    novo->chave = chave;
    novo->prox = NULL;

    return novo;
}
