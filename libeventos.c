#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "libeventos.h"
#include "libaleat.h"
/*Prototype bc c is shit*/
void saida_saida (mundo_t *mundo, pessoa_t * pessoa_saida, int pessoa_fila, local_t *local, int caso);
void saida_chegada (mundo_t *mundo, pessoa_t *pessoa, local_t *local, int caso);
evento_t *cria_evento_fim_simulacao(int tempo);
evento_t *cria_evento_disseminacao(int id_pessoa, int id_local, conjunto_t *cj_rumores_pessoa, int tempo);
evento_t *cria_evento_partida(int id_pessoa, int id_local, int tempo);
evento_t *cria_evento_chegada(int id_pessoa, int id_local, int tempo);
void destroi_evento_fim_simulacao(evento_t *evento);

/*
 * ###############################
 *      Funções do Mundo
 * ###############################
 */

/*stupid old compiler*/
int max(int num1, int num2)
{
    return (num1 > num2 ) ? num1 : num2;
}

/* Cria o mundo e inicializa suas entidades */
mundo_t *cria_mundo(int n_tamanho, int n_rumores, int n_pessoas, int n_locais, lef_t *lef)
{
    /* Tenta alocar espaço para mundo */
    mundo_t *m;
    if (!(m = malloc(sizeof(mundo_t))))
        return NULL;

    /* Inicializa mundo */
    m->tempo_atual = 0;
    m->nro_pessoas = n_pessoas;
    m->nro_locais = n_locais;
    m->nro_rumores = n_rumores;
    m->tamanho_mundo.x = n_tamanho;
    m->tamanho_mundo.y = n_tamanho;

    /* Tenta alocar espaço para pessoas */
    if (!(m->pessoas = malloc(sizeof(pessoa_t) * n_pessoas)))
    {
        free(m);
        return NULL;
    }

    /* Tenta alocar espaço para locais */
    if (!(m->locais = malloc(sizeof(local_t) * n_locais)))
    {
        free(m->pessoas);
        free(m);
        return NULL;
    }

    /* Tenta alocar espaço para rumores */
    if (!(m->rumores = cria_conjunto(n_rumores)))
    {
        free(m->pessoas);
        free(m->locais);
        free(m);
        return NULL;
    }

    /* Inicialização */
    int i;

    /* Inicializa rumores */
    for (i = 0; i < n_rumores; i++)
    {
        insere_conjunto(m->rumores, i);
    }

        /* Inicializa pessoas */
    for (i = 0; i < n_pessoas; i++)
    {
        m->pessoas[i].id_pessoa = i;
        m->pessoas[i].extroversao = aleat(0, 100);
        m->pessoas[i].paciencia = aleat(0, 100);
        m->pessoas[i].idade = aleat(18, 100);
        m->pessoas[i].rumores = cria_subconjunto(m->rumores, aleat(1, 5));
    }

    /* Inicializa locais */
    for (i = 0; i < n_locais; i++)
    {
        m->locais[i].id_local = i;
        m->locais[i].capacidade = aleat(5, 30);
        m->locais[i].localizacao.x = aleat(0, n_tamanho);
        m->locais[i].localizacao.y = aleat(0, n_tamanho);
        m->locais[i].pessoa_t = cria_conjunto(n_pessoas);
        m->locais[i].fila_t = cria_fila();
    }


    /* Para cada pessoa */
    for (i = 0; i < n_pessoas; i++)
    {
        /* Escolhe um local aleatório */
        int local = aleat(0, n_locais - 1);
        
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

/* Libera o mundo */
void destroi_mundo(mundo_t *m)
{
    int i;
    for (i = 0; i < m->nro_pessoas; i++)
    {
        destroi_conjunto(m->pessoas[i].rumores);
    }
    for (i = 0; i < m->nro_locais; i++)
    {
        destroi_fila(m->locais[i].fila_t);
        destroi_conjunto(m->locais[i].pessoa_t);
    }
    free(m->pessoas);
    free(m->locais);
    free(m->rumores);
    free(m);
}

/*
 * ####################################
 *      Funções auxiliares
 * ####################################
 */

int local_cheio(local_t *local)
{
    return local->pessoa_t->card >= local->capacidade;
}

/*
 * ####################################
 *      Comportamento de eventos
 * ####################################
 */

void trata_evento_chegada(int id_pessoa, int id_local, mundo_t *mundo, lef_t *lef)
{
    /* Procura local */
    local_t *local;
    if (!(local = &mundo->locais[id_local]))
        return;

    /* Procura pessoa */
    pessoa_t *pessoa;
    if (!(pessoa = &mundo->pessoas[id_pessoa]))
        return;

    /* Checa se local está cheio */
    if (local_cheio(local))
    {
        if (pessoa->paciencia / 4 - tamanho_fila(local->fila_t) > 0)
        {
            /* Pessoa entra na fila */
            if (!(insere_fila(local->fila_t, id_pessoa)))
                return;

            saida_chegada(mundo, &mundo->pessoas[id_pessoa], local, 2);
        }
        else
        {
            /* Pessoa sai do local */
            /* Cria evento de saída */
            evento_t *evento_saida;
            if (!(evento_saida = cria_evento_partida(id_pessoa, id_local, mundo->tempo_atual + 0)))
                return;

            /* Insere na lef */
            if (!(adiciona_ordem_lef(lef, evento_saida)))
                return;

            saida_chegada(mundo, &mundo->pessoas[id_pessoa], local, 3);
        }
    }
    else
    {
        /* Calcule o tempo de permanência no local */
        int tpl = max(1, pessoa->paciencia / 10 + aleat(-2, 6));

        /* Cria evento de saida */
        evento_t *evento_saida;
        if (!(evento_saida = cria_evento_partida(id_pessoa, id_local, mundo->tempo_atual + tpl)))
            return;

        /* Insere na lef */
        if (!(adiciona_ordem_lef(lef, evento_saida)))
            return;

        insere_conjunto(local->pessoa_t, id_pessoa);
        saida_chegada(mundo, &mundo->pessoas[id_pessoa], local, 1);
    }
}

void trata_evento_partida(int id_pessoa, int id_local, mundo_t *mundo, lef_t *lef)
{
    /* Procura local */
    local_t *local;
    if (!(local = &mundo->locais[id_local]))
        return;

    if (!(fila_vazia(local->fila_t)))
    {
        /* Cria evento de chegada para id_pessoa na fila em id local */
        int id_pessoa_fila;
        if(!(retira_fila(local->fila_t, &id_pessoa_fila)))
            return;

        evento_t *evento_chegada;
        if (!(evento_chegada = cria_evento_chegada(id_pessoa_fila, id_local, mundo->tempo_atual + 0)))
            return;

        /* Insere no inicio da lef */
        if (!(adiciona_inicio_lef(lef, evento_chegada)))
            return;

        saida_saida(mundo, &mundo->pessoas[id_pessoa], id_pessoa_fila, local, 2);
    }

    /* Cria evento de chegada para id_pessoa em outro local */
    int id_local_novo = aleat(0, mundo->nro_locais - 1);

    local_t *local_novo;
    if (!(local_novo = &mundo->locais[id_local_novo]))
        return;

    pessoa_t *pessoa;
    if (!(pessoa = &mundo->pessoas[id_pessoa]))
        return;

    /* Distância cartesiana entre local e novo local */
    int distance = sqrt(pow(local->localizacao.x - local_novo->localizacao.x, 2) + pow(local->localizacao.y - local_novo->localizacao.y, 2));
    int velocity = 100 - max(0, pessoa->idade - 40);
    int TDL = distance / velocity;

    retira_conjunto(local->pessoa_t, id_pessoa);

    /* Cria evento de chegada */
    evento_t *evento_chegada;
    if (!(evento_chegada = cria_evento_chegada(id_pessoa, id_local_novo, (mundo->tempo_atual + TDL) / 15)))
        return;

    /* Insere na lef */
    if (!(adiciona_ordem_lef(lef, evento_chegada)))
        return;

    saida_saida(mundo, &mundo->pessoas[id_pessoa], -1, local, 1);
}

void trata_evento_disseminacao(int id_pessoa, int id_local, conjunto_t *cj_rumores, mundo_t *mundo, lef_t *lef)
{
    /* Procura local */
    local_t *local;
    if (!(local = &mundo->locais[id_local]))
        return;

    /* Procura pessoa */
    pessoa_t *pessoa_origem;
    if (!(pessoa_origem = &mundo->pessoas[id_pessoa]))
        return;


    int i_rumor, i_pessoa, rumor, id_destinatario;
    iniciar_iterador(local->pessoa_t);

    printf("%6d:RUMOR Pessoa %4d Local %2d ", mundo->tempo_atual, pessoa_origem->id_pessoa, local->id_local);
    /* Para cada pessoa no local  */
    iniciar_iterador (cj_rumores);
    for (i_rumor = 0; i_rumor < cardinalidade (cj_rumores); i_rumor++) {
        incrementar_iterador (cj_rumores, &rumor); 
        iniciar_iterador (local->pessoa_t); 
        for (i_pessoa = 0; i_pessoa < cardinalidade (local->pessoa_t); i_pessoa++) {
            incrementar_iterador (local->pessoa_t, &id_destinatario);
            if (aleat (0, 100) < mundo->pessoas[id_destinatario].extroversao)  
                if (insere_conjunto (mundo->pessoas[id_destinatario].rumores, rumor))
                    printf("(P%d/R%d) ", mundo->pessoas[id_destinatario].id_pessoa, rumor);
        }
    }
    printf ("\n");
}

void trata_evento_fim_simulacao(mundo_t *mundo, lef_t *lef)
{
    /* Libera memória */
    destroi_mundo(mundo);
    destroi_lef(lef);
}

void destroi_evento_chegada(evento_t *evento)
{
    chegada_t *chegada = (chegada_t *)evento->dados;
    free(chegada);
    free(evento);
}

void destroi_evento_partida(evento_t *evento)
{
    saida_t *partida = (saida_t *)evento->dados;
    free(partida);
    free(evento);
}

void destroi_evento_disseminacao(evento_t *evento)
{
    disseminacao_t *disseminacao = (disseminacao_t *)evento->dados;
    destroi_conjunto(disseminacao->rumores);
    free(disseminacao);
    free(evento);
}

void destroi_evento_fim_simulacao(evento_t *evento)
{
    free(evento);
}

/*
 * ####################################
 *      Criação de eventos
 * ####################################
 */

evento_t *cria_evento_chegada(int id_pessoa, int id_local, int tempo)
{
    evento_t *evento;
    if (!(evento = malloc(sizeof(evento_t))))
        return NULL;

    chegada_t *chegada;
    if (!(chegada = malloc(sizeof(chegada_t))))
        return NULL;

    chegada->id_pessoa = id_pessoa;
    chegada->id_local = id_local;

    evento->dados = chegada;
    
    evento->tipo = CHEGADA;
    evento->tempo = tempo;
    evento->tamanho = sizeof(chegada_t);
    evento->destroidados = (void*)destroi_evento_chegada;

    return evento;
}

evento_t *cria_evento_partida(int id_pessoa, int id_local, int tempo)
{
    evento_t *evento;
    if (!(evento = malloc(sizeof(evento_t))))
        return NULL;

    saida_t *partida;
    if (!(partida = malloc(sizeof(saida_t))))
        return NULL;

    partida->id_pessoa = id_pessoa;
    partida->id_local = id_local;

    evento->dados = partida;

    evento->tipo = PARTIDA;
    evento->tempo = tempo;
    evento->destroidados = (void *)destroi_evento_partida;

    return evento;
}

evento_t *cria_evento_disseminacao(int id_pessoa, int id_local, conjunto_t *cj_rumores_pessoa, int tempo)
{
    evento_t *evento;
    if (!(evento = malloc(sizeof(evento_t))))
        return NULL;

    disseminacao_t *disseminacao;
    if (!(disseminacao = malloc(sizeof(disseminacao_t))))
        return NULL;

    disseminacao->id_pessoa = id_pessoa;
    disseminacao->id_local = id_local;
    disseminacao->rumores = cj_rumores_pessoa;

    evento->dados = disseminacao;

    evento->tipo = DISSEMINACAO;
    evento->tempo = tempo;
    evento->destroidados =(void*) destroi_evento_disseminacao;

    return evento;
}

evento_t *cria_evento_fim_simulacao(int tempo)
{
    evento_t *evento;
    if (!(evento = malloc(sizeof(evento_t))))
        return NULL;

    evento->tipo = FIM_SIMULACAO;
    evento->tempo = tempo;
    evento->dados = NULL;
    evento->tamanho = 0;
    evento->destroidados = (void *)destroi_evento_fim_simulacao;

    return evento;
}


/* Print e afins para evento */

void saida_chegada (mundo_t *mundo, pessoa_t *pessoa, local_t *local, int caso) {
    switch (caso) {
        case 1: { /* caso de entrada */
            printf ("%6d:CHEGA Pessoa %4d Local %2d (%2d/%2d), ENTRA\n", mundo->tempo_atual, pessoa->id_pessoa, local->id_local, cardinalidade (local->pessoa_t), local->capacidade);
            break;
        }
        case 2: { /* caso de ida para fila */
            printf ("%6d:CHEGA Pessoa %4d Local %2d (%2d/%2d), FILA %2d\n", mundo->tempo_atual, pessoa->id_pessoa, local->id_local, cardinalidade (local->pessoa_t), local->capacidade, tamanho_fila(local->fila_t));
            break;
        }
        case 3: { /* caso de desistencia */
            printf ("%6d:CHEGA Pessoa %4d Local %2d (%2d/%2d), DESISTE\n", mundo->tempo_atual, pessoa->id_pessoa, local->id_local, cardinalidade (local->pessoa_t), local->capacidade);
            break;
        }
    }
}

/* trata a saida do evento de saida */
void saida_saida (mundo_t *mundo, pessoa_t * pessoa_saida, int pessoa_fila, local_t *local, int caso) {
    switch (caso) {
        case 1: { /* caso de saida sem fila */
            printf ("%6d:SAIDA Pessoa %4d Local %2d (%2d/%2d)\n", mundo->tempo_atual, pessoa_saida->id_pessoa, local->id_local, local->capacidade, local->capacidade);
            break;
        }
        case 2: { /* caso de saida no espaço na fila */
            printf ("%6d:SAIDA Pessoa %4d Local %2d (%2d/%2d), REMOVE FILA %2d\n", mundo->tempo_atual, pessoa_saida->id_pessoa, local->id_local, local->capacidade, local->capacidade, pessoa_fila);
            break;
        }
    }
}
