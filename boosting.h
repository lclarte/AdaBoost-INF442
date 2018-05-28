#ifndef BOOSTING_H
#define BOOSTING_H

#include "apprentissage.h"

class ClassifieurBooste {

public:
	ClassifieurBooste();
	~ClassifieurBooste();

	void adaboost(int);
	void initialiser_classifieurs(float, int);

	vector<Classifieur*> cls_faibles;
	int n; //nombre d'elements de la base de validation utilises
	vector<float> poids;

};
#endif
