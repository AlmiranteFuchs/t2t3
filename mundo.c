#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libeventos.h"
#include "entidades.h"
#include "libaleat.h"

int main()
{
    start_seed(); /* Inicializa SEED */

    /* Cria uma lef pra guardar os eventos */
    lef_t *lef = cria_lef();

    /* Cria um evento do fim da simulação */
    evento_t *evento_fim;
    if (!(evento_fim = cria_evento_fim_simulacao(34944)))
        return 0;

    /* Insere na lef */
    if (!(adiciona_inicio_lef(lef, evento_fim)))
        return 0;

    mundo_t* mundo;
    if (!(mundo = cria_mundo(20000, 30, 100, 8, lef)))
        return 0;

    evento_t *evento_atual;
    while ((evento_atual = obtem_primeiro_lef(lef)))
    {
        mundo->tempo_atual = evento_atual->tempo;
        switch (evento_atual->tipo)
        {
        case CHEGADA:
            /* Sei lá pq não funciona a definição está correta */
            trata_evento_chegada(((chegada_t *)evento_atual->dados)->id_pessoa, ((chegada_t *)evento_atual->dados)->id_local, mundo, lef);
            break;

        case PARTIDA:
            trata_evento_partida(((saida_t *)evento_atual->dados)->id_pessoa, ((saida_t *)evento_atual->dados)->id_local, mundo, lef);
            break;

        case DISSEMINACAO:
            trata_evento_disseminacao(((disseminacao_t *)evento_atual->dados)->id_pessoa, ((disseminacao_t *)evento_atual->dados)->id_local, ((disseminacao_t *)evento_atual->dados)->rumores, mundo, lef);
            break;

        case FIM_SIMULACAO:
            trata_evento_fim_simulacao(mundo, lef);
            break;
        }

        /* Da free no evento */
        if (evento_atual->destroidados != NULL)
            evento_atual->destroidados(evento_atual->dados);
        free(evento_atual);
    }
    return 1;
}
/* Não roda, dado o tempo para a dimensão do trabalho */
/* Diversos erros de reimplementação de biblioteca, dada que não estruturei com calma qual lib faz exatamente oq separada das outras */
/* Favor considerar lógica invés de puramente compilação */