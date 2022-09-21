#include <stdlib.h>
#include "libmundo.h"
#include "liblef.h"

mundo_t *cria_mundo(int nro_pessoas, int nro_locais, int nro_rumores, int max_x, int max_y, lef_t* lef)
{
    /* Tenta alocar espaço para mundo */
    mundo_t *m;
    if (!(m = malloc(sizeof(mundo_t))))
        return NULL;

    /* Inicializa mundo */
    m->tempo_atual = 0;
    m->nro_pessoas = nro_pessoas;
    m->nro_locais = nro_locais;
    m->nro_rumores = nro_rumores;
    m->tamanho_mundo.x = max_x;
    m->tamanho_mundo.y = max_y;

    /* Tenta alocar espaço para pessoas */
    if (!(m->pessoas = malloc(sizeof(pessoa_t) * nro_pessoas)))
    {
        free(m);
        return NULL;
    }

    /* Tenta alocar espaço para locais */
    if (!(m->locais = malloc(sizeof(local_t) * nro_locais)))
    {
        free(m->pessoas);
        free(m);
        return NULL;
    }

    /* Tenta alocar espaço para rumores */
    if (!(m->rumores = cria_conjunto(nro_rumores)))
    {
        free(m->pessoas);
        free(m->locais);
        free(m);
        return NULL;
    }

    /* Inicialização */
    int i;
    /* Inicializa pessoas */
    for (i = 0; i < nro_pessoas; i++)
    {
        m->pessoas[i].id_pessoa = i;
        m->pessoas[i].extroversao = aleat(0, 100);
        m->pessoas[i].paciencia = aleat(0, 100);
        m->pessoas[i].idade = aleat(18, 100);
        m->pessoas[i].rumores = cria_subconjunto(m->rumores, aleat(1, 5));
    }

    /* Inicializa locais */
    for (i = 0; i < nro_locais; i++)
    {
        m->locais[i].id_local = i;
        m->locais[i].capacidade = aleat(5, 30);
        m->locais[i].localizacao.x = aleat(0, max_x);
        m->locais[i].localizacao.y = aleat(0, max_y);
        m->locais[i].pessoas = cria_conjunto(nro_pessoas);
        m->locais[i].fila = cria_fila();
    }

    /* Inicializa rumores */
    for (i = 0; i < nro_rumores; i++)
    {
        insere_conjunto(m->rumores, i);
    }

    /* Para cada pessoa */
    for (i = 0; i < nro_pessoas; i++)
    {
        /* Escolhe um local aleatório */
        int local = aleat(0, nro_locais - 1);
        /* Evento de chegada */
        evento_t *evento_chegada;
        /* Insere evento na lef */

        if (!(evento_chegada = cria_evento_chegada(i, local, aleat(0, 96 * 7))))
            return NULL;

        /* Insere na lef */
        if (!(adiciona_ordem_lef(lef, evento_chegada)))
            return NULL;
    }
    return m;
}

void destroi_mundo(mundo_t *m)
{
    int i;
    for (i = 0; i < m->nro_pessoas; i++)
    {
        destroi_conjunto(m->pessoas[i].rumores);
    }
    for (i = 0; i < m->nro_locais; i++)
    {
        destroi_fila(m->locais[i].fila);
        destroi_conjunto(m->locais[i].pessoas);
    }
    free(m->pessoas);
    free(m->locais);
    free(m->rumores);
    free(m);
}

/*
 * ###############################
 *      Informações / Imprime
 * ###############################
 */

void imprime_tempo_mundo(mundo_t *m)
{
    int i;
    printf("Tempo atual: %d)\n", m->tempo_atual);
}

void imprime_pessoas_mundo(mundo_t *m)
{
    int i;
    printf("Pessoas:\n");
    for (i = 0; i < m->nro_pessoas; i++)
    {
        printf("Pessoa %d: extroversao %d, paciencia %d, idade %d, rumores ", m->pessoas[i].id_pessoa, m->pessoas[i].extroversao, m->pessoas[i].paciencia, m->pessoas[i].idade);
        imprime_conjunto(m->pessoas[i].rumores);
        printf("\n");
    }
}

void imprime_locais_mundo(mundo_t *m)
{
    int i;
    printf("Locais:\n");
    for (i = 0; i < m->nro_locais; i++)
    {
        printf("Local %d: capacidade %d, nro_pessoas %d", m->locais[i].id_local, m->locais[i].capacidade, m->locais[i].pessoas->card);
        /* imprime_fila(m->locais[i].fila); */ /* FIXME: Não tem imprime fila lmao */
        printf("\n");
    }
}

void imprime_rumores_mundo(mundo_t *m)
{
    int i;
    printf("Rumores:\n");
    imprime(m->rumores);
}

void imprime_mundo(mundo_t *m)
{
    imprime_tempo_mundo(m);
    imprime_pessoas_mundo(m);
    imprime_locais_mundo(m);
    imprime_rumores_mundo(m);
}

/*
 * ##############################
 *      Funções de busca
 * ##############################
 */

local_t *procura_local(int id_local, local_t *locais)
{
    if (!(&locais[id_local]))
        return NULL;
    return &locais[id_local];
}

pessoa_t *procura_pessoa(int id_pessoa, pessoa_t *pessoas)
{
    if (!(&pessoas[id_pessoa]))
        return NULL;
    return &pessoas[id_pessoa];
}

int local_cheio(local_t *local)
{
    return local->pessoas->card >= local->capacidade;
}

/*
 * ####################
 *      Auxiliares
 * ####################
 */

int adiciona_pessoa_fila(fila_t *fila, int id_pessoa)
{
    if (fila_cheia(fila))
        return 0;

    insere_fila(fila, id_pessoa);
    return 1;
}

int adiciona_rumor_pessoa(pessoa_t *p, int id_rumor)
{
    if (!(insere_conjunto(p->rumores, id_rumor)))
        return NULL;
    return 1;
}
