#include <sys/stat.h>
#include "utilitaires.h"

using namespace cv;

int main(int argc, char** argv) {
   cout <<"Nom du fichier : " << argv[1] << endl;
   Mat image = charger_image(argv[1]);
   int r = image.rows;
   int c = image.cols;
   int* tab[r];
   for(int i = 0; i < r; i++) {
      tab[i] = new int[c];   
   }
   
   calculer_image_integrale(tab, image);
   for(int i = 0; i < r; i++) {
      for(int j = 0; j < c; j++) {
         cout << tab[i][j] << " " ;      
      }
      cout << endl;
   }
   return 0;
}
