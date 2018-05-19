#include "csp.hpp"
//#include "backtrack.cpp"


Csp::Csp()
{
	
}
Csp::~Csp()
{
	
}

/*
	estComplet
	Retourne vrai si toutes les variables de l'instance CSP ont une valeur
	Retourne faux sinon
*/
bool Csp::estComplet ()
{
	for (int i = 0; i < variables.size (); i++)
	{
		//if (solutions[i] == 0)
		if (variables.at(i)->solution == 0)
		{
			return false;
		}
	}
	return true;
}



/*
	show
	Affiche les variables et leur domaine, 
	ainsi que les contrantes et les variables de leur portée
*/
void Csp::show ()
{
	for (int i = 0; i < this->variables.size(); i++)
	{
		Var* var = this->variables.at (i);
		
		cout << "Variable : " << var->valeur << endl;
		cout << "\tDomaine : " ;
		for (int j = 0; j < var->domaine.size(); j++)
			cout << var->domaine.at (j) << " ";
		cout << endl;
	}
	for (int i = 0; i < this->contraintes.size(); i++)
	{
		if (this->contraintes.at (i)->nat == DIFFERENCE)
			cout << "Contrainte binaire de difference portant sur";
		else
			cout << "Contrainte n-aire de somme portant sur";
		for (int j = 0; j < this->contraintes.at (i)->arite; j++)
			cout << " " << this->contraintes.at (i)->portee.at (j)->valeur;
		cout << " et de valeur " <<  this->contraintes.at (i)->valeur << endl;  
	}
	
}


/*
	domdeg
	Fonction de comparaison entre les valeur d'heuristique de deux variables.
	Utilisée pour trier les variables selon leur valeur d'heuristique
*/
bool domdeg (Var* i,Var* j) { return (i->heuristique > j->heuristique); }

/*
	calculer_heuristique
	Calcule la valeur d'heuristique de chaque variable selon dom/deg
	Trie ensuite les variable pour leur utilisation dans les fonction de résolution
*/
void Csp::calculer_heuristique ()
{
  for (int i = 0; i < this->variables.size(); i++)
  {
    variables.at (i)->heuristique = (float)(variables.at (i)->domaine.size() / (float)variables.at (i)->contraintes.size());
  }

  sort (variables.begin(), variables.end(), domdeg);

  for (int i = 0; i < this->variables.size(); i++)
  {
    cout << "Heuristique de " << variables.at (i)->valeur << " : " << variables.at (i)->heuristique << endl;
  }
}

