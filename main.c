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
    int isSet;
} Protein;

typedef struct lig_st {
    // point[] refere-se à cordenada do Ligante:
    // Sendo:
    //  - point[0] = x
    //  - point[1] = y
    //  - point[2] = z
    double point[3];
} Ligante;

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
    Protein protein;
    // cada folha também terá coordenadas de máx e mín
    // para saber-mos onde procurar por determinada proteína.
    Cube coords;
} Leaf;

void setCubeCoords(Leaf *leaf) {

    char str[MAX];

    // seta o valor MÍNIMO de x,y,z para o cubo
    fgets(str, sizeof (str), stdin);
    sscanf(str, " %lf %lf %lf", &leaf->coords.min[0], &leaf->coords.min[1], &leaf->coords.min[2]);

    // seta o valor MÁXIMO de x,y,z para o cubo
    fgets(str, sizeof (str), stdin);
    sscanf(str, " %lf %lf %lf", &leaf->coords.max[0], &leaf->coords.max[1], &leaf->coords.max[2]);
}

Protein getNewProtein(char* str) {

    Protein new_prot;
    char aux[MAX];

    // armazena os pontos da proteína
    // (o aux é apenas para ignorar os nomes
    // tenho sérios problemas com regex)
    sscanf(str, " %s %s %lf %lf %lf", aux, aux, &new_prot.point[0], &new_prot.point[1], &new_prot.point[2]);
    // retorna a nova proteína para que seja
    // inserida na árvore.
    new_prot.isSet = 1;
    return new_prot;
}

Ligante getNewLigante(char* str) {

    Ligante new_lig;
    char aux[MAX];

    // armazena os pontos do ligante
    // (o aux é apenas para ignorar os nomes
    // tenho sérios problemas com regex)
    sscanf(str, " %s %s %lf %lf %lf", aux, aux, &new_lig.point[0], &new_lig.point[1], &new_lig.point[2]);
    // retorna o novo ligante para que seja
    // comparado na árvore.
    return new_lig;
}

Leaf* findLeaf(Leaf* root, double* point) {

    int c;
    Leaf *aux;

    // Função Recursiva:
    // procura na arvore o filho com
    // as coordenadas passadas no 'point'.
    // ao encontrar, se for uma folha, retorna
    // para que seja inserida, se for um nó, 
    // procura entre seus filhos.
    if(root->is_leaf){
	return root;
    }
    aux = root;
    for (c = 0; c < 8; c++) {
	if (aux->sons[c] != NULL &&
		point[0] <= aux->sons[c]->coords.max[0] &&
		point[1] <= aux->sons[c]->coords.max[1] &&
		point[2] <= aux->sons[c]->coords.max[2] &&
		
		point[0] >= aux->sons[c]->coords.min[0] &&
		point[1] >= aux->sons[c]->coords.min[1] &&
		point[2] >= aux->sons[c]->coords.min[2] ) {

	    if (aux->sons[c]->is_leaf) return aux->sons[c];
	    else return findLeaf(aux->sons[c], point);
	}
    }
}

/*
              *-----------*
            /     /     / |
          / - - / - - /   |
        /     /     / |   |
      *-----------*   | / |
      |     |     |   |   /
      |     |     | / |   *
      | - - - - - |   | /
      |     |     |   /
      |     |     | /
      *-----------*
}*/


void splitCubes(Leaf* leaf, double* half_edge) {

    // auxiliar
    int c;
    Cube lower, upper;
   
    for(c = 0; c < 3; c++){
        lower.min[c] = leaf->coords.min[c];
        lower.max[c] = leaf->coords.max[c] - half_edge[c];
        
        upper.max[c] = leaf->coords.max[c];
        upper.min[c] = leaf->coords.min[c] + half_edge[c];
    }
    
    // Aloca memória para todos os filhos da folha
    // e transforma ela em um NÓ.
    for(c = 0; c < 4; c++){
        leaf->sons[c] = (Leaf *) malloc(sizeof (Leaf));
        leaf->sons[c]->coords = lower;
        leaf->sons[c]->is_leaf = 1;
	leaf->sons[c]->protein.isSet = 0;
    }
    
    for(c = 4; c < 8; c++){
        leaf->sons[c] = (Leaf *) malloc(sizeof (Leaf));
        leaf->sons[c]->coords = upper;  
        leaf->sons[c]->is_leaf = 1;
	leaf->sons[c]->protein.isSet = 0;
    }
    
    for(c = 0; c < 4; c++){
        leaf->sons[c+1]->coords.max[c] += half_edge[c];
        leaf->sons[c+1]->coords.min[c] += half_edge[c];
    }
    
    for(c = 0; c < 4; c++){
        leaf->sons[c+4]->coords.max[c] -= half_edge[c];
        leaf->sons[c+4]->coords.min[c] -= half_edge[c];
    }
}


