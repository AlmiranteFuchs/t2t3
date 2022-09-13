#include <stdlib.h>
#include "libmundo.h"
#include "libfila.h"
#include "libconjunto.h"

mundo_t *cria_mundo(int nro_pessoas, int nro_locais, int nro_rumores, int max_x, int max_y)
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
    if (!(m->rumores = malloc(sizeof(rumor_t) * nro_pessoas)))
    {
        /*  free(m->coordenadas); */
        free(m->locais);
        free(m->pessoas);
        free(m);
        return NULL;
    }

    /* Inicialização */
    int i;
    srand(time(NULL));

    /* Inicializa pessoas */
    for (i = 0; i < nro_pessoas; i++)
    {
        m->pessoas[i].id_pessoa = i;
        m->pessoas[i].extroversao = rand() % 101;
        m->pessoas[i].paciencia = rand() % 101;
        m->pessoas[i].idade = rand() % 119;
        m->pessoas[i].rumores = cria_conjunto(nro_rumores);
    }

    /* Inicializa locais */
    for (i = 0; i < nro_locais; i++)
    {
        m->locais[i].id_local = i;
        m->locais[i].capacidade = rand() % 101;
        m->locais[i].nro_pessoas = 0;
        m->locais[i].fila = cria_fila();
    }

    /* Inicializa rumores */
    for (i = 0; i < nro_rumores; i++)
    {
        m->rumores[i].id_rumor = i;
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
    }
    free(m->pessoas);
    free(m->locais);
    free(m->rumores);
    free(m);
}



void imprime_mundo(mundo_t *m)
{
    int i;
    printf("Tempo atual: %d)\n", m->tempo_atual);
}
