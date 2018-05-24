
class Classifieur {

public:
	Classifieur(float, float);
	Classifieur();
	~Classifieur();

	int classifier(int);
	void regression(int, int, float);

private:
	float w1, w2;

};