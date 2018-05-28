#include "apprentissage.h"
#include <string>
#include <cstring>

using namespace std;
	
	//Cette fonction est appelee par chaque process, qui va charger K images dans le nom_dossier de 
	//maniere aleatoire. On va boucler sur chaque image, calculer le vecteur caracteriristique 
	//et mettre a jour tous les classifieurs de cls

	//l'entraingement des classifieurs se fait grace a la fonctoin regression
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

		/*
		* REMARQUE IMPORTANTE : Ici, chaque processus calcule les caracteristiques de maniere sequentielle 
		(et n'utilise donc pas la fonction calculer_caracteristiques_MPI) car le gain de vitesse n'est pas 
		(encore) significatif et en plus ca peut creer des blocages via les differents Send/Rcv de cette 
		fonction et de l'autre qui "se croisent"
		*/
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

}

vector<Classifieur*> entrainement_MPI(float epsilon, int K) {
	/*Dans un premier temps, on va repartir les indices des differents 
	* classifieurs pour chaque processus
	*/

	const int root = 0;

	int taskid;
	int tasknb;

	MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
	MPI_Comm_size(MPI_COMM_WORLD, &tasknb);

	int nb_classifieurs = NB_CARAC/tasknb;
	int debut_classifieurs = NB_CARAC%tasknb + taskid*nb_classifieurs;

	/**
	* REMARQUE : on va utiliser la fonction MPI_Gather pour recuperer les poids de tous les 
	* classifieurs il faut donc que les process y compris root envoient le meme nombre de donnees
	donc les NB_CARAC%tasknb seront rajoutes par le root a LA TOUTE FIN (au lieu d'etre calculees
	avec tous les autres)
	*/
	

	vector<Classifieur*> cls = vector<Classifieur*>();
	for(int i = 0; i < nb_classifieurs; i++) {
		cls.push_back(new Classifieur(1.0, 0.0, i+debut_classifieurs));
	}
	
	//Chaque processus va entrainer son classifieur dans son coin
	entrainer_classifieurs(cls, epsilon, K, DEFAULT_FOLDER);

	float* buffer_w_local = new float[2*nb_classifieurs];
	for(int i = 0; i < cls.size(); i++) {
		buffer_w_local[2*i] = cls.at(i)->get_w1();
		buffer_w_local[2*i+1] = cls.at(i)->get_w2();
	}
	
	float* buffer_w_global = new float[2*nb_classifieurs*tasknb];
	MPI_Gather(buffer_w_local, nb_classifieurs, MPI_FLOAT, buffer_w_global, nb_classifieurs, MPI_FLOAT, root, MPI_COMM_WORLD);
	if(taskid == root) {
		//Pour le root, on va en plus calculer les premiers w1 et w2
		vector<Classifieur*> retour = vector<Classifieur*>();//liste de classifieurs qu'on va retourner
		for(int i = 0; i < NB_CARAC%tasknb; i++) {
			retour.push_back(new Classifieur(1.0, 0.0, i));
		}
		entrainer_classifieurs(retour, epsilon, K, DEFAULT_FOLDER);
			//on ajoute les classifieurs qui correspondent au tableau buffer_w_global
		for(int i = 0; i < nb_classifieurs*tasknb; i++) {
			Classifieur* cl = new Classifieur(buffer_w_global[2*i], buffer_w_global[2*i+1], i+(NB_CARAC%tasknb));
			retour.push_back(cl);
		}
		return retour;
	}
	return vector<Classifieur*>(); //pour les autres processus on s'en fout 
	//de ce qu'on renvoie	
	
}
