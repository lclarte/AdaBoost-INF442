#include <sys/stat.h>
#include "apprentissage.h"

using namespace cv;
   
int main(int argc, char** argv) {

   MPI_Init(&argc, &argv);
   srand(time(NULL));

   /* Test 1 : test de la fonction du calcul distribue des caracteristiques
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
   */
   clock_t t1, t2;

   t1 = clock();
   entrainement_MPI(1.0, 1000);
   t2 = clock();
   int taskid;
   MPI_Comm_rank(MPI_COMM_WORLD, &taskid);

   cout << taskid << ':' <<   t2 - t1 << endl;

   MPI_Finalize();

   return 0;
}
