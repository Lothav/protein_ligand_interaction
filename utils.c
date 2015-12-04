#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ligante.h"
#include "protein.h"
#include "cube.h"
#include "leaf.h"
#include "utils.h"


void getPointsInsideBox(Leaf* leaf, Ligante lig, double cubeLig_edge, int *sum) {

    // auxiliar
    int a;
    Cube lig_cube;

    // Função recursiva que soma a quantidade de proteínas
    // dentro do cubo do ligante.
    
    for (a = 0; a < 3; a++) {
	// este loop pega a coordenada do ligante
	// e então calcula x,y,z de seu max e seu min
	lig_cube.max[a] = lig.point[a] + (cubeLig_edge / 2);
	lig_cube.min[a] = lig.point[a] - (cubeLig_edge / 2);
    }

    // se for uma folha
    if (leaf->is_leaf) {
	// e se a proteína estiver setada:
	
	// -> aqui se faz necessário uma condição dentro da outra.
	// Caso não seja uma folha(é um nó), deve-se chamar a recursividade.
	// Porém se for folha e a proteína não estiver setada
	// (folha vazia), nada deve-se fazer.
	if (leaf->protein.isSet) {
	    if (    leaf->protein.point[0] >= lig_cube.min[0] &&
		    leaf->protein.point[1] >= lig_cube.min[1] &&
		    leaf->protein.point[2] >= lig_cube.min[2] &&

		    leaf->protein.point[0] <= lig_cube.max[0] &&
		    leaf->protein.point[1] <= lig_cube.max[1] &&
		    leaf->protein.point[2] <= lig_cube.max[2])
		// Caso o ponto da proteína esteja dentro do
		// cubo do ligante (ou em cima da face/aresta)
		// soma +1 na contagem
            (*sum)++;
	}
    } else {
	// caso seja um nó, procura nos filhos quais têm
	// a coordenada coincidente:
	for (a = 0; a < 8; a++) {
	    if (    leaf->sons[a]->coords.max[0] >= lig_cube.min[0] &&
		    leaf->sons[a]->coords.max[1] >= lig_cube.min[1] &&
		    leaf->sons[a]->coords.max[2] >= lig_cube.min[2] &&

		    leaf->sons[a]->coords.min[0] <= lig_cube.max[0] &&
		    leaf->sons[a]->coords.min[1] <= lig_cube.max[1] &&
		    leaf->sons[a]->coords.min[2] <= lig_cube.max[2])
		// Caso o filho tenha alguma parte coincidente com
		// o cubo do ligante, procura proteína nele.
		getPointsInsideBox(leaf->sons[a], lig, cubeLig_edge, sum);
	}
    }
}


void printResult(LiganteList* ligant_list){

    Ligante *print, *print_aux;
    print = ligant_list->header->prox;
    print_aux = NULL;
    
    // mostra no stdout as interações finais
    // de cada ligante
    while(1){
	printf("%s: %d\n", print->name, print->sum);
	print_aux = print;
	print = print->prox;
	free(print_aux);
	if(print == NULL) break;	
    }
    
    // limpa a lista de ligantes
    free(ligant_list);
    free(ligant_list->header);
}