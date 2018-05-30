#include <math.h>
#include <time.h>
#include <iterator>
#include<string>
#include "utilitaires.h"

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

int binary_to_int(int* binaire){
	int sum = 0;
	for (int i = 0; i<(sizeof(binaire)/sizeof(*binaire));++i){
		sum = sum + binaire[i]*pow(2,i);
	}
	return sum;
}

int ou_exclusif(int a,int b){
	if (a != b){return 1;}
	else {return 0;}
}

int* int_to_binary(int id, int taille){
	int* binaire = new int[taille];
	for (int i = 0;i<taille;i++){binaire[i]=0;}
	int nombre = id;
	int reste = 0;
	int i = 0;
	while (nombre != 0){
		reste = nombre%2;
		nombre = nombre/2;
		binaire[i] = reste;
		++i;
	}
	return binaire;
}

int* gray_to_binary(int* code_gray){
	int* binaire = new int[sizeof(code_gray)/sizeof(*code_gray)];
	binaire[sizeof(code_gray)/sizeof(*code_gray)-1] = code_gray[sizeof(code_gray)/sizeof(*code_gray)-1];
	for(int i = sizeof(code_gray)/sizeof(*code_gray)-2;i>=0;--i){
		binaire[i] = ou_exclusif(binaire[i+1],code_gray[i]);
	}
	return binaire;
}

int gray_to_id(int* code_gray, bool valeur, int indice){
	int* code_gray_bis = new int[sizeof(code_gray)/sizeof(*code_gray)];
	for(int i =0;i<sizeof(code_gray)/sizeof(*code_gray);i++){
		code_gray_bis[i] = code_gray[i];
	}
	if(valeur){
		code_gray_bis[indice] = 1;
	}
	else{ 
		code_gray_bis[indice] = 0;
	}
	int* binaire = gray_to_binary(code_gray_bis);
	return binary_to_int(binaire);

}

int* binary_to_code_gray(int* id,int taille){
	int* code_gray = new int[taille];
	int* binaire_decale = new int[taille];
	for (int i =0; i<taille-1;++i){
		code_gray[i] = ou_exclusif(id[i],id[i+1]);
	code_gray[taille-1] = id[taille-1];
	return code_gray;
}
}


int* int_to_code_gray(int id,int taille){
	return binary_to_code_gray(int_to_binary(id,taille),taille);}

