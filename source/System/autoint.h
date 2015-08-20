/* --------------------------- */
/* Natura no Story             */
/* --------------------------- */

#ifndef H_AUTOINTNDS
#define H_AUTOINTNDS

class autoint
{
	public: //Fonctions
		//Fonctions autoint
		autoint() { }
		autoint(int val) { valeur = val;}
		autoint(autoint &autoval) { valeur = autoval.valeur;}
		int GetIntVal() { return valeur; }

		//Surcharge d'operateurs
		autoint operator=(int val) { valeur = val; return *this; }
		autoint operator=(autoint &autoval) { valeur = autoval.valeur; return *this; }
		autoint operator+(int val) { valeur += val; return *this; }
		autoint operator+(autoint &autoval) { valeur += autoval.valeur; return *this; }

	public: //Variables
		int valeur;     //Valeur de l'autoint


};

#endif