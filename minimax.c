#include "chess.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// retourne la valeur de la piece dans position
int get_valeur(char position, unsigned char *plateau)
{
    int valeur;
    switch (plateau[position])
    {
    case VIDE:
        valeur = 0;
        break;
    case PION:
    case PION + PIECE_SPECIAL:
    case PION + PIECE_NOIRE:
    case PION + PIECE_NOIRE + PIECE_SPECIAL:
        valeur = VALEUR_PION;
        break;
    case CAVALIER:
    case CAVALIER + PIECE_NOIRE:
    case FOU:
    case FOU + PIECE_NOIRE:
        valeur = VALEUR_CAVALIER;
        break;
    case TOUR:
    case TOUR + PIECE_NOIRE:
        valeur = VALEUR_TOUR;
        break;
    case REINE:
    case REINE + PIECE_NOIRE:
        valeur = VALEUR_REINE;
        break;
    case ROI:
    case ROI + PIECE_SPECIAL:
    case ROI + PIECE_NOIRE:
    case ROI + PIECE_NOIRE + PIECE_SPECIAL:
        valeur = VALEUR_ROI;
        break;
    default:
        fprintf(stderr, "\nFONCTION GET_VALEUR: PIECE INCONNUE\n");
        valeur = -1;
    }
    return valeur;
}

// retourne la somme de la valeur des pieces d'une couleur passée en parametre
int get_valeur_total(char couleur, unsigned char *plateau)
{
    char i, nb_pieces = 0;
    int valeur = 0;

    if (couleur == NOIR)
    {
        i = 0;                                         // i commence à 0 car les noirs commencent en haut
        while (i < TAILLE_ECHEQUIER && nb_pieces < 16) // il n'y a jamais plus de 16 pieces d'une couleur
        {
            if (get_color(plateau[i]) == couleur)
            {
                valeur = valeur + get_valeur(i, plateau);
                nb_pieces++;
            }
            i++;
        }
    }
    else if (couleur == BLANC)
    {
        i = TAILLE_ECHEQUIER - 1;
        while (i >= 0 && nb_pieces < 16)
        {
            if (get_color(plateau[i]) == couleur)
            {
                valeur = valeur + get_valeur(i, plateau);
                nb_pieces++;
            }
            i--;
        }
    }
    else
    {
        fprintf(stderr, "\nERREUR FONCTION GET_VALEUR_TOTAL: COULEUR INVALIDE\n");
        return -1;
    }
    return valeur;
}

// retourne le score total de l'echequier passé en argument, score calculé par rapport aux noirs
int get_score(unsigned char *plateau)
{
    // score noir + somme des valeurs des pieces blanches capturées
    return get_valeur_total(NOIR, plateau) + (1290 - get_valeur_total(BLANC, plateau));
}

// retourne le plus grand entre les 2 entiers
int get_max(int a, int b)
{
    if (a > b)
        return a;
    else
        return b;
}

int get_min(int a, int b)
{
    if (a > b)
        return b;
    else
        return a;
}

// retourne 0 si on lui passe 1, et 1 si on lui passe 0
char get_minimizer(char maximizer)
{
    if (maximizer == 1)
    {
        return 0;
    }
    else
        return 1;
}

char get_couleur_ennemie(char couleur)
{
    if (couleur == NOIR)
    {
        return BLANC;
    }
    else
        return NOIR;
}

void test(unsigned char *plateau)
{
    liste *liste_pieces = (liste *)malloc(sizeof(liste));
    liste_pieces = NULL;

    liste_pieces = liste_moves(BLANC, liste_pieces, plateau);
    affichage_liste(liste_pieces);
    liste *tmp_pieces = liste_pieces;
    while (tmp_pieces != NULL)
    {
        printf("%d ", tmp_pieces->valeur);
        tmp_pieces = tmp_pieces->next;
    }
    liberation(liste_pieces);
}

// couleur = couleur du joueur qui doit jouer, maximizer = es-ce qu'il veut maximizer ou minimizer son score
int minimax(char couleur, char maximizer, unsigned char *plateau, char profondeur)
{
    char couleur_ennemie = get_couleur_ennemie(couleur);
    if (profondeur == 0 || echec_et_mat(couleur_ennemie, plateau) != -1)
    {
        printf("profondeur = 0");
        return get_score(plateau);
    }

    char minimizer = get_minimizer(maximizer);

    int best_eval;
    int eval;

    char i = 0;
    if (maximizer = 1) // on cherche a maximizer le score
    {
        best_eval = INT_MIN; // int min car on veut maximizer -> on commence au minimum

        liste *liste_pieces = (liste *)malloc(sizeof(liste)); // liste des pieces pouvant bouger
        liste_pieces = NULL;
        liste_pieces = liste_moves(couleur, liste_pieces, plateau);
        liste *tmp_pieces = liste_pieces;

        while (tmp_pieces != NULL) // POUR CHAQUE PIECE POUVANT BOUGER:
        {
            liste *moves = (liste *)malloc(sizeof(liste)); // liste des moves d'une piece
            moves = NULL;
            moves = get_legal_all(tmp_pieces->valeur, moves, plateau);
            liste *tmp_moves = moves;

            while (tmp_moves != NULL) // POUR CHAQUE MOVE DE LA PIECE:
            {
                // on effectue le move
                plateau[tmp_moves->valeur] = plateau[tmp_pieces->valeur];
                plateau[tmp_pieces->valeur] = VIDE;

                // on evalue l'echequier de la perspective de l'ennemi (il va vouloir minimizer donc maximizer = 0)
                eval = minimax(couleur_ennemie, 0, plateau, profondeur - 1);
                best_eval = get_max(best_eval, eval);

                tmp_moves = tmp_moves->next;
            }
            // liberation de moves avant la prochaine itération:
            liste *t = moves;
            while (moves != NULL)
            {
                t = moves->next;
                free(moves);
                moves = t;
            }
            tmp_pieces = tmp_pieces->next;
        }
        // liberation de liste_pieces:
        liste *tt = liste_pieces;
        while (liste_pieces != NULL)
        {
            tt = liste_pieces->next;
            free(liste_pieces);
            liste_pieces = tt;
        }

        return best_eval; // on retourne l'évaluation maximale
    }

    else // on cherche a minimizer le score
    {
        best_eval = INT_MAX; // int max car on veut minimizer -> on commence au maximum

        liste *liste_pieces = (liste *)malloc(sizeof(liste)); // liste des pieces pouvant bouger
        liste_pieces = NULL;
        liste_pieces = liste_moves(couleur, liste_pieces, plateau);
        liste *tmp_pieces = liste_pieces;

        while (tmp_pieces != NULL) // POUR CHAQUE PIECE POUVANT BOUGER:
        {
            liste *moves = (liste *)malloc(sizeof(liste)); // liste des moves d'une piece
            moves = NULL;
            moves = get_legal_all(tmp_pieces->valeur, moves, plateau);
            liste *tmp_moves = moves;

            while (tmp_moves != NULL) // POUR CHAQUE MOVE DE LA PIECE:
            {
                // on effectue le move
                plateau[tmp_moves->valeur] = plateau[tmp_pieces->valeur];
                plateau[tmp_pieces->valeur] = VIDE;

                // on evalue l'echequier de la perspective de l'ennemi (il va vouloir maximizer donc maximizer = 1)
                eval = minimax(couleur_ennemie, 1, plateau, profondeur - 1);
                best_eval = get_min(best_eval, eval);

                tmp_moves = tmp_moves->next;
            }
            // liberation de moves avant la prochaine itération:
            liste *t = moves;
            while (moves != NULL)
            {
                t = moves->next;
                free(moves);
                moves = t;
            }
            tmp_pieces = tmp_pieces->next;
        }
        // liberation de liste_pieces:
        liste *tt = liste_pieces;
        while (liste_pieces != NULL)
        {
            tt = liste_pieces->next;
            free(liste_pieces);
            liste_pieces = tt;
        }
        return best_eval; // on retourne l'évaluation maximale
    }
}