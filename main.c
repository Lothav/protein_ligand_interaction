/* 
 * File:   main.c
 * Author: LuizOtavio
 *
 * Created on 21 de Novembro de 2015, 01:27
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
    char name[11];
    int sum;
    struct lig_st *prox;
} Ligante;

typedef struct ligante_list_st{

    Ligante *header;
   
}LiganteList;

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
    sscanf(str, " %s %s %lf %lf %lf", aux, aux, 
	&new_lig.point[0], &new_lig.point[1], &new_lig.point[2]);
    // retorna o novo ligante para que seja
    // comparado na árvore.
    return new_lig;
}

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

/*
             *------------*
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


void splitCubes(Leaf* leaf, double half_edge[]) {

    // auxiliar
    int c,a;
    
    // Transformar a Folha em Nó :
    
    Cube lower, upper;
    /*
	Percebi que a partir do cubo extremo de cima
	podemos chegar nos 3 vizinhos(valor max e min) dele 
	variando o x,y,z um de cada vez.
     
	Chamei ele de 'uper';
 
     
             *------------*
	    /     ///////||
	  / - - / - - /||||
	/     /     / |||||
      *-----------*   | / |
      |     |     |   |   /
      |     |     | / |   *
      | - - - - - |   | /
      |     |     |   /
      |     |     | /
      *-----------*
     
      
     
	Mesma coisa para o extremo de baixo
	Chamei ele de 'lower';
 
     
             *------------*
	    /     /    /  |
	  / - - / - - /   |
	/     /     / |   |
      *-----------*   | / |
      |     |     |   |   /
      |     |     | / |   *
      | - - - - - |   | /
      |||||||     |   /
      |||||||     | /
      *-----------*
}*/
    
    // Vamos à lógica:
    
    
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

void freeTree(Leaf *root) {
    int a;
    // Função recursiva que limpa toda a árvore filho por filho.
    for (a = 0; a < 8; a++) {
        if (root->sons[a] != NULL) {
            freeTree(root->sons[a]);
        }
    }
    free(root);
}

void setLeafProtein(Leaf* leaf, Protein new_protein) {

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
	setLeafProtein(new_leaf, protein);
	setLeafProtein(old_leaf, new_protein);
    }
}

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
	    if (    leaf->protein.point[0] < lig_cube.min[0] ||
		    leaf->protein.point[1] < lig_cube.min[1] ||
		    leaf->protein.point[2] < lig_cube.min[2] ||

		    leaf->protein.point[0] > lig_cube.max[0] ||
		    leaf->protein.point[1] > lig_cube.max[1] ||
		    leaf->protein.point[2] > lig_cube.max[2])
		return;
            else (*sum)++;
	}
    } else {
	for (a = 0; a < 8; a++) {
	    if (    leaf->sons[a]->coords.max[0] < lig_cube.min[0] ||
		    leaf->sons[a]->coords.max[1] < lig_cube.min[1] ||
		    leaf->sons[a]->coords.max[2] < lig_cube.min[2] ||

		    leaf->sons[a]->coords.min[0] > lig_cube.max[0] ||
		    leaf->sons[a]->coords.min[1] > lig_cube.max[1] ||
		    leaf->sons[a]->coords.min[2] > lig_cube.max[2])
		continue;
                else getPointsInsideBox(leaf->sons[a], lig, cubeLig_edge, sum);
	}
    }
}

void insertOnMiddle(Ligante* new_ligante, Ligante* aux){
    new_ligante->prox = aux->prox;
    aux->prox = new_ligante;
}

void putLiganteOnLiganteList(Ligante new_ligante, LiganteList* ligant_list){
    
    Ligante *aux, *new;
    
    aux = ligant_list->header;
    new = NULL;
    new = (Ligante *) malloc( sizeof(Ligante) );
    strcpy(new->name, new_ligante.name);
    new->sum = new_ligante.sum;
    new->prox = NULL;
    
    while(aux->prox != NULL){
	if(aux->prox->sum >= new->sum){
	    insertOnMiddle(new, aux);
	    break;
	}
	aux = aux->prox;
    }
    if(aux->prox == NULL) {
	aux->prox = new;
	aux->prox->prox = NULL;
    }
}


int main(int argc, char** argv) {

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
    
    ligant_list = NULL;
    ligant_list = (LiganteList *) malloc( sizeof(LiganteList) );
    ligant_list->header = NULL;
    ligant_list->header = (Ligante *) malloc( sizeof(Ligante) );
    ligant_list->header->prox = NULL;

    while (aux[0] != '-' && aux[1] != '1') {

	root = NULL;
	root = (Leaf *) malloc(sizeof (Leaf)*2);
	root->is_leaf = 1;
	root->protein.isSet = 0;
        
        for(a = 0; a< 8; a++){
            root->sons[a] = NULL;
        }

	sum = 0;
	p_sum = &sum;
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

	while (str[0] == 'L') {

	    // agora recebemos os ligantes:
	    new_ligante = getNewLigante(str);
	    strcpy(new_ligante.name, lig_name);
	    getPointsInsideBox(root, new_ligante, cubeLig_edge, p_sum);
	    new_ligante.sum = sum;
	    fgets(str, sizeof (str), stdin);
	}

	putLiganteOnLiganteList(new_ligante, ligant_list);
	
	freeTree(root);
	sscanf(str, " %s %s", aux, lig_name);
    }
    
    Ligante *print, *print_aux;
    print = ligant_list->header->prox;
    print_aux = NULL;
    
    while(1){
	printf("%s: %d\n", print->name, print->sum);
	print_aux = print;
	print = print->prox;
	free(print_aux);
	if(print == NULL) break;	
    }
    
    free(ligant_list);
    free(ligant_list->header);
    
    return (EXIT_SUCCESS);
}