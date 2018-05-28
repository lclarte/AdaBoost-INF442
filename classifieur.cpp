#include "classifieur.h"

Classifieur::~Classifieur() {

}

Classifieur::Classifieur(int i) {
	Classifieur(1.0, 0, i);
}

Classifieur::Classifieur(float w_1, float w_2, int i) {
	this->w1 = w_1;
	this->w2 = w_2;
	this->indice = i;
}

int Classifieur::classifier(int x) {
	return this->w1*x + this->w2 >= 0 ? 1 : -1;
}

void Classifieur::regression(int x, int c, float epsilon) {
	int h = this->classifier(x);
	this->w1 -= epsilon*(h-c)*x;
	this->w2 -= epsilon*(h-c);
}

int Classifieur::get_indice() {
	return this->indice;
}

float Classifieur::get_w1() {
	return this->w1;
}

float Classifieur::get_w2() {
	return this->w2;
}

// c est 1 si visage, -1 sinon
int Classifieur::erreur(vector<int> carac, int c) {
	return this->classifier(carac.at(this->indice)) == c ? 0 : 1;
}