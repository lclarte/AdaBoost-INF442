#include <vector>

using namespace std;

class Classifieur {

public:
	Classifieur(float, float, int);
	Classifieur(int);
	~Classifieur();

	int classifier(int);
	void regression(int, int, float);

	int get_indice();
	float get_w1();
	float get_w2();
	int erreur(vector<int>, int);

private:
	float w1, w2;
	int indice; //indice est l'indice du vecteur caracteristique associee au classifieur

};