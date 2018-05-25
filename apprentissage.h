#include "classifieur.h"
#include "utilitaires.h"

#include <iostream>
#include <assert.h>
#include <mpi.h>
#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <string>

using namespace std;

const int NB_CARAC = 9608995;//LOngueur du vecteur de caracteristiques	

//Il y a autant de fichiers dans app/neg que test/neg et que dev/neg
const int NB_F_NEG = 4416;
const int NB_F_POS = 819;

void entrainer_classifieurs(vector<Classifieur*>, float, int, string);
vector<Classifieur> creation_classifieur_faible(float,int);