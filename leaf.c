#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ligante.h"
#include "protein.h"
#include "cube.h"
#include "leaf.h"
#include "utils.h"



Leaf* findLeaf(Leaf* root, double* point) {

    int c;

    // Função Recursiva:
    // procura na arvore o filho com
    // as coordenadas passadas no 'point'.
    // ao encontrar, se for uma folha, retorna
    // para que seja inserida, se for um nó, 
    // procura entre seus filhos.
    if (root->is_leaf) {
	return root;
    }
    c = 0;
    for (c = 0; c < 8; c++) {
	if (root->sons[c] != NULL &&
		point[0] <= root->sons[c]->coords.max[0] &&
		point[1] <= root->sons[c]->coords.max[1] &&
		point[2] <= root->sons[c]->coords.max[2] &&

		point[0] >= root->sons[c]->coords.min[0] &&
		point[1] >= root->sons[c]->coords.min[1] &&
		point[2] >= root->sons[c]->coords.min[2]) {

	    return findLeaf(root->sons[c], point);
	}
    }
}

void freeLeafs(Leaf *root) {
    int a;
    // Função recursiva que limpa toda a árvore filho por filho.
    for (a = 0; a < 8; a++) {
        if (root->sons[a] != NULL) {
            freeLeafs(root->sons[a]);
        }
    }
    free(root);
}

void setProtein(Leaf* leaf, Protein new_protein) {

    double half_edge[3];
    Protein protein;
    Leaf *new_leaf, *old_leaf;
    int a;

    // se a proteína da folha escolhida não esta setada
    // e é uma folha (não é um nó)
    
    
    old_leaf = NULL;
    new_leaf = NULL; 
    if (!leaf->protein.isSet && leaf->is_leaf) {
	leaf->protein = new_protein;
	leaf->protein.isSet = 1;
    } else {
	// senão transforma a folha em nó :

	// calcula a metade do comprimento em x, y, z da aresta do cubo
	for(a = 0; a< 3; a++) half_edge[a] = fabs(leaf->coords.max[a] - leaf->coords.min[a]) / 2;

	// guarda a proteína que está na folha
	// para 'descermos' com ela após ramificar-mos
	// os filhos.
	protein = leaf->protein;
	//divide a folha em sub-folhas
	// 'sons[]':
	if (leaf->is_leaf && leaf->protein.isSet) {
	    splitCubes(leaf, half_edge);
	    leaf->is_leaf = 0;
	}
	// fala que a folha agora é um nó.

	// diz que a proteína da folha não está setada
	leaf->protein.isSet = 0;

	// 'desce' com a folha guardada para um
	// dos filhos criados.
	new_leaf = findLeaf(leaf, protein.point);
	old_leaf = findLeaf(leaf, new_protein.point);

	
	// caso ambas às proteínas encontrem a mesma folha
	// deve-se ramificá-las mais uma vez:
	while (new_leaf == old_leaf) {
	    
	    // calcula novamente cada areasta para o novo cubo
	    for(a = 0; a< 3; a++) half_edge[a] = fabs(new_leaf->coords.max[a] - new_leaf->coords.min[a]) / 2;	    
	    
	    // divite então o cubo
            splitCubes(new_leaf, half_edge);
            new_leaf->is_leaf = 0;
            new_leaf->protein.isSet = 0;
	    //encontra as novas folhas
            old_leaf = findLeaf(new_leaf, new_protein.point);
            new_leaf = findLeaf(new_leaf, protein.point);
	    //caso as novas folhas forem iguais, essa operação se repetirá
	}

	//e então insere as proteínas nas suas 
	// repectivas folhas.
	setProtein(new_leaf, protein);
	setProtein(old_leaf, new_protein);
    }
}

    void setCubeCoords(Leaf *leaf) {

    char str[MAX];

    // seta o valor MÍNIMO de x,y,z para o cubo
    fgets(str, sizeof (str), stdin);
    sscanf(str, " %lf %lf %lf", &leaf->coords.min[0], &leaf->coords.min[1], &leaf->coords.min[2]);

    // seta o valor MÁXIMO de x,y,z para o cubo
    fgets(str, sizeof (str), stdin);
    sscanf(str, " %lf %lf %lf", &leaf->coords.max[0], &leaf->coords.max[1], &leaf->coords.max[2]);
}

void splitCubes(Leaf* leaf, double half_edge[]) {

    // auxiliar
    int c,a;
    
    // Transformar a Folha em Nó :
    
    Cube lower, upper;
    
    // half_edge é metade da aresta do cubo naquela
    // posição (x/y/z.
    // Com isso temos as coordenadas max e min de
    // lower e upper. (note que upper.min == lower.max)
    for (c = 0; c < 3; c++) {
	lower.min[c] = leaf->coords.min[c];
	lower.max[c] = leaf->coords.max[c] - half_edge[c];

	upper.max[c] = leaf->coords.max[c];
	upper.min[c] = leaf->coords.min[c] + half_edge[c];
    }

    // Aloca memória para todos os filhos da folha
    // e transforma ela em um NÓ.
    for (c = 0; c < 4; c++) {
	leaf->sons[c] = NULL;
	leaf->sons[c] = (Leaf *) malloc(sizeof (Leaf)*2);
	// Seta todos os cubos que serão vizinhos de lower
	// com suas coordenadas.
	leaf->sons[c]->coords = lower;
	leaf->sons[c]->is_leaf = 1;
	leaf->sons[c]->protein.isSet = 0;
        for(a = 0; a< 8; a++){
            leaf->sons[c]->sons[a] = NULL;
        }
    }

    for (c = 4; c < 8; c++) {
	leaf->sons[c] = NULL;
	leaf->sons[c] = (Leaf *) malloc(sizeof (Leaf)*2);
	// Seta todos os cubos que serão vizinhos de upper
	// com suas coordenadas.
	leaf->sons[c]->coords = upper;
	leaf->sons[c]->is_leaf = 1;
	leaf->sons[c]->protein.isSet = 0;
        for(a = 0; a < 8; a++){
            leaf->sons[c]->sons[a] = NULL;
        }
    }

    
    // Então variando uma coordenada(x || y || z) de
    // max e min por filho temos o max e min dos cubos
    // adjacentes à lower e upper.
    
    //lembrando que [0] = X , [1] = Y , [2] = Z
    for (c = 0; c < 4; c++) {
	leaf->sons[c + 1]->coords.max[c] += half_edge[c];
	leaf->sons[c + 1]->coords.min[c] += half_edge[c];
    }
    for (c = 0; c < 4; c++) {
	leaf->sons[c + 4]->coords.max[c] -= half_edge[c];
	leaf->sons[c + 4]->coords.min[c] -= half_edge[c];
    }
}