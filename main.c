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

#define MAX 50

typedef struct cube_str{
    double max_x, max_y, max_z;
    double min_x, min_y, min_z;
    double edge;
}Cube;


void setCubeCoords(Cube *cube){
      
    char str[MAX];
    
    // seta o valor MÍNIMO de x,y,z para o cubo
    fgets(str, sizeof (str), stdin);
    sscanf(str, " %lf %lf %lf", &cube->min_x, &cube->min_y,&cube->min_z);
    
    // seta o valor MÁXIMO de x,y,z para o cubo
    fgets(str, sizeof (str), stdin);
    sscanf(str, " %lf %lf %lf", &cube->max_x, &cube->max_y, &cube->max_z);
}

int main(int argc, char** argv) {
    
    double cubeLig_edge;
    char str[MAX], lig_name[MAX];
    Cube *out_cube;
    int a;
    
    out_cube = (Cube *) malloc( sizeof(Cube) );
    
    // armazena o valor da aresta do cubo em volta de cada ligante
    fgets(str, sizeof (str), stdin);
    sscanf(str, "%lf", &cubeLig_edge);
    
    // recebe o nome do ligante
    fgets(str, sizeof (str), stdin);
    sscanf(str, "%*[^0-9]%s", lig_name);
    
    setCubeCoords(out_cube);
    
    return (EXIT_SUCCESS);
}

