#include "csp.hpp"


Csp::Csp()
{
	
}
Csp::~Csp()
{
	
}

bool Csp::estConsistant (vector<Solution> solutions)
{
	for (int i = 0; i < this->contraintes.size (); i++)
	{
		for (int j = 0; j < this->contraintes.at (i).arite; j++)
			for (int k = 0; k < this->contraintes.at (i).portee.at (j); k++)
			{
				for (int l = 0; l < solutions.size (); l++)
				{
					if (solutions.at (l).variable.valeur ==  this->contraintes.at (i).portee.at (j))
						cout << "True" << solutions.at (l).variable.valeur << " == " << this->contraintes.at (i).portee.at (j) << endl;
				}
			}
	}
	return true;
}

vector<Solution> Csp::backtrack (vector<Solution> solutions, vector<Var> variablesEnCours)
{
	if (variablesEnCours.empty())
	{
		// A est une solution
		return solutions;
	}
	else
	{
		Var var = variablesEnCours.back (); // Choisir x dans V
		variablesEnCours.pop_back ();
		
		cout << "Var : " << var.valeur << endl;
		
		for (int d = 1; d <= 9; d++)
		{
			cout << "Domaine : " << d << endl;
			Solution sol;
			sol.variable = var;
			sol.valeur = d;
			solutions.push_back (sol);
			if (estConsistant (solutions))
				cout << "estConsistant" << endl;
			// Si A U {x <- v} est consistant
				// solutions = backtrack (solutions, variablesEnCours);
			
		}
	}
	return solutions;
}

void Csp::forward_checking ()
{
	
}

void Csp::show ()
{
	for (int i = 0; i < this->variables.size(); i++)
		cout << "Variable : " << this->variables.at (i).valeur << endl;
	
	for (int i = 0; i < this->contraintes.size(); i++)
	{
		if (this->contraintes.at (i).nat == DIFFERENCE)
			cout << "Contrainte binaire de difference portant sur";
		else
			cout << "Contrainte n-aire de somme portant sur";
		for (int j = 0; j < this->contraintes.at (i).arite; j++)
			cout << " " << this->contraintes.at (i).portee.at (j);
		cout << " et de valeur " <<  this->contraintes.at (i).valeur << endl;  
	}
	
}

