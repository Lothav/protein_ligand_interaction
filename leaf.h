/* 
 * File:   leaf.h
 * Author: root
 *
 * Created on 4 de Dezembro de 2015, 03:53
 */




#ifndef LEAF_H
#define LEAF_H
#define MAX 500


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


// Encontra a folha recursivamente com as coordenadas
// em que se encontra o 'point'.
Leaf* findLeaf(Leaf* root, double* point);

// Limpa recursivamente todas às folhas da árvore.
void freeLeafs(Leaf *root);

// Insere a proteína na folha.
// Porém verifica antes se a folha já contém uma proteína.
// Se sim, transforma a folha em um Nó e encontra as novas folhas
// para as proteínas de forma recursiva.
void setProtein(Leaf* leaf, Protein new_protein);


// Recebe no STDIN as coordenadas do um novo cubo
// e insere nele.
void setCubeCoords(Leaf *leaf);

    
    /*
	Percebi que a partir do cubo extremo de cima
	podemos chegar nos 3 vizinhos(valor max e min) dele 
	variando o x,y,z um de cada vez.
     
	Chamei ele de 'uper';
 
     
             *------------*
	    /     ///////||
	  / - - / - - /||||
	/     /     / |||||
      *-----------*   ||/ |
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
    */
void splitCubes(Leaf* leaf, double half_edge[]);

#endif /* LEAF_H */

