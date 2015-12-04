#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ligante.h"


Ligante getNewLigante(char* str) {

    Ligante new_lig;
    char aux[MAX];
    
    // armazena os pontos do ligante
    // (o aux é apenas para ignorar os nomes
    // tenho sérios problemas com regex)
    sscanf(str, " %s %s %lf %lf %lf", aux, aux, 
	&new_lig.point[0], &new_lig.point[1], &new_lig.point[2]);
    // retorna o novo ligante para que seja
    // comparado na árvore.
    return new_lig;
}

void insertOnMiddle(Ligante* new_ligante, Ligante* aux){
    // insere no meio da lista de Ligantes
    new_ligante->prox = aux->prox;
    aux->prox = new_ligante;
}

void putLiganteOnLiganteList(Ligante new_ligante, LiganteList* ligant_list){
    
    Ligante *aux, *new;
    
    // Coloca o ligante na lista de ligantes
    // já de forma ordenada.
    
    // aux inicializa no header da lista
    aux = ligant_list->header;
    
    
    // declara um novo ligante
    new = NULL;
    new = (Ligante *) malloc( sizeof(Ligante) );
    
    //copia as informações para o novo ligante
    strcpy(new->name, new_ligante.name);
    new->sum = new_ligante.sum;
    new->prox = NULL;
    
    while(aux->prox != NULL){
	// percorre a lista até encontrar um ligante->prox
	// maior do que o novo.
	// Logo, é aqui que devemos inserir.
	if(aux->prox->sum >= new->sum){
	    insertOnMiddle(new, aux);
	    break;
	}
	aux = aux->prox;
    }
    // caso o loop acima acabe, significa que a inserção
    // deve ser feita no final da lista:
    if(aux->prox == NULL) {
	aux->prox = new;
	aux->prox->prox = NULL;
    }
}