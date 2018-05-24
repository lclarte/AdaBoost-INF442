#include "utilitaires.h"
#include <math.h>
#include <time.h>
#include <iterator>

/**
	ATTENTION : FORMAT DE L'IMAGE INTEGRALE : tab[image][colonne]
*/

/***

rque : EN THEORIE ON A AU TOTAL : 
GAU : 675675 calculs
HAU: 540540    "
EXT:4054050
DIA: 4504500
==========
total : 9774765 > 7000000 (experimentalement)
peut etre une peitte erreur de calcul au niveau des indicages mais je ne pense pas que ca change grand chose
*/


void calculer_image_integrale(int **image_integrale, Mat image) {
      int rows = image.rows;
      int cols = image.cols;
      
      for(int i = 0; i < rows; i++) {
         for(int j = 0; j < cols; j++) {
            int val_gauche = (i > 0) ? image_integrale[i-1][j] : 0;
            int val_haut = (j > 0) ? image_integrale[i][j-1] : 0;
            int val_haut_gauche = (i > 0 && j > 0) ? image_integrale[i-1][j-1] : 0;
            
            image_integrale[i][j] = val_gauche + val_haut - val_haut_gauche + (int)image.at<uchar>(i,j);
         }
      }
     
}

Mat charger_image(char* filename) {
   return imread(filename, IMREAD_GRAYSCALE);
}

//attention, i_i est stockee dans le format i_i[lignes][colonnnes]
//ici, l est la ligne de depart, c colonne de depart
int rectangle(int l, int c, int l2, int c2, int** i_i) {
   return i_i[l2][c2] - i_i[l2][c] - i_i[l][c2] + i_i[l][c];
}

//l1, l2, c1 : largeurs et hauteur
int GAU(int l, int l1, int c, int c1, int c2, int** image_integrale) {
   return rectangle(l, c, l+l1, c+c1, image_integrale) - rectangle(l, c+c1, l+l1, c+c1+c2, image_integrale);
}

int HAU(int l, int l1, int l2, int c, int c1, int **image_integrale) {
   return -rectangle(l, c, l+l1, c+c1, image_integrale) + rectangle(l+l1, c, l+l1+l2, c+c1, image_integrale);
}

int EXT(int l, int l1, int c, int c1, int c2, int c3, int **image_integrale) {
   return rectangle(l, c, l+l1, c+c1, image_integrale) - rectangle(l, c+c1, l+l1, c+c1+c2, image_integrale) + rectangle(l, c+c1+c2, l+l1, c+c1+c2+c3, image_integrale);
}

int DIA(int l, int l1, int l2, int c, int c1, int c2, int** image_integrale) {
   return HAU(l, l1, l2, c, c1, image_integrale) - HAU(l, l1, l2, c+c1, c2, image_integrale);
}

/* 
* FONCTIONS POUR CALCULER TOUTES LES CARACTERISTIQUES 
*/

/*
* Cette fonction va calculer toutes les carac. GAU dont l'extremite haut-gauche 
est (l_depart, c_depart) 
*/
vector<int> calculer_tous_GAU(int l_depart, int c_depart, int** image_integrale) {
	//Standard : on va boucler d'abord sur les lignes, puis sur les colonnes
	vector<int> carac_GAU = vector<int>();
	for(int l1 = MIN_TAILLE; l_depart + l1 < NOMBRE_LIGNES; l1 += DELTA_TAILLE) {
		for(int c1 = MIN_TAILLE; c_depart + c1 < NOMBRE_COLONNES - MIN_TAILLE; c1 += DELTA_TAILLE) {
			for(int c2 = MIN_TAILLE; c_depart + c1 + c2 < NOMBRE_COLONNES; c2 += DELTA_TAILLE) {
				carac_GAU.push_back(GAU(l_depart, l1, c_depart, c1, c2, image_integrale));
			}
		}
	}
	return carac_GAU;
}

