#include "classifieur.h"
#include "utilitaires.h"

#include <iostream>
#include <assert.h>
#include <mpi.h>
#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <string>

using namespace std;

const int NB_CARAC = 7447580;//LOngueur du vecteur de caracteristiques	


void entrainer_classifieurs(vector<Classifieur*>, float, int, string);

//Actuellement, problemes de performance (cf repartition peut etre inadequate de la charge de travail	)
vector<Classifieur*> entrainement_MPI(float, int);