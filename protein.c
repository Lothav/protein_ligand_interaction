#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#include "protein.h"


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