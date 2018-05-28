#include "boosting.h"

ClassifieurBooste::ClassifieurBooste() {
	cls_faibles = vector<Classifieur*>();
	poids 		= vector<float>();
	int n = 0;
}

Classifieur::~Classifieur() { }

void Classifieur::initialiser_classifieurs(float epsilon, int K) {
	//Initialise le tableau cls_faibles par le MPI
	cls_faibles = entrainenement_MPI(epsilon, K);
}

void Classifieur::adaboost(int n_) {
	n = n_;
	
	//Dans un premier temps, on initialise les vecteurs
	poids = vector<float>();
	for(int i = 0; i < n; i++) {
		poids.push_back(1/n);
	}

	/*On va calculer pour tout classifieur, la quantite E(h^k(x_j), c_j) qui est une quantite fixee
	*puisque les classifieurs n'evoluent plus (seule la ponderation change)
	* On peut stocker ces coefficients dans un tableau statique
	*/

	//Ensuite, on boucle sur K

	/*On calcule le classifieur qui minimise l'erreur, de maniere parallelle : chaque processus va 
	* calculer un "pretendant", puis le root va comparer tous les pretendants
	*/

}