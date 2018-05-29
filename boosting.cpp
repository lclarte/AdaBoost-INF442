#include "boosting.h"

ClassifieurBooste::ClassifieurBooste() {
	cls_faibles = vector<Classifieur*>();
	alpha 		= vector<float>();
	int n = 0;
}

Classifieur::~Classifieur() { }

//Dans cette fonction, on initialise TOUS les classifieurs pour le vecteur caracteristique
void Classifieur::initialiser_classifieurs(float epsilon, int K) {
	//Initialise le tableau cls_faibles par le MPI
	cls_faibles = entrainenement_MPI(epsilon, K);
}

/*
* Une maniere intelligence de faire les choses est que chaque processus entraine dans son coin les classifieurs qui lui sont
* assignes, et ensuite lors de adaboost on NE VA PAS envoyer les valeurs des differents classifieurs mais a chaque etape de AdaBoost
*les processus vont envoyer leur minorant, puis on va appliquer l'algorithme
*Avantage : On s'economise pas mal de calculs
*Inconvenient : un peu relou a coder
*/

/*Ici, on suppose qu'on a deja appele initialiser_classifieurs pour entrainer les classifieurs*/
void Classifieur::adaboost(int n_) {
	int taskid, tasknb;
	const int root = 0;

	MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
	MPI_Comm_size(MPI_COMM_WORLD, &tasknb);

	this->n = n_;
	
	//Dans un premier temps, on initialise les vecteurs
	alpha = vector<float>();
	for(int i = 0; i < this->n; i++) {
		alpha.push_back(0);
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
