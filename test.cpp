#include "test.h"

void tester_adaboost(int* tableau, ClassifieurBooste cb, float theta){ 
  
  int reussite_pos = 0;
  int reussite_neg = 0;
 
  for(int i = 0; i < NB_F_NEG; i++) {
    
    int categorie = 0;
    vector<int> carac = get_carac(i, categorie, TEST);
    int retour = 0;
    
    //Ensuite, on demande au classifieur de nous le classifier
    
    if(retour == -1) {
      reussite_neg++;
    }
    
  }
  for(int i = NB_F_NEG; i < NB_F_TOTAL; i++) {
    
    int categorie = 0;
    vector<int> carac = get_carac(i, categorie, TEST);
    int retour = 0;
    
    //On demande au classifieur de le classifier
    
    if(retour == 1) {
	reussite_pos++;
    }
    
  }
  
  tableau[0] = reussite_neg;
  tableau[1] = reussite_pos;
  
}