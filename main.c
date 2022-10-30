#include "chess.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int echequier[64];

int main(){

    int ligne, colonne, position, rep = 0;
    initialiser_debug();
    
    affichage_echequier(echequier, MAX);
    while (rep != 2){

        printf("\nSelectionner une piece:\n\tColonne: ");
        scanf("%d", &colonne);
        printf("\n\tLigne: ");
        scanf("%d", &ligne);

        position = get_pos(ligne, colonne);
        bouger(position);
        affichage_echequier(echequier, MAX);
        printf("\n*Continuer? (1: oui, 2: non): ");
        scanf("%d", &rep);
    }

    return 0;
}
