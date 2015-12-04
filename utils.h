/* 
 * File:   utils.h
 * Author: root
 *
 * Created on 4 de Dezembro de 2015, 03:57
 */

#ifndef UTILS_H
#define UTILS_H



// Função solicitada na descrição.
// Conta e soma de forma recursiva todas as interações Ligante/Proteína.
void getPointsInsideBox(Leaf* leaf, Ligante lig, double cubeLig_edge, int *sum);

// Função de STDOUT.
// exibe na tela os resultados
void printResult(LiganteList* ligant_list);


#endif /* UTILS_H */
    