#ifndef CSP_HPP
#define CSP_HPP

#include <iostream>
#include <vector>
#include <algorithm>    // std::sort

using namespace std;

enum nature {DIFFERENCE,NAIRES};

struct Var;
typedef struct Var Var;

typedef struct
{
	vector<int> ints; // Cases a verifier
	vector<Var*> portee; // Cases a verifier
	int arite; // Nombre de variables soumises `a la contrainte
	nature nat; // 0 : binaire de difference 1 : n-aires
	int valeur; 
	/* Si nat == DIFFERENCE, valeur = ? 
	 * Si nat == NAIRES, valeur = somme a atteindre
	 */
	
} Contrainte;

struct Var
{
	int valeur;
	int solution;
	float heuristique;
	vector<int> domaine;
	vector<Contrainte*> contraintes;
};

typedef struct
{
	Var variable;
	int valeur;
	
} Solution;

class Csp {
	private:
		vector<Var*> variables;
		vector<Contrainte*> contraintes;

		Var* currentVar;
		
		
	public:
		Csp();
		~Csp();
		
		void initialisation (char * nom_fichier);
		void parse (char * nom_fichier);
		void init_containtes ();
		void calculer_heuristique ();

		/* fonctions à compléter pour remplir vos structures de données */

		void Variable (int num);
		/* fonction permettant la création d'une nouvelle variable ayant pour numéro num */
		void Contrainte_Difference (int var1, int var2);
		/* fonction permettant la création d'une nouvelle contrainte binaire de différence entre les variables var1 et var2*/
		void Contrainte_Somme (int portee[], int arite, int val);
		/* fonction permettant la création d'une nouvelle contrainte n-aire de somme portant sur les variables contenant dans le tableau portee de taille arite et dont la valeur est val */
		
		inline vector<Var*> getVariables () {return variables;};
		void show ();
		
		bool estConsistant ();
		bool estComplet ();
		
		vector<Var*> backtrack ();
		vector<Var*> forward_checking ();
};

#endif

