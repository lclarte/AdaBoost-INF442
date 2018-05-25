#include "apprentissage.h"

using namespace std;


void entrainer_classifieurs(vector<Classifieur*> cls, float epsilon, int K) {
	//Cette fonction est appelee par chaque process, qui va charger K images dans le dossier de 
	//maniere aleatoire. On va boucler sur chaque image, calculer le vecteur caracteriristique 
	//et mettre a jour tous les classifieurs de cls

	//l'entraingement des classifieurs se fait grace a la fonctoin regression


}

vector<Classifieur> creation_classifieur_faible(float epsilon, int K) {

	//Dans un premier temps, on doit connaitre la longueur du vecteur caracteristique

	//On va creer les classifieurs, envoyer depuis le root les indices des classifieurs, 	
	//les autres vont recuperer les indices, creer localement les classifieurs et 
	//les entrainer avec entrainer_classifieurs
	//Ensuite on envoie au root les couples (w1, w2) grace a la fonction MPI_Gather 

	//On initialise les classifieurs avec ces valeurs et on renvoie 

}