void setLeafProtein(Leaf* leaf, Protein new_protein) {

    double half_edge[3];
    Protein protein;
    Leaf *new_leaf, *old_leaf;

    // se a proteína da folha escolhida não esta setada
    // e é uma folha (não é um nó)
    if (!leaf->protein.isSet && leaf->is_leaf) {
	leaf->protein = new_protein;
	leaf->protein.isSet = 1;
    } else {
	// senão transforma a folha em nó :
	
	half_edge[0] = (leaf->coords.max[0] - leaf->coords.min[0]) / 2;
	half_edge[1] = (leaf->coords.max[1] - leaf->coords.min[1]) / 2;
	half_edge[2] = (leaf->coords.max[2] - leaf->coords.min[2]) / 2;

	// guarda a proteína que está na folha
	// para 'descermos' com ela após ramificar-mos
	// os filhos.
	protein = leaf->protein;
	// fala que a folha agora é um nó.
        leaf->is_leaf = 0;
	// diz que a proteína da folha não está setada
	leaf->protein.isSet = 0;
	
	//divide a folha em sub-folhas
	// 'sons[]':
        if(leaf->sons[0] == NULL){
            splitCubes(leaf, half_edge);
        }
        
	// 'desce' com a folha guardada para um
	// dos filhos criados.
	new_leaf = findLeaf(leaf, protein.point);
	setLeafProtein(new_leaf, protein);
        
	// insere a nova folha.
        old_leaf = findLeaf(leaf, new_protein.point);
	setLeafProtein(old_leaf, new_protein);
    }
}


void getPointsInsideBox(Leaf* leaf, Ligante lig, double cubeLig_edge, int *sum){
    
    // auxiliar
    int a;
    Cube lig_cube;
  
    for(a = 0; a < 4; a++){
        lig_cube.max[a] = lig.point[a] + cubeLig_edge/2;
        lig_cube.min[a] = lig.point[a] - cubeLig_edge/2;
    }

    if(leaf->is_leaf){
        if(leaf->protein.isSet){
            if(leaf->coords.min[0] > lig_cube.min[0] && 
               leaf->coords.min[1] > lig_cube.min[1] &&
               leaf->coords.min[2] > lig_cube.min[2] &&
                    
               leaf->coords.max[0] < lig_cube.max[0] && 
               leaf->coords.max[1] < lig_cube.max[1] &&
               leaf->coords.max[2] < lig_cube.max[2] )
                (*sum)++;
        }
    } else {
        for (a = 0; a < 8; a++) {
            if (leaf->sons[a]->coords.min[0] > lig_cube.min[a] && 
                leaf->sons[a]->coords.min[1] > lig_cube.min[a] && 
                leaf->sons[a]->coords.min[2] > lig_cube.min[a] &&
                    
                leaf->sons[a]->coords.max[0] < lig_cube.max[a] &&
                leaf->sons[a]->coords.max[1] < lig_cube.max[a] && 
                leaf->sons[a]->coords.max[2] < lig_cube.max[a])
                    getPointsInsideBox(leaf->sons[a], lig, cubeLig_edge, sum);
        }
    }
}

int main(int argc, char** argv) {

    double cubeLig_edge;
    char str[MAX], lig_name[MAX], aux[11];
    Leaf *root, *leaf;
    int sum, *p_sum;
    Protein new_protein;
    Ligante new_ligante;

    root = (Leaf *) malloc(sizeof (Leaf));
    root->is_leaf = 1;
    root->protein.isSet = 0;

    // armazena o valor da aresta do cubo em volta de cada ligante
    fgets(str, sizeof (str), stdin);
    sscanf(str, " %lf", &cubeLig_edge);

    // enquanto a linha 'Nome: nome_lig' 
    // não receber -1 faz:
    while (aux[0] != '-' && aux[1] != '1') {

        sum = 0;
        p_sum = &sum;
        
        
	// recebe o nome do ligante e o armazena em
	// 'lig_name'.
	fgets(str, sizeof (str), stdin);
	sscanf(str, " %s %s", aux, lig_name);

	// lê e insere no cubo alocado as coordenadas
	// de seus pontos extremos na raiz.
	setCubeCoords(root);

	fgets(str, sizeof (str), stdin);
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
	    setLeafProtein(leaf, new_protein);
	    fgets(str, sizeof (str), stdin);
	}
	
	while(str[0] == 'L'){

	    // agora recebemos os ligantes:
	    new_ligante = getNewLigante(str);
	    getPointsInsideBox(root, new_ligante, cubeLig_edge, p_sum);
            
            printf("\n\n Valor de ligante => %i !\n", sum);
            fgets(str, sizeof (str), stdin);
	}
        
        printf("cabo");
        
    }
    return (EXIT_SUCCESS);
}