vector<int> calculer_tous_HAU(int l_depart, int c_depart, int** image_integrale) {
   vector<int> carac_HAU = vector<int>();
   for(int l1 = MIN_TAILLE; l_depart + l1 < NOMBRE_LIGNES - MIN_TAILLE; l1 += DELTA_TAILLE) {
      //nombre_lignes - min_taille : on "laisse de la place" pour l2
      for(int l2 = MIN_TAILLE; l_depart + l1 + l2 < NOMBRE_LIGNES; l2 += DELTA_TAILLE) {
         for(int c1 = MIN_TAILLE; c_depart + c1 < NOMBRE_COLONNES; c1 += DELTA_TAILLE) {
            carac_HAU.push_back(HAU(l_depart, l1, l2, c_depart, c1, image_integrale));
         }
      }
   }
   return carac_HAU;
}

vector<int> calculer_tous_EXT(int l_depart, int c_depart, int** image_integrale) {
	vector<int> carac_EXT = vector<int>();
	for(int l1 = MIN_TAILLE; l_depart + l1 < NOMBRE_LIGNES; l1 += DELTA_TAILLE) {
		for(int c1 = MIN_TAILLE; c_depart + c1 < NOMBRE_COLONNES - 2*MIN_TAILLE; c1 += DELTA_TAILLE) {
			for(int c2 = MIN_TAILLE; c_depart + c1 + c2 < NOMBRE_COLONNES - MIN_TAILLE; c2 += DELTA_TAILLE) {
				for(int c3 = MIN_TAILLE; c_depart + c1 + c2 + c3 < NOMBRE_COLONNES; c3 += DELTA_TAILLE) {
					carac_EXT.push_back(EXT(l_depart, l1, c_depart, c1, c2, c3, image_integrale));
				}
			}
		}
	}
	return carac_EXT;
}

vector<int> calculer_tous_DIA(int l_depart, int c_depart, int** image_integrale) {
	vector<int> carac_DIA = vector<int>();
	for(int l1 = MIN_TAILLE; l_depart + l1 < NOMBRE_LIGNES - MIN_TAILLE; l1 += DELTA_TAILLE) {
		for(int l2 = MIN_TAILLE; l_depart + l1 + l2 < NOMBRE_LIGNES; l2 += DELTA_TAILLE) {
			for(int c1 = MIN_TAILLE; c_depart + c1 < NOMBRE_COLONNES - MIN_TAILLE; c1 += DELTA_TAILLE) {
				for(int c2 = MIN_TAILLE; c_depart + c1 + c2 < NOMBRE_COLONNES; c2 += DELTA_TAILLE){
					carac_DIA.push_back(DIA(l_depart, l1, l2, c_depart, c1, c2, image_integrale));
				}
			}
		}
	}
	return carac_DIA;
}

