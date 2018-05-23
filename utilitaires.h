#include <iostream>
#include <assert.h>
#include <mpi.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

const int NOMBRE_COLONNES = 92;
const int NOMBRE_LIGNES   = 112;
const int DELTA_TAILLE    = 4;
const int MIN_TAILLE      = 8;

const int NB_LGN_REDUIT = NOMBRE_LIGNES/DELTA_TAILLE;
const int NB_CLN_REDUIT = NOMBRE_COLONNES/DELTA_TAILLE;

void calculer_image_integrale(int**, Mat);
Mat charger_image(char*);

int rectangle(int, int, int, int, int**); //calcule la somme dans le rectangle a partir de l'image integrale
int GAU(int, int, int, int, int, int**); //on soustrait la zone de droite a la zone de gauche
int HAU(int, int, int, int, int, int**); //on soustrait la zone du haut a la zone du bas
int EXT(int, int, int, int, int, int, int**); //on soustrait la zone du milieu au zones exterieures
int DIA(int, int, int, int, int, int, int**);

vector<int> calculer_tous_GAU(int, int, int**);
vector<int> calculer_tous_HAU(int, int, int**);
vector<int> calculer_tous_EXT(int, int, int**);
vector<int> calculer_tous_DIA(int, int, int**);

vector<int> calculer_caracteristiques_MPI(int**);
int calculer_nombre_cases(int, int, int=NOMBRE_COLONNES*NOMBRE_LIGNES);
void convertir_case_indices(int, int&, int&);