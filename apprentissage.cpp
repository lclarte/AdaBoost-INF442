#include "apprentissage.h"
#include <string>
#include <cstring>

using namespace std;

void entrainer_classifieurs(vector<Classifieur*> cls, float epsilon, int K, string nom_dossier) {
	//il faut avoir appele srand avant
	const int NB_F_TOTAL = NB_F_NEG + NB_F_POS;

	for(int k = 0; k < K; k++) {

		/***
		* Dans un premier temps, on va charger l'image
		*/
		string copie_nom_dossier(nom_dossier);
		string nom_fichier = "";
		int categorie_fichier = -1;

		int nb_fichier = rand() % NB_F_TOTAL;
		if(nb_fichier >= NB_F_NEG) {
			nb_fichier -= NB_F_NEG;
			stringstream ss;
			ss << nb_fichier;
			string str_nb_fichier = ss.str();

			nom_fichier = nom_dossier + "pos/im" + str_nb_fichier + ".jpg"; 
			categorie_fichier = 1;
		}
		else{
			stringstream ss;
			ss << nb_fichier;
			string str_nb_fichier = ss.str();

			nom_fichier = nom_dossier + "neg/im" + str_nb_fichier + ".jpg";
		}
		//Il faut stocker dans nom_fichier_char	le nom du fichier
		Mat image = charger_image(nom_fichier.c_str());

		/*
		* Ensuite, on va calculer le vecteur caracteristique
		*/ 
		int** image_integrale = new int*[NOMBRE_LIGNES];
		for(int l = 0; l < NOMBRE_LIGNES; l++) {
			image_integrale[l] = new int[NOMBRE_COLONNES];
		}

		calculer_image_integrale(image_integrale, image);

		vector<int> vec_carac = calculer_caracteristiques_sequentiel(image_integrale);

		/**
		* Pour chaque classifieur, on va faire la regression idoine
		*/
		for(vector<Classifieur*>::iterator it = cls.begin(); it != cls.end(); ++it) {
			Classifieur* cl_actuel = *it;
			int indice_vec = cl_actuel->get_indice();
			cl_actuel->regression(vec_carac[indice_vec], categorie_fichier, epsilon);
		}
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