vector<int> calculer_caracteristiques_MPI(int** image_integrale) {
  //On fait l'hypothese que MPI_Init et MPI_Finalize sont deja appeles dans le main

  const int root = 0;
  clock_t t1, t2;
  t1 = clock();
  int tasknb, taskid;

  MPI_Comm_size(MPI_COMM_WORLD, &tasknb);
  MPI_Comm_rank(MPI_COMM_WORLD, &taskid);

  //1erement, on verifie qu'on a une puissance de deux pour le nombre de proc.
  const float k = log2(tasknb);
  if(floor(k) - k < 0) {

      cerr << "Probleme dans le nombre de processus : il doit etre une puissance de deux" << endl;
      return vector<int>();
  }

  //2emement, on va repartir selon taskid les calculs des caracteristiques
  int taskcases = calculer_nombre_cases(taskid, tasknb);

  //Ensuite, on vectora calculer les dites carac.
  /**
   * On a un tableau de 4*taskcases cases (parce qu'on calcule pour GAU, HAU, EXT et DIA)
   * On parcourt dans une boucle toutes les cases du tableau et on appelle calculer_tous_***
   */
  vector<int>* carac_locales_array = new vector<int>[taskcases]; //stocke les vector 
  int case_actuelle_reduite = taskid; //case actuelle est un nombre, faut le convertir en couple (l, c)
  int l_actuel, c_actuel;
  int nb_total_cases = 0;
  for(int c = 0; c < taskcases; c++) {
  	vector<int> carac_locales = vector<int>();
  	convertir_case_indices(case_actuelle_reduite, l_actuel, c_actuel);

  	vector<int> tmp1 = calculer_tous_GAU(l_actuel, c_actuel, image_integrale); 
  	vector<int> tmp2 = calculer_tous_HAU(l_actuel, c_actuel, image_integrale);
  	vector<int> tmp3 = calculer_tous_EXT(l_actuel, c_actuel, image_integrale);
  	vector<int> tmp4 = calculer_tous_DIA(l_actuel, c_actuel, image_integrale);
  	
  	carac_locales.insert(carac_locales.end(), tmp1.begin(), tmp1.end()); // ORDE : GAU, HAU, EXT, DIA
  	carac_locales.insert(carac_locales.end(), tmp2.begin(), tmp2.end());
  	carac_locales.insert(carac_locales.end(), tmp3.begin(), tmp3.end());
  	carac_locales.insert(carac_locales.end(), tmp4.begin(), tmp4.end());

  	carac_locales_array[c] = carac_locales;
  	case_actuelle_reduite += tasknb;
  	nb_total_cases += carac_locales.size();
  }

  cout << "Le process " << taskid << " a termine. Nombre de cas traites : " << nb_total_cases << endl;

  //3emement, on va envoyer les carac. au root de la facon suivante (idee basique)
  //Le root va parcourir l'image et regarder case par case le proc. qui est cense envoyer 
  //les donnees.
  if(taskid == root) {
  	int compteur_array_local = 0; //compte la case du tableau carac_locale_array a stocker
  	vector<int> contenu_total = vector<int>();

  	//on parcourt toutes les cases 
  	for(int c = 0; c < NB_CASES_REDUIT; c++) {
  		MPI_Status* status;
  		int task_src = c % tasknb;
	  	if(task_src != root){	
	  		int taille_contenu;
	  		int* contenu_recu;
	  		MPI_Recv(&taille_contenu, 1, MPI_INT, task_src, 0, MPI_COMM_WORLD, status);
	  		if(taille_contenu > 0) {
	  			contenu_recu = new int[taille_contenu];
	  			MPI_Recv(contenu_recu, taille_contenu, MPI_INT, task_src, 0, MPI_COMM_WORLD, status);
	  			for(int i = 0; i < taille_contenu; i++) {
	  				contenu_total.push_back(contenu_recu[i]); 
	  			}
	  		}

	  	}
	  	else{
	  		for(int i = 0; i < carac_locales_array[compteur_array_local].size(); i++) {
	  			contenu_total.push_back(carac_locales_array[compteur_array_local].at(i));
	  		}
	  		compteur_array_local += 1;
	  	}

	}
	cout << "Taille totale : " << contenu_total.size() << endl;	
	t2 = clock();
	cout << "Temps d'execution : " << t2 - t1 << endl;
  	return contenu_total;
  }
  else{
  	//du cote des autres process, on va envoyer en continu tous les data
  	for(int t = 0; t < taskcases; t++) {
  		vector<int> a_envoyer = carac_locales_array[t];
  		int taille_envoi = a_envoyer.size();
  		MPI_Send(&taille_envoi, 1, MPI_INT, root, 0, MPI_COMM_WORLD);
  		if(taille_envoi > 0){
  			MPI_Send(a_envoyer.data(), a_envoyer.size(), MPI_INT, root, 0, MPI_COMM_WORLD);
  		}
  	}

  }

}

int calculer_nombre_cases(int taskid, int tasknb, int nb_cases) {
	int nb_cases_reduit = nb_cases/16; //on va faire des deplacements de 4 en 4
	//sur les deux directions donc le nombre total de cases est divise par 16
    return nb_cases_reduit/tasknb + (taskid < (nb_cases_reduit%tasknb) ? 1 : 0);
}

//convertit en prenant en compte le fait que on passe DELTA_TAILLE cases dans le sens des lignes et colonnes
//on suppose que indice est deja dans NOMBRE_LIGNES*NOMBRE_COLONNES/(DELTA_TAILLE**2) 
void convertir_case_indices(int indice_reduit, int& ligne_reelle, int& colonne_reelle) {
	ligne_reelle = DELTA_TAILLE*(indice_reduit/NB_CLN_REDUIT)-1;
	colonne_reelle = DELTA_TAILLE*(indice_reduit%NB_CLN_REDUIT)-1;
}

