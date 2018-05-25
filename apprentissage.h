#include "classifieur.h"

#include <iostream>
#include <assert.h>
#include <mpi.h>
#include <opencv2/opencv.hpp>

using namespace std;

const int NB_CARAC = 9608995;//LOngueur du vecteur de caracteristiques	

//Il y a autant de fichiers dans app/neg que test/neg et que dev/neg
const int NB_F_NEG = 4416;
const int NB_F_POS = 819;

void entrainer_classifieurs(vector<Classifieur*>,  float, int, char*="../Images/");
vector<Classifieur> creation_classifieur_faible(float,int);