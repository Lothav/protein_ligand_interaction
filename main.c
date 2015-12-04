/* 
 * File:   main.c
 * Author: LuizOtavio
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#include "ligante.h"
#include "protein.h"
#include "cube.h"
#include "leaf.h"
#include "utils.h"


/*
 
 *		INTERAÇÃO LIGANTE PROTEÍNA
 * 
 *	UFMG 2015/2
 *	TP AEDS II
 * profs.: RAQUEL MINARDI / GISELE PAPPA
 * monitor:  PAULO BICALHO
 * 
 */

int main(int argc, char** argv) {

    // comprimento da aresta do cubo do ligante
    double cubeLig_edge;
    char str[MAX], lig_name[MAX], aux[11];
    Leaf *root, *leaf;
    int sum, *p_sum, a;
    Protein new_protein;
    Ligante new_ligante;
    LiganteList *ligant_list;

    // armazena o valor da aresta do cubo em volta de cada ligante
    fgets(str, sizeof (str), stdin);
    sscanf(str, " %lf", &cubeLig_edge);

    fgets(str, sizeof (str), stdin);
    sscanf(str, " %s %s", aux, lig_name);
    
    // declara a lista de Ligantes que será utilizada no final
    // para exibir na tela de forma ordenada.
    ligant_list = NULL;
    ligant_list = (LiganteList *) malloc( sizeof(LiganteList) );
    ligant_list->header = NULL;
    ligant_list->header = (Ligante *) malloc( sizeof(Ligante) );
    ligant_list->header->prox = NULL;

    
    // Enquanto não for inserido '-1'
    while (aux[0] != '-' && aux[1] != '1') {

	// declara uma nova raiz para a árvore
	root = NULL;
	root = (Leaf *) malloc(sizeof (Leaf)*2);
	root->is_leaf = 1;
	root->protein.isSet = 0;
        for(a = 0; a< 8; a++){
            root->sons[a] = NULL;
        }

	//seta a soma das interações do ligante em 0
	sum = 0;
	p_sum = &sum;
	// lê e insere no cubo alocado as coordenadas
	// de seus pontos extremos na raiz.
	setCubeCoords(root);

	fgets(str, sizeof (str), stdin);
	
	
	// ---- Leitura das Proteínas
	
	while (str[0] == 'P') {
	    // cria uma nova proteína:
	    //  - aloca espaço para ela
	    //  - recebe as coordenadas do stdin
	    new_protein = getNewProtein(str);
	    // percorre a raiz e encontra a folha
	    // referente às coordenadas da nova proteína.
	    leaf = findLeaf(root, new_protein.point);

	    // função recursiva que insere a proteína
	    // na folha escolhida.
	    setProtein(leaf, new_protein);
	    fgets(str, sizeof (str), stdin);
	}
	
	// ---- Leitura dos Ligantes

	while (str[0] == 'L') {

	    // agora recebemos os ligantes:
	    new_ligante = getNewLigante(str);
	    // copia o nome do ligante para ele.
	    strcpy(new_ligante.name, lig_name);
	    // função recursiva para somar as iterações.
	    getPointsInsideBox(root, new_ligante, cubeLig_edge, p_sum);
	    // recebe a soma final
	    new_ligante.sum = sum;
	    
	    fgets(str, sizeof (str), stdin);
	}
	
	// Insere de forma ordenada o ligante na lista.
	putLiganteOnLiganteList(new_ligante, ligant_list);
	
	// Limpa a árvore e todos seus filhos
	freeLeafs(root);
	sscanf(str, " %s %s", aux, lig_name);
    }
    
    //  ---- Resultado no STDOUT
    
    printResult(ligant_list);
    
    return (EXIT_SUCCESS);
}