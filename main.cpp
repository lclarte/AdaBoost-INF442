#include <sys/stat.h>
#include "apprentissage.h"

using namespace cv;
   
int main(int argc, char** argv) {

   MPI_Init(&argc, &argv);
   srand(time(NULL));

   /*cout <<"Nom du fichier : " << argv[1] << endl;
   Mat image = charger_image(argv[1]);
   int r = image.rows; //=92
   int c = image.cols; //=112
   int* tab[r];
   for(int i = 0; i < r; i++) {
      tab[i] = new int[c];   
   }

   calculer_image_integrale(tab, image);
   vector<int> carac = calculer_caracteristiques_MPI(tab);
   */

   vector<Classifieur*> cls = vector<Classifieur*>();
   string tmp = "../Images/";
   entrainer_classifieurs(cls, 0, 5, tmp);

   MPI_Finalize();

   return 0;
}
