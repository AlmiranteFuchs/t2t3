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
    if (!(evento_fim = cria_evento_fim_simulacao(2000)))
        return 0;
    
    /* Insere na lef */
    if (!(adiciona_inicio_lef(lef, evento_fim)))
        return 0;

    chegada_t *dados_chegada;   /* ponteiro que vai receber os dados de chegada */
    saida_t *dados_saida;       /* ponteiro que vai receber os dados de saida */
    disseminacao_t *dados_diss; /* ponteiro que vai receber os dados de disceminacao */


    mundo_t *mundo;
    if (!(mundo = cria_mundo(20000, 30, 100, 8, lef)))
        return 0;

   
    evento_t *evento_atual;
    while ((evento_atual = obtem_primeiro_lef(lef)))
    {
        mundo->tempo_atual = evento_atual->tempo;
        switch (evento_atual->tipo)
        {
        case CHEGADA:
        {
            dados_chegada = (chegada_t *)evento_atual->dados;
            trata_evento_chegada(dados_chegada->id_pessoa, dados_chegada->id_local, mundo, lef);
            free(dados_chegada);
            free(evento_atual);
            break;
        }

        case PARTIDA:
        {
            dados_saida = (saida_t *)evento_atual->dados;
            trata_evento_partida(dados_saida->id_pessoa, dados_saida->id_local, mundo, lef);
            free(dados_saida);
            free(evento_atual);
            break;
        }
        case DISSEMINACAO:
        {
            dados_diss = (disseminacao_t *)evento_atual->dados;
            trata_evento_disseminacao(dados_diss->id_pessoa, dados_diss->id_local, dados_diss->rumores, mundo, lef);
            free(dados_diss);
            free(evento_atual);
            break;
        }

        case FIM_SIMULACAO:
            printf("\nFim da simulacao ;)\n");
            trata_evento_fim_simulacao(mundo, lef);
            return;
            break;
        }

        /* Da free no evento */
        /* if (evento_atual->destroidados != NULL)
            evento_atual->destroidados(evento_atual->dados);
        free(evento_atual); */
    }
    return 1;
}
/* Não roda, dado o tempo para a dimensão do trabalho */
/* Diversos erros de reimplementação de biblioteca, dada que não estruturei com calma qual lib faz exatamente oq separada das outras */
/* Favor considerar lógica invés de puramente compilação */