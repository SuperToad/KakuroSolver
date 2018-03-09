#include <stack>
#include "csp.hpp"


Csp::Csp()
{
	
}
Csp::~Csp()
{
	
}

bool Csp::estComplet (int* solutions)
{
	for (int i = 0; i < variables.size (); i++)
		if (solutions[i] == -1)
			return false;
	return true;
}

bool Csp::estConsistant (int* solutions)
{
	for (int i = 0; i < this->contraintes.size (); i++)
	{
		bool estVerifiable = true;
		
		for (int j = 0; ((j < this->contraintes.at (i).arite) && (estVerifiable)); j++)
		{
			// On verifie si la contrainte peut etre satisfaite :
			// toutes les variables associées a la contrainte sont instanciés
			
			if (solutions[this->contraintes.at (i).portee.at (j)] == -1)
				estVerifiable = false;
		}
		if (estVerifiable)
		{
			cout << "Contrainte " << i << " verifiable" << endl;
			
			if (this->contraintes.at (i).nat == DIFFERENCE)
			{
				if (solutions[this->contraintes.at (i).portee.at (0)] == solutions[this->contraintes.at (i).portee.at (1)])
				{
					cout << "Contrainte binaire de difference rejetee; valeur = " << solutions[this->contraintes.at (i).portee.at (0)] << endl;
					return false;
				}
			}
			else
			{
				int somme = 0;
				for (int j = 0; j < this->contraintes.at (i).arite; j++)
				{
					somme += solutions[this->contraintes.at (i).portee.at (j)];
				}
				cout << "Somme a atteindre = " << contraintes.at (i).valeur << "; resultat = " << somme << endl;
				if (contraintes.at (i).valeur != somme)
					return false;
			}
			
		}
		else
			cout << "Contrainte " << i << " non verifiable" << endl;
	}
	return true;
}

int* Csp::backtrack ()
{
	vector<Var> variablesTot = variables;
	stack<Var> variablesEnCours;
	
	int solutions [variables.size ()];
	// init solutions
	for (int i = 0; i < variables.size (); i++)
		solutions[i] = -1;
	
	/*if (variablesEnCours.empty())
	{
		// A est une solution
		return solutions;
	}
	else
	{*/

	Var varInit = variablesTot.back (); // Choisir x dans V
	variablesTot.pop_back ();

	variablesEnCours.push(varInit);
	while (!variablesEnCours.empty())
	{
		/*Var* var = &variablesEnCours.back (); // Choisir x dans V
		variablesEnCours.pop_back ();*/
		Var* var = &variablesEnCours.top ();
		
		cout << "\t\tVar : " << var->valeur << endl;
		
		bool consistance = false;
		
		for (int d = 1; ((d <= 9) && (!consistance)); d++)
		{
			cout << "\tDomaine : " << d << endl;
			//solutions.erase (&var->valeur);
			//solutions.insert (&var->valeur, d);
			int oldValue = solutions [var->valeur];
			solutions [var->valeur] = d;
			consistance = estConsistant (solutions);
			if (consistance)
			{
				cout << "estConsistant" << endl;

				// Retirer le haut de la pile
				variablesEnCours.pop ();

				if (estComplet (solutions) )
				{
					cout << "estComplet" << endl;
					return solutions;
				}

			}
			else
				solutions [var->valeur] = oldValue;
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

