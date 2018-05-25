#include "classifieur.h"

#include <iostream>
#include <assert.h>
#include <mpi.h>
#include <opencv2/opencv.hpp>

using namespace std;

const int NB_CARAC = 9608995;//LOngueur du vecteur de caracteristiques	

//Nombre de fichiers dans chaque dossier, a changer par les vraies valeurs
const int NB_F_APP_POS = 0;
const int NB_F_APP_NEG = 0;
const int NB_F_DEV_POS = 0;
const int NB_F_DEV_NEG = 0;
const int NB_F_TEST_POS = 0;
const int NB_F_TEST_NEG = 0;


void entrainer_classifieurs(vector<Classifieur*>,  float, int, char*="../Images");
vector<Classifieur> creation_classifieur_faible(float,int);