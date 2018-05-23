#include "utilitaires.h"

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
   return i_i[l2][c2] - i_i[l2][c] - i_i[l][c2] + i_i[l][c]
}

//l1, l2, c1 : largeurs et hauteur
int GAU(int l, int l1, int c, int c1,, int c2, int** image_integrale) {
   return rectangle(l, c, l+l1, l+c1, image_integrale) - rectangle(l, c+c1, l+l1, c+c1+c2, image_integrale);
}

int HAU(int l, int l1, int l2, int c, int c1, int **image_integrale) {
   return -rectangle(l, c, l+l1, c+c1) + rectangle(l+l1, c, l+l1+l2, c+c1);
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
}

vector<int> calculer_caracteristiques_MPI(int** image_integrale) {
  //On fait l'hypothese que MPI_Init et MPI_Finalize sont deja appeles dans le main

  const int root = 0;
  int tasknb, taskid;

  MPI_Comm_size(MPI_COMM_WORLD, &tasknb);
  MPI_Comm_rank(MPI_COMM_WORLD, &taskid);

  //1erement, on verifie qu'on a une puissance de deux pour le nombre de proc.
  const float k = log(tasknb);
  if(floor(k) - k < 0) {
      cerr << "Probleme dans le nombre de processus : il doit etre une puissance de deux" << endl;
      return vector<int>();
  }

  //2emement, on va repartir selon taskid les calculs des caracteristiques
  int taskcases = calculer_nombre_cases(taskid, tasknb);

//Ensuite, on va calculer les dites carac.
  /**
   * On a un tableau de 4*taskcases cases (parce qu'on calcule pour GAU, HAU, EXT et DIA)
   * On parcour dans une boucle toutes les cases du tableau et on appelle calculer_tous_***
   */

  //3emement, on va envoyer les carac. au root de la facon suivante (idee basique)
  //Le root va parcourir l'image et regarder case par case le proc. qui est cense envoyer 
  //les donnees.
  //le proc, de son cote, va envoyer au root les donnes calculees sous forme de tableau (attention, il faudra 
  //convertir un array en vector et reciproquement

  //Le root va stocker toutes ces donnees dans un vector qu'il va retourner
}
