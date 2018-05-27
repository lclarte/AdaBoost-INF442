
class Classifieur {

public:
	Classifieur(float, float, int);
	Classifieur(int);
	~Classifieur();

	int classifier(int);
	void regression(int, int, float);

	int get_indice();

private:
	float w1, w2;
	int indice; //indice est l'indice du vecteur caracteristique associee au classifieur 

};