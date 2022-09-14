#include <stdio.h>
#include <stdlib.h>
#include "libconjunto.h"
#include "libaleat.h"
#include "libmundo.h"
#include "libeventos.h"

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

    mundo_t *mundo = cria_mundo(100, 8, 30, 20000, 20000, lef);

    evento_t *evento_atual;
    while ((evento_atual = retirar_evento(lef)))
    {
        mundo->tempo_atual = evento_atual->tempo;
        switch (evento_atual->tipo)
        {
        case CHEGADA:
            /* Sei lá pq não funciona a definição está correta */
            trata_evento_chegada(evento_atual->dados->id_pessoa, evento_atual->dados->id_local, mundo, lef);
            break;

        case PARTIDA:
            trata_evento_partida(evento_atual->dados->id_pessoa, evento_atual->dados->id_local, mundo, lef);
            break;

        case DISSEMINACAO:
            trata_evento_disseminacao(evento_atual->dados->id_pessoa, evento_atual->dados->id_local, evento_atual->dados->conjunto, mundo, lef);
            break;

        case FIM_SIMULACAO:
            trata_evento_fim_simulacao(mundo, lef);
            break;
        }
    }
}
/* Não roda, dado o tempo para a dimensão do trabalho */
/* Diversos erros de reimplementação de biblioteca, dada que não estruturei com calma qual lib faz exatamente oq separada das outras */
/* Favor considerar lógica invés de puramente compilação */