#ifndef CSP_HPP
#define CSP_HPP

#include <iostream>
#include <vector>

using namespace std;

enum nature {DIFFERENCE,NAIRES};

typedef struct
{
	int valeur;
	
} Var;

typedef struct
{
	vector<int> portee; // Cases a verifier
	int arite; // Nombre de variables soumises `a la contrainte
	nature nat; // 0 : binaire de difference 1 : n-aires
	int valeur; 
	/* Si nat == DIFFERENCE, valeur = ? 
	 * Si nat == NAIRES, valeur = somme a atteindre
	 */
	
} Contrainte;

typedef struct
{
	Var variable;
	int valeur;
	
} Solution;

class Csp {
	private:
		vector<Var> variables;
		vector<Contrainte> contraintes;
		
		
	public:
		Csp();
		~Csp();
		
		void parse (char * nom_fichier);

		/* fonctions à compléter pour remplir vos structures de données */

		void Variable (int num);
		/* fonction permettant la création d'une nouvelle variable ayant pour numéro num */
		void Contrainte_Difference (int var1, int var2);
		/* fonction permettant la création d'une nouvelle contrainte binaire de différence entre les variables var1 et var2*/
		void Contrainte_Somme (int portee[], int arite, int val);
		/* fonction permettant la création d'une nouvelle contrainte n-aire de somme portant sur les variables contenant dans le tableau portee de taille arite et dont la valeur est val */
		
		inline vector<Var> getVariables () {return variables;};
		void show ();
		
		bool estConsistant (int* solutions);
		
		int* backtrack ();
		void forward_checking ();
};

#endif

