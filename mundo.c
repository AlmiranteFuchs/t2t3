#include <stdio.h>
#include <stdlib.h>
#include "libconjunto.h"

int main(){
    /*
     *  Teste busca binária e ordenação
     */
    /*
    int* aux = malloc(sizeof(int) * 5);
    int i;
    for (i = 0; i < 4; i++)
    {
        int atual;
        printf("\nInsira: ");
        scanf("%d", &atual);
        aux[i] = atual;
    }

    int atual;
    printf("\nInsira pra inserir: ");
    scanf("%d", &atual);

    insere_ordenado(aux, 4, atual);
    for (i = 0; i < 5; i++)
    {
        printf("\nValor vetor em [%d]: %d", i, aux[i]);
    }

    printf("Insira qual quer procurar: ");
    scanf("%d", &atual);

    int existe = busca_binaria_recursiva(aux, 0, 4, atual);
    printf("Existe? %d", existe);
    return 1;
    */
    /*
     *  Teste busca binária e ordenação fim
     */


    /*
     * @brief 
     * Teste libconjunto
     */

    conjunto_t *novo_c = cria_conjunto(5);

    /* Insere e testa cheio */
    int i;
    for (i = 0; i < 4; i++)
    {
        int atual;
        printf("\nInsira: ");
        scanf("%d", &atual);
        insere_conjunto(novo_c, atual);
    }
    /*
    printf("Conjunto: \n");
    imprime(novo_c);
    int retirado = retirar_um_elemento(novo_c);
    printf("\nRetirado indice: %d\n", retirado);
    imprime(novo_c);
    */

    /*
       imprime(novo_c);
       int result2 = redimensiona(novo_c);
       printf("\n redimensiona= %d\n", result2);
       imprime(novo_c);

    */

   
    /*
     * @brief
     * Teste libconjunto FIM
     */

    return 1;
}