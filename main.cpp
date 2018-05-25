#include <sys/stat.h>
#include "utilitaires.h"

using namespace cv;
   
int main(int argc, char** argv) {

   MPI_Init(&argc, &argv);

   cout <<"Nom du fichier : " << argv[1] << endl;
   Mat image = charger_image(argv[1]);
   int r = image.rows; //=92
   int c = image.cols; //=112
   int* tab[r];
   for(int i = 0; i < r; i++) {
      tab[i] = new int[c];   
   }

   calculer_image_integrale(tab, image);
   vector<int> carac = calculer_caracteristiques_MPI(tab);

   MPI_Finalize();

   return 0;
}
