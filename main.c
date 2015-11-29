/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: LuizOtavio
 *
 * Created on 21 de Novembro de 2015, 01:27
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 500

typedef struct prot_st {
    // point[] refere-se à cordenada da da Proteína:
    // Sendo:
    //  - point[0] = x
    //  - point[1] = y
    //  - point[2] = z
    double point[3];
} Protein;

typedef struct cube_str {
    // max/min referem-se às cordenadas de um certo cubo:
    // Sendo:
    //  - max/min [0] = x
    //  - max/min [1] = y
    //  - max/min [2] = z
    double max[3];
    double min[3];
} Cube;

//typedef struct Leaf* Pointer;

typedef struct leaf_st {
    // boolena que verifica se a folha é de fato
    // uma folha ou um nó (se == 1 -> folha)
    int is_leaf;
    // ponteiro que aponta para essa struct
    // ou seja, dentro de Leaf, temos um 
    // "array de Leaf" com 8 filhos.
    struct leaf_st* sons[8];
    // cada folha também terá uma proteína.
    Protein* protein;
    // cada folha também terá coordenadas de máx e mín
    // para saber-mos onde procurar por determinada proteína.
    Cube* coords;
} Leaf;

void setCubeCoords(Leaf *leaf) {

    char str[MAX];

    // seta o valor MÍNIMO de x,y,z para o cubo
    fgets(str, sizeof (str), stdin);
    sscanf(str, " %lf %lf %lf", &leaf->coords->min[0], &leaf->coords->min[1], &leaf->coords->min[2]);

    // seta o valor MÁXIMO de x,y,z para o cubo
    fgets(str, sizeof (str), stdin);
    sscanf(str, " %lf %lf %lf", &leaf->coords->max[0], &leaf->coords->max[1], &leaf->coords->max[2]);
}

Protein* getNewProtein(Leaf *leaf) {

    Protein *new_prot;
    char str[MAX], aux[MAX];

    // aloca espaço para nova proteína
    new_prot = (Protein *) malloc(sizeof (Protein));

    // lê a linha com os dados da nova proteína
    fgets(str, sizeof (str), stdin);
    // armazena os pontos da proteína
    // (o aux é apenas para ignorar os nomes
    // tenho sérios problemas com regex)
    sscanf(str, " %s %s %lf %lf %lf", aux, aux, &new_prot->point[0], &new_prot->point[1], &new_prot->point[2]);
    // retorna a nova proteína para que seja
    // inserida na árvore.
    return new_prot;
}

Leaf* findLeaf(Leaf* root, double point[3]) {

    int c;
    Leaf *aux;

    aux = root;
    for (c = 0; c < 8; c++) {
	if (aux != NULL &&
		point[0] < aux->coords->max[0] &&
		point[1] < aux->coords->max[1] &&
		point[2] < aux->coords->max[2] &&

		point[0] > aux->coords->min[0] &&
		point[1] > aux->coords->min[1] &&
		point[2] > aux->coords->min[2]) {

	    if (aux->is_leaf) return aux;
	    else findLeaf(aux, point);
	}
	aux = aux->sons[c];
    }
}

int main(int argc, char** argv) {

    double cubeLig_edge;
    char str[MAX], lig_name[MAX], aux[11], type[30];
    Leaf *root, *leaf;
    int a;
    Protein* new_protein;

    root = (Leaf *) malloc(sizeof (Leaf));
    root->protein = NULL;
    root->is_leaf = 1;
    root->coords = (Cube *) malloc( sizeof(Cube) );

    // armazena o valor da aresta do cubo em volta de cada ligante
    fgets(str, sizeof (str), stdin);
    sscanf(str, " %lf", &cubeLig_edge);

    // recebe o nome do ligante e o armazena em
    // 'lig_name'. O aux é apenas para receber o '-1'
    fgets(str, sizeof (str), stdin);
    sscanf(str, " %s %s", aux, lig_name);

    // enquanto a linha 'Nome: nome_lig' 
    // não receber -1 faz:
    while (aux[0] != '-' && aux[1] != '1') {
	
	// lê e insere no cubo alocado as coordenadas
	// de seus pontos extremos na raiz.
	setCubeCoords(root);


	// cria uma nova proteína:
	//  - aloca espaço para ela
	//  - recebe as coordenadas do stdin
	new_protein = getNewProtein(root);
	// percorre a raiz e encontra a folha
	// referente às coordenadas da nova proteína.
	leaf = findLeaf(root, new_protein->point);
	leaf->protein = new_protein;
	leaf->is_leaf = 1;

	fgets(str, sizeof (str), stdin);
	sscanf(str, " %s %", &type);
	
    }

    return (EXIT_SUCCESS);
}

