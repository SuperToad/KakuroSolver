#include "csp.hpp"
//#include "backtrack.cpp"


Csp::Csp()
{
	
}
Csp::~Csp()
{
	
}

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
	
	/*for (int i = 0; i < this->contraintes.size(); i++)
	{
		if (this->contraintes.at (i)->nat == DIFFERENCE)
			cout << "Contrainte binaire de difference portant sur";
		else
			cout << "Contrainte n-aire de somme portant sur";
		for (int j = 0; j < this->contraintes.at (i)->arite; j++)
			cout << " " << this->contraintes.at (i)->ints.at (j);
		cout << " et de valeur " <<  this->contraintes.at (i)->valeur << endl;  
	}
	cout << endl; */ 
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

bool domdeg (Var* i,Var* j) { return (i->heuristique > j->heuristique); }

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

