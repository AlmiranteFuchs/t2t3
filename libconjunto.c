#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "libconjunto.h"

/* Prototypes */
int busca_binaria_recursiva(int *vector, int begin, int end, int elemento);
int desloca_vetor(int *v, int inicio, int final);
int troca_elementos(int *v, int ind_x, int ind_a);
int insere_ordenado(int *v, int final, int elemento);

/*
 *  Cria um conjunto vazio e o retorna, se falhar retorna NULL.
 *  max eh o tamanho maximo do conjunto, isto eh, o tamanho maximo do vetor
 */
conjunto_t *cria_conjunto(int max)
{

    /* #Tenta alocar espaço para a fila type Struct  */
    conjunto_t *conj;
    if (!(conj = (conjunto_t *)malloc(sizeof(conjunto_t))))
        return NULL;

    /* #Tenta alocar espaço para elementos do conjunto  */
    int *aux;
    if (!(aux = (int *)malloc(sizeof(int) * max)))
    {
        free(conj);
        return NULL;
    }

    /* #Se tudo foi alocado corretamente preencha os elementos do conjunto  */
    conj->max = max;
    conj->card = 0;
    conj->ptr = 0;
    conj->v = aux;

    return conj;
}

/*
 *  Remove todos os elementos do conjunto, libera espaco e devolve NULL.
 */
conjunto_t *destroi_conjunto(conjunto_t *c)
{
    int i;
    for (i = 0; i < c->card; i++)
    {
        c->v[i] = 0;
        c->card--;
    }
    free(c->v);
    free(c);
    return NULL;
};

/*
 *  Retorna 1 se o conjunto esta vazio e 0 caso contrario.
 */
int conjunto_vazio(conjunto_t *c) { return c->card == 0; }

/*
 *  Retorna 1 se o conjunto esta cheio e 0 caso contrario.
 */
int conjunto_cheio(conjunto_t *c) { return c->card -1 == c->max; }

/*
 *  Retorna a cardinalidade do conjunto, isto eh, o numero de elementos presentes nele.
 */
int cardinalidade(conjunto_t *c) { return c->card -1; }

/*
 *  Insere o elemento no conjunto, garante que nao existam repeticoes.
 *  Retorna 1 se a operacao foi bem sucedida, 0 se tenta inserir elemento existente,
 *  e retorna -1 em caso de falha por falta de espaco.
 */

int insere_conjunto(conjunto_t *c, int elemento)
{
    /* Checa se o conjunto está cheio */
    if (conjunto_cheio(c))
        return -1;

    if (busca_binaria_recursiva(c->v, 0, c->card - 1, elemento) != -1)
        return 0;

    /* Trata primeira inserção */
    if (conjunto_vazio(c))
    {
        c->v[0] = elemento;
        c->card++;
        return 1;
    }

    /* Insere ordenado */
    insere_ordenado(c->v, c->card, elemento);
    c->card++;
    return 1;
};

/*
 * Remove o elemento elemento do conjunto caso ele exista e o retorna.
 * Retorna 1 se a operacao foi bem sucedida e 0 se o elemento nao existe.
 */

int retira_conjunto(conjunto_t *c, int elemento)
{
    int indice_elem;
    if ((indice_elem = busca_binaria_recursiva(c->v, 0, c->card - 1, elemento)) != -1)
    {
        /* Caso ele exista */
        int aux = c->v[indice_elem];
        c->v[indice_elem] = 0;
        c->card--;

        desloca_vetor(c->v, indice_elem, c->card);
        return aux;
    }
    return 0;
}

/*
 * Retorna 1 se o elemento pertence ao conjunto e 0 caso contrario.
 */
int pertence(conjunto_t *c, int elemento)
{
    if (busca_binaria_recursiva(c->v, 0, c->card - 1, elemento) != -1)
        return 1;
    return 0;
}

/*
 * Retorna 1 se o conjunto c1 esta contido no conjunto c2 e 0 caso contrario.
 */
int contido(conjunto_t *c1, conjunto_t *c2)
{

    /* Se card de c1 > c2 inerentemente ele não está contido em c2 */
    if (c1->card > c2->card)
        return 0;

    int i;
    for (i = 0; i < c1->card; i++)
    {
        /* Procura algum elemento que não esteja em c2 */
        if (busca_binaria_recursiva(c2->v, 0, c2->card - 1, c1->v[i]) == -1)
            return 0;
    }
    return 1;
}

