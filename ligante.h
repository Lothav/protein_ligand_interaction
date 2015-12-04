/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ligante.h
 * Author: root
 *
 * Created on 4 de Dezembro de 2015, 03:47
 */



#ifndef LIGANTE_H
#define LIGANTE_H
#define MAX 500

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
    

// Cria um novo ligante e insere nele as
// coordenadas inseridas no STDIN
Ligante getNewLigante(char* str);

// Insere o ligante no meio da lista de ligantes ordenada.
void insertOnMiddle(Ligante* new_ligante, Ligante* aux);

// Insere o ligante na lista de ligantes de forma ordenada.
void putLiganteOnLiganteList(Ligante new_ligante, LiganteList* ligant_list);


#endif /* LIGANTE_H */

