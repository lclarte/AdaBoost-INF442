#include "classifieur.h"

#include <iostream>
#include <assert.h>
#include <mpi.h>
#include <opencv2/opencv.hpp>

using namespace std;

//Nombre de fichiers dans chaque dossier
const string chemin_app("../Images/app");
const string chemin_dev("../Images/dev");
const string chemin_test("../Images/test");

//Nombre de fichiers dans chaque dossier, a changer par les vraies valeurs
const int NB_F_APP_POS = 0;
const int NB_F_APP_NEG = 0;
const int NB_F_DEV_POS = 0;
const int NB_F_DEV_NEG = 0;
const int NB_F_TEST_POS = 0;
const int NB_F_TEST_NEG = 0;


void entrainer_classifieurs(vector<Classifieur*>,  float, int);
vector<Classifieur> creation_classifieur_faible(float,int);