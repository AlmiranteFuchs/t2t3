#include <stdio.h>
#include <stdlib.h>
#include "libconjunto.h"

int main(){
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
}