/*
 * Retorna 1 se o conjunto c1 eh igual ao conjunto c2 e 0 caso contrario.
 */
int sao_iguais(conjunto_t *c1, conjunto_t *c2)
{
    /* Cardinalidade fundamentalmente diferentes == diferentes */
    if (cardinalidade(c1) != cardinalidade(c2))
        return 0;

    int i;
    for (i = 0; i < c1->card; i++)
    {
        /* Caso não encontre o atual elemento de c1 em c2 == diferentes */
        if (busca_binaria_recursiva(c2->v, 0, c2->card - 1, c1->v[i]) == -1)
            return 0;
    }
    /* Caso encontre todos == iguais */
    return 1;
}

/*
 * Cria e retorna o endereco do conjunto diferenca entre c1 e c2, nesta ordem.
 * Retorna 0 se a operacao falhou.
 */
conjunto_t *cria_diferenca(conjunto_t *c1, conjunto_t *c2)
{
    conjunto_t *c_diferenca;

    if (!(c_diferenca = cria_conjunto(c1->max))) /* Max <= c1 == c2 */
        return NULL;

    int i;
    for (i = 0; i < c1->card; i++)
    {
        if (busca_binaria_recursiva(c2->v, 0, c2->card - 1, c1->v[i]) == -1)
        {
            /* Se ele achou atual elemento de c1 em c2 */
            insere_conjunto(c_diferenca, c1->v[i]);
        }
    }

    return c_diferenca;
}

/*
 * Cria e retorna o endereco do conjunto interseccao entre os conjuntos c1 e c2.
 * Retorna 0 se a operacao falhou.
 */
conjunto_t *cria_interseccao(conjunto_t *c1, conjunto_t *c2)
{
    conjunto_t *c_intersec;

    if (!(c_intersec = cria_conjunto(c1->max))) /* Max <= c1 == c2 */
        return NULL;

    int i;
    for (i = 0; i < c1->card; i++)
    {
        if (busca_binaria_recursiva(c2->v, 0, c2->card - 1, c1->v[i]) != -1)
        {
            /* Se ele achou atual elemento de c1 em c2 */
            insere_conjunto(c_intersec, c1->v[i]);
        }
    }

    return c_intersec;
}

/*
 * Cria e retorna o endereco do conjunto uniao entre os conjuntos c1 e c2.
 * Retorna NULL se a operacao falhou.
 */
conjunto_t *cria_uniao(conjunto_t *c1, conjunto_t *c2)
{
    /* Tenta criar um conjunto do tamanho dos dois somado*/
    conjunto_t *c_uniao;
    if (!(c_uniao = cria_conjunto(c1->max + c2->max)))
        return NULL;

    /* União --> Conjunto com todos os elementos de a e b */
    /* Empurra para c_uniao todos os elementos de c1 e c2, réplicas são tratadas no insere */
    int i;
    for (i = 0; i < c1->card; i++)
    {
        insere_conjunto(c_uniao, c1->v[i]);
    }

    for (i = 0; i < c2->card; i++){
        insere_conjunto(c_uniao, c2->v[i]);
    }

    return c_uniao;
}

/*
 * Retorna uma copia do conjunto c e 0 caso nao seja possivel.
 */
conjunto_t *cria_copia(conjunto_t *c)
{
    conjunto_t *c_copia;
    if ((c_copia = cria_conjunto(c->max)))
    {
        int i;
        for (i = 0; i < c->card; i++)
            insere_conjunto(c_copia, c->v[i]);

        return c_copia;
    }
    return 0;
}

/*
 * Cria um subconjunto com elementos aleatorios do conjunto c.
 * Se o conjunto for vazio, retorna um subconjunto vazio.
 * Se o n >= cardinalidade (c) entao retorna o proprio conjunto c.
 * Supoe que a funcao srand () tenha sido chamada antes.
 */
conjunto_t *cria_subconjunto(conjunto_t *c, int n)
{
    if (conjunto_vazio(c))
        return cria_conjunto(c->max);

    if (n >= c->card)
        return c;

    conjunto_t *c_aleatorio;
    if (!(c_aleatorio = cria_conjunto(n)))
        return NULL;

    /* Seed para random */
    time_t t;
    int i;

    srand((unsigned)time(&t));

    for (i = 0; i < n; i++)
    {
        int indice_aleatorio = rand() % c->card;
        insere_conjunto(c_aleatorio, c->v[indice_aleatorio]);
    }
    return c_aleatorio;
}

/*
 * Imprime os elementos do conjunto sempre em ordem crescente,
 * mesmo que a estrutura interna nao garanta isso.
 */
void imprime(conjunto_t *c)
{
    if (conjunto_vazio(c))
    {
        printf("Conjunto vazio\n");
        return;
    }

    int i;
    for (i = 0; i < c->card; i++)
        printf("%d\n", c->v[i]);
}

/*
 * Retorna 1 se conseguiu dobrar o tamanho max do conjunto e 0 caso contrario.
 */
int redimensiona(conjunto_t *c)
{
    /* Copia os valores para novo aloc, libera o espaço e recebe o novo alocamento */
    if (!(c->v = (int *)realloc(c->v, sizeof(int) * (c->max * 2))))
        return 0;
    c->max = c->max * 2;

    return 1;
}

/*
 * Inicializa o contador que sera usado na funcao incrementar_iterador
 */
void iniciar_iterador(conjunto_t *c) { c->ptr = 0; }

/*
 * Incrementa iterador e retorna o elemento apontado. Retorna 0 se acabou conjunto
 */
int incrementar_iterador(conjunto_t *c, int *elemento)
{
    if (c->ptr < c->card)
    {

        /* Redutante? retorno e ponteiro para elemento apontado ;x */
        *elemento = c->v[c->ptr];
        c->ptr++;
        return 1;
    }
    return 0;
}

/*
 * Escolhe um elemento qualquer do conjunto para ser removido e o remove.
 * Nao faz teste se conjunto eh vazio, deixa para main fazer isso
 */
int retirar_um_elemento(conjunto_t *c)
{
    /* Gera uma seed para o random */
    time_t t;
    srand((unsigned)time(&t));

    /* Gera um indice aleatorio */
    int min_number = 0;
    int max_number = c->card - 1;
    int indice_aleatorio = rand() % (max_number + 1 - min_number) + min_number;
    
    int aux = c->v[indice_aleatorio];
    
    c->v[indice_aleatorio] = 0;
    c->card--;

    /* Pra manter a estrutura ordenada, desloca vetor */
    desloca_vetor(c->v, indice_aleatorio, c->card);

    return aux;
}

/*
 * ###########################################
 * #### - Funções Privadas e auxiliares - ####
 * ###########################################
 */

/*
 *  Busca um elemento no vetor
 *  Busca binária, retorna 1 se encontrou, 0 senão
 */
int busca_binaria_recursiva(int *vector, int begin, int end, int elemento)
{
    /* Índice representando o meio do sub-vetor begin->end */
    int i = (begin + end) / 2;

    if (begin > end)
    { /* Ponto de parada. Item não está no vetor */
        return -1;
    }

    if (vector[i] == elemento)
    { /* Item encontrado */
        return i;
    }

    if (vector[i] < elemento)
    { /* Item está no sub-vetor à direita */
        return busca_binaria_recursiva(vector, i + 1, end, elemento);
    }
    else
    { /* vector[i] > item. Item está no sub-vetor à esquerda */
        return busca_binaria_recursiva(vector, begin, i - 1, elemento);
    }
    return -1; /* Reduntante, mas deixarei por medo */
}

/*
 *  Troca os indíces de x e a
 */
int troca_elementos(int *v, int ind_x, int ind_a)
{
    int aux = *(v + ind_x);
    *(v + ind_x) = *(v + ind_a);
    *(v + ind_a) = aux;
    return 1;
}

/*
 *  Insertion Sort ou algo do tipo
 */
int insere_ordenado(int *v, int final, int elemento)
{
    v[final] = elemento;
    if (&v[final] == &v[0])
        return 0;

    if (v[final - 1] > elemento)
    {
        troca_elementos(v, final, final - 1);
        return insere_ordenado(v, final - 1, elemento);
    }
    return 0;
}

/*
 *  Desloca vetor, troca do começo até o final
 */
int desloca_vetor(int *v, int inicio, int final)
{
    while (inicio < final)
    {
        troca_elementos(v, inicio, inicio + 1);
        inicio++;
    }
    return 1;
}
