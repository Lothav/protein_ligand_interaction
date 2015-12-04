/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   protein.h
 * Author: root
 *
 * Created on 4 de Dezembro de 2015, 03:43
 */


#ifndef PROTEIN_H
#define PROTEIN_H

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


// Cria um nova proteína e insere nela as
// coordenadas inseridas no STDIN
Protein getNewProtein(char* str);


#endif /* PROTEIN_H */

