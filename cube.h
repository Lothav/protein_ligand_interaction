
/* 
 * File:   cube.h
 * Author: root
 *
 * Created on 4 de Dezembro de 2015, 03:49
 */

#ifndef CUBE_H
#define CUBE_H
#define MAX 500


typedef struct cube_str {
    // max/min referem-se às cordenadas de um certo cubo:
    // Sendo:
    //  - max/min [0] = x
    //  - max/min [1] = y
    //  - max/min [2] = z
    double max[3];
    double min[3];
} Cube;

#endif /* CUBE_H */