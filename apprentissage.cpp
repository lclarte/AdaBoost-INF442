#include "apprentissage.h"
#include <string>

using namespace std;

void entrainer_classifieurs(vector<Classifieur*> cls, float epsilon, int K, string nom_dossier) {
	//il faut avoir appele srand avant
	const int NB_F_TOTAL = NB_F_NEG + NB_F_POS;

	for(int k = 0; k < K; k++) {
		string copie_nom_dossier(nom_dossier);
		string nom_fichier = "";

		int nb_fichier = rand() % NB_F_TOTAL;
		if(nb_fichier >= NB_F_NEG) {
			nb_fichier -= NB_F_NEG;
			stringstream ss;
			ss << nb_fichier;
			string str_nb_fichier = ss.str();

			nom_fichier = nom_dossier + "pos/im" + str_nb_fichier + ".jpg"; 
		}
		else{
			stringstream ss;
			ss << nb_fichier;
			string str_nb_fichier = ss.str();

			nom_fichier = nom_dossier + "neg/im" + str_nb_fichier + ".jpg";
		}
		char* nom_fichier_char;
		//Il faut stocker dans nom_fichier_char	

		cout << nom_fichier << endl;
	}

	
	//Cette fonction est appelee par chaque process, qui va charger K images dans le nom_dossier de 
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