void calculer_image_integrale(int** image_integrale, Mat image){
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

Mat charger_image(const char* filename) {
   return imread(filename, IMREAD_GRAYSCALE);
}

vector<int> get_carac(int nb_fichier, int& categorie, int dossier) {
  string nom_dossier = DEFAULT_FOLDER;
  switch(dossier){
    
    case APP:
      nom_dossier += "app∕";
      break;
    case DEV:
      nom_dossier += "dev/";
      break;
     
    case TEST:
      nom_dossier += "test/";
      break;
    
    default:
      break;
  }
  
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
		categorie = 1;
	}
	else{
		stringstream ss;
		ss << nb_fichier;
		string str_nb_fichier = ss.str();
		nom_fichier = nom_dossier + "neg/im" + str_nb_fichier + ".jpg";
		categorie = -1;
	}
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
	vector<int> vec_carac = calculer_caracteristiques_sequentiel(image_integrale); //A remplacer eventuellement par du paralleliser 
	
	return vec_carac;
	
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
vector<int> calculer_caracteristiques_MPI(int** image_integrale){
//clock_t t1, t2; //pour avoir le temps ecoule : t1 = clock()
//t = clock();
int tasknb;
int taskid;
MPI_Comm_size(MPI_COMM_WORLD, &tasknb);
MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
const float k = log2(tasknb);
if(floor(k) - k < 0) {
      cerr << "Probleme dans le nombre de processus : il doit etre une puissance de deux" << endl;
      return vector<int>();
}

//On converti les id de chaque processus en code gray pour se placer dans la topologie de
//l'hypercube pour pouvoir transmettre les donnees plus rapidement
int* taskid_code_gray = int_to_code_gray(taskid,k);
int taskcases = calculer_nombre_cases(taskid, tasknb); //calcul du nombre de taches a effectuer pour chaque
//processus
int case_actuelle_reduite = taskid; //case actuelle est un nombre, faut le convertir en couple (l, c)
int l_actuel, c_actuel;
int nb_total_cases = 0;
vector<int> contenu_total = vector<int>();

for(int c = 0; c < taskcases; c++) {
	//tableau qu'on communique
	vector<int>* carac_locales_array = new vector<int>[tasknb];
	vector<int> carac_locales = vector<int>();
	convertir_case_indices(case_actuelle_reduite, l_actuel, c_actuel);
	//calcul des caracteristiques
  	vector<int> tmp1 = calculer_tous_GAU(l_actuel, c_actuel, image_integrale); 
  	vector<int> tmp2 = calculer_tous_HAU(l_actuel, c_actuel, image_integrale);
  	vector<int> tmp3 = calculer_tous_EXT(l_actuel, c_actuel, image_integrale);
  	vector<int> tmp4 = calculer_tous_DIA(l_actuel, c_actuel, image_integrale);
  	
  	carac_locales.insert(carac_locales.end(), tmp1.begin(), tmp1.end()); // ORDE : GAU, HAU, EXT, DIA
  	carac_locales.insert(carac_locales.end(), tmp2.begin(), tmp2.end());
  	carac_locales.insert(carac_locales.end(), tmp3.begin(), tmp3.end());
  	carac_locales.insert(carac_locales.end(), tmp4.begin(), tmp4.end());

  	carac_locales_array[0] = carac_locales;
  	case_actuelle_reduite += tasknb;
  	nb_total_cases += carac_locales.size();
	//On parcout les bits en code gray du poids le plus faible au point le plus fort
	for (int i =0; i < k;++i){
		if (taskid_code_gray[i]==1){
			int sizeoft = carac_locales_array[i].size();
			//chaque processus avec sur le bit etudie un "1" envoie au processus avec le meme code
			//gray mais un "0" a cet endroit
  			MPI_Send(&sizeoft, 1, MPI_INT, gray_to_id(taskid_code_gray,false,i), 0,MPI_COMM_WORLD);
			if(carac_locales_array[i].size() > 0){
				int* locale = new int[carac_locales_array[i].size()];
				for (int landa=0; landa<carac_locales_array[i].size();landa++){
					  locale[landa] = carac_locales_array[i][landa];}
  				MPI_Send(locale, carac_locales_array[i].size(), MPI_INT, gray_to_id(taskid_code_gray,false,i), 0, MPI_COMM_WORLD);
  			}
			MPI_Barrier(MPI_COMM_WORLD);}
		else{
			MPI_Status* status;
	  		int taille_contenu = 0;
	  		
	  		MPI_Recv(&taille_contenu, 1, MPI_INT, gray_to_id(taskid_code_gray,true,i), 0, MPI_COMM_WORLD, status);
	  		if(taille_contenu > 0) {
				int* contenu_recu = new int[taille_contenu];
	  			MPI_Recv(contenu_recu, taille_contenu, MPI_INT, gray_to_id(taskid_code_gray,true,i), 0, MPI_COMM_WORLD, status);
				if(taskid == 0){
	  				for(int j = 0; j < taille_contenu; j++) {
	  					contenu_total.push_back(contenu_recu[j]); 
	  				}
				}
				else{
					vector<int> contenu_provisoire;
					for(int j=0;j<carac_locales_array[i].size();j++){
						contenu_provisoire.push_back(carac_locales_array[i][j]);
					}
					for(int j = 0; j < taille_contenu; j++) {
	  					contenu_provisoire.push_back(contenu_recu[j]); 
	  				}
	  			//on a le nouveau vecteur a transmettre;	
	  			carac_locales_array[i+1] = contenu_provisoire;	
				}
			}
			MPI_Barrier(MPI_COMM_WORLD);}
		}

   }
//t=t-clock();

//cout << "temps écoulé : " << t << endl;


return contenu_total;
}

vector<int> calculer_caracteristiques_sequentiel(int** image_integrale) {
  vector<int> caracteristiques = vector<int>();
  for(int i = 0; i < NB_CASES_REDUIT; i++) {
    int l_actuel(0), c_actuel(0);
    convertir_case_indices(i, l_actuel, c_actuel);

    vector<int> tmp1 = calculer_tous_GAU(l_actuel, c_actuel, image_integrale); 
    vector<int> tmp2 = calculer_tous_HAU(l_actuel, c_actuel, image_integrale);
    vector<int> tmp3 = calculer_tous_EXT(l_actuel, c_actuel, image_integrale);
    vector<int> tmp4 = calculer_tous_DIA(l_actuel, c_actuel, image_integrale);

    caracteristiques.insert(caracteristiques.end(), tmp1.begin(), tmp1.end());
    caracteristiques.insert(caracteristiques.end(), tmp2.begin(), tmp2.end());
    caracteristiques.insert(caracteristiques.end(), tmp3.begin(), tmp3.end());
    caracteristiques.insert(caracteristiques.end(), tmp4.begin(), tmp4.end());
  }

  return caracteristiques;
}

int calculer_nombre_cases(int taskid, int tasknb, int nb_cases) {
	int nb_cases_reduit = nb_cases/16; //on va faire des deplacements de 4 en 4
	//sur les deux directions donc le nombre total de cases est divise par 16
    return nb_cases_reduit/tasknb + (taskid < (nb_cases_reduit%tasknb) ? 1 : 0);
}

//convertit en prenant en compte le fait que on passe DELTA_TAILLE cases dans le sens des lignes et colonnes
//on suppose que indice est deja dans NOMBRE_LIGNES*NOMBRE_COLONNES/(DELTA_TAILLE**2) 
void convertir_case_indices(int indice_reduit, int& ligne_reelle, int& colonne_reelle) {
	ligne_reelle = DELTA_TAILLE*(indice_reduit/NB_CLN_REDUIT); //pourquoi un -1 ? 
	colonne_reelle = DELTA_TAILLE*(indice_reduit%NB_CLN_REDUIT);
}

