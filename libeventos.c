#include <stdlib.h>
#include <windef.h>
#include "libmundo.h"
#include "liblef.h"
#include "libaleat.h"
#include "libeventos.h"

/*
 * ####################################
 *      Comportamento de eventos
 * ####################################
 */

void trata_evento_chegada(int id_pessoa, int id_local, mundo_t *mundo, lef_t *lef)
{
    /* Procura local */
    local_t *local;
    if (!(local = procura_local(id_local, mundo->locais)))
        return NULL;

    /* Procura pessoa */
    pessoa_t *pessoa;
    if (!(pessoa = procura_pessoa(id_pessoa, mundo->pessoas)))
        return NULL;

    /* Checa se local está cheio */
    if (local_cheio(local))
    {
        if (pessoa->paciencia / 4 - tamanho_fila(local->fila) > 0)
        {
            /* Pessoa entra na fila */
            if (!(adiciona_pessoa_fila(local->fila, id_pessoa)))
                return NULL;
        }
        else
        {
            /* Pessoa sai do local */
            /* Cria evento de saída */
            evento_t *evento_saida;
            if (!(evento_saida = cria_evento_partida(id_pessoa, id_local, mundo->tempo_atual + 0)))
                return NULL;

            /* Insere na lef */
            if (!(adiciona_ordem_lef(lef, evento_saida)))
                return NULL;
        }
    }
    else
    {
        /* Calcule o tempo de permanência no local */
        int tpl = max(1, pessoa->paciencia / 10 + aleat(-2, 6));

        /* Cria evento de saida */
        evento_t *evento_saida;
        if (!(evento_saida = cria_evento_partida(id_pessoa, id_local, mundo->tempo_atual + tpl)))
            return NULL;

        /* Insere na lef */
        if (!(adiciona_ordem_lef(lef, evento_saida)))
            return NULL;
    }
}

void trata_evento_partida(int id_pessoa, int id_local, mundo_t *mundo, lef_t *lef)
{
    /* Procura local */
    local_t *local;
    if (!(local = procura_local(id_local, mundo->locais)))
        return NULL;

    if (!(fila_vazia(local->fila)))
    {
        /* Cria evento de chegada para id_pessoa na fila em id local */
        int id_pessoa_fila;
        retira_fila(local->fila, &id_pessoa_fila);

        evento_t *evento_chegada;
        if (!(evento_chegada = cria_evento_chegada(id_pessoa_fila, id_local, mundo->tempo_atual + 0)))
            return NULL;

        /* Insere no inicio da lef */
        if (!(adiciona_inicio_lef(lef, evento_chegada)))
            return NULL;
    }

    /* Cria evento de chegada para id_pessoa em outro local */
    int id_local_novo = aleat(0, mundo->nro_locais - 1);

    local_t *local_novo;
    if (!(local_novo = procura_local(id_local_novo, mundo->locais)))
        return NULL;

    pessoa_t *pessoa;
    if (!(procura_pessoa(id_pessoa, mundo->pessoas)))
        return NULL;

    /* Distância cartesiana entre local e novo local */
    int distance = sqrt(pow(local->localizacao.x - local_novo->localizacao.x, 2) + pow(local->localizacao.y - local_novo->localizacao.y, 2));
    int velocity = 100 - max(0, pessoa->idade - 40);
    int TDL = distance / velocity;

    /* Cria evento de chegada */
    evento_t *evento_chegada;
    if (!(evento_chegada = cria_evento_chegada(id_pessoa, id_local_novo, (mundo->tempo_atual + TDL) / 15)))
        return NULL;

    /* Insere na lef */
    if (!(adiciona_ordem_lef(lef, evento_chegada)))
        return NULL;
}

void trata_evento_disseminacao(int id_pessoa, int id_local, conjunto_t *cj_rumores, mundo_t *mundo, lef_t *lef)
{
    /* Busca local */
    local_t *local;
    if (!(local = procura_local(id_local, mundo->locais)))
        return NULL;

    /* Para cada pessoa no local  */
    int i;
    iniciar_iterador(local->pessoas);
    while (i = incrementar_iterador(local->pessoas, &i))
    {
        /* Evita que a mesma pessoa escute seus rumores */
        if (i != id_pessoa && (!(conjunto_vazio(cj_rumores))))
        {
            /* Busca pessoa */
            pessoa_t *pessoa;
            if (!(procura_pessoa(i, mundo->pessoas)))
                return NULL;

            if (aleat(0, 100) < pessoa->extroversao)
            {
                /* ID_RUMOR = retira_evento (CJ_RUMOR) */
                int id_rumor = retirar_um_elemento(cj_rumores);
                /* Insere rumor em pessoa */
                if (!(adiciona_rumor_pessoa(pessoa, id_rumor)))
                    return NULL;
            }
        }
    }
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

    evento->tipo = CHEGADA;
    evento->tempo = tempo;
    if (!(evento->dados = malloc(sizeof(chegada_t))))
        return NULL;
    chegada_t *dados = evento->dados;
    dados->id_pessoa = id_pessoa;
    dados->id_local = id_local;

    /*evento->destroidados = destroi_evento_chegada;*/

    memcpy(evento->dados, dados, sizeof(dados));
    memcpy(evento->destroidados, destroi_evento_chegada, sizeof(destroi_evento_chegada));

    return evento;
}

evento_t *cria_evento_partida(int id_pessoa, int id_local, int tempo)
{
    evento_t *evento;
    if (!(evento = malloc(sizeof(evento_t))))
        return NULL;

    evento->tipo = PARTIDA;
    evento->tempo = tempo;
    if (!(evento->dados = malloc(sizeof(saida_t))))
        return NULL;
    saida_t *dados = evento->dados;
    dados->id_pessoa = id_pessoa;
    dados->id_local = id_local;
    evento->destroidados = destroi_evento_partida;

    return evento;
}

evento_t *cria_evento_disseminacao(int id_pessoa, int id_local, conjunto_t *cj_rumores, int tempo)
{
    evento_t *evento;
    if (!(evento = malloc(sizeof(evento_t))))
        return NULL;

    evento->tipo = DISSEMINACAO;
    evento->tempo = tempo;
    if (!(evento->dados = malloc(sizeof(disseminacao_t))))
        return NULL;
    disseminacao_t *dados = evento->dados;
    dados->id_pessoa = id_pessoa;
    dados->id_local = id_local;
    dados->rumores = cj_rumores;
    evento->destroidados = destroi_evento_disseminacao;

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
    evento->destroidados = destroi_evento_fim_simulacao;

    return evento;
}

evento_t *retirar_evento(lef_t *lef)
{
    evento_t *event = obtem_primeiro_lef(lef);
    /* Era pra ter um remove no .h da liblef mas tudo bem né ;'^  ))) */
    nodo_f_t *aux = lef->Primeiro->prox;

    if (lef->Primeiro->evento->destroidados != NULL)
        lef->Primeiro->evento->destroidados(lef->Primeiro->evento->dados);
    free(lef->Primeiro->evento);
    free(lef->Primeiro);

    lef->Primeiro = aux;

    return event;
}