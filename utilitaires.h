#ifndef UTILITAIRES
#define UTILITAIRES

#include <iostream>
#include <assert.h>
#include <mpi.h>
#include <opencv2/opencv.hpp>
#include <time.h>

const int APP = 0; //numero pour reperer les dossiers
const int DEV = 1;
const int TEST = 2;

const std::string DEFAULT_FOLDER = "/usr/local/INF442-2018/P5/";

using namespace cv;
using namespace std;

const int NOMBRE_COLONNES = 112;
const int NOMBRE_LIGNES   = 92;
const int NOMBRE_CASES    = NOMBRE_COLONNES*NOMBRE_LIGNES;
const int DELTA_TAILLE    = 4; //vraie valeur : 4
const int MIN_TAILLE      = 8;

const int NB_CASES_REDUIT = NOMBRE_CASES/(DELTA_TAILLE*DELTA_TAILLE);
const int NB_LGN_REDUIT   = NOMBRE_LIGNES/DELTA_TAILLE;
const int NB_CLN_REDUIT   = NOMBRE_COLONNES/DELTA_TAILLE;

//Nombres de fichiers positifs et negatifs
const int NB_F_NEG = 4416;
const int NB_F_POS = 819;
const int NB_F_TOTAL = NB_F_POS + NB_F_NEG;

void calculer_image_integrale(int**, Mat);
Mat charger_image(const char*);

//Recupere la caracteristique de l'image en fonction de son numero dans la base de donnees
vector<int> get_carac(int, int&, int=DEV);

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
vector<int> calculer_caracteristiques_sequentiel(int**);
int calculer_nombre_cases(int, int, int=NOMBRE_COLONNES*NOMBRE_LIGNES);
void convertir_case_indices(int, int&, int&);

#endif