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
	cout << "test1" << endl;
	for (int i = 0; i < variables.size (); i++)
	{
		cout << "i : " << variables.at(i).solution << endl;
		//if (solutions[i] == 0)
		if (variables.at(i).solution == 0)
		{
			cout << "test3" << endl;
			return false;
		}
	}
	cout << "test2" << endl;
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
			
			//if (solutions[this->contraintes.at (i).portee.at (j)] == 0)
			if (variables.at(this->contraintes.at (i).portee.at (j)).solution == 0)
				estVerifiable = false;
		}
		if (estVerifiable)
		{
			cout << "Contrainte " << i << " verifiable" << endl;
			
			if (this->contraintes.at (i).nat == DIFFERENCE)
			{
				if (variables.at(this->contraintes.at (i).portee.at (0)).solution == variables.at(this->contraintes.at (i).portee.at (1)).solution)
				{
					cout << "Contrainte binaire de difference rejetee; valeur = " << variables.at(this->contraintes.at (i).portee.at (0)).solution << endl;
					return false;
				}
			}
			else
			{
				int somme = 0;
				for (int j = 0; j < this->contraintes.at (i).arite; j++)
				{
					somme += variables.at(this->contraintes.at (i).portee.at (j)).solution;
				}
				cout << "Somme a atteindre = " << contraintes.at (i).valeur << "; resultat = " << somme << endl;
				if (contraintes.at (i).valeur != somme)
					return false;
			}
			
		}
		//else
			//cout << "Contrainte " << i << " non verifiable" << endl;
	}
	return true;
}

int* Csp::backtrack ()
{
	// init struct solutions
	for (int i = 0; i < variables.size (); i++)
		variables.at (i).solution = 0;
	
	vector<Var> variablesEnCours = variables;
	stack<Var> pileVariables;
	
	int solutions [variables.size ()];
	// init solutions
	for (int i = 0; i < variables.size (); i++)
		solutions[i] = 0;
	
	
	
	/*if (pileVariables.empty())
	{
		// A est une solution
		return solutions;
	}
	else
	{*/
	int idVar = 0;

	Var varInit = variablesEnCours.back (); // Choisir x dans V
	//variablesEnCours.pop_back ();
	
	// init pile
	//for (int i = 0; i < variables.size (); i++)
		//pileVariables.push(variables.at (i));
	//pileVariables.push(varInit);
	pileVariables.push(variablesEnCours.at (idVar++));
	
	while (!pileVariables.empty())
	{
		/*Var* var = &pileVariables.back (); // Choisir x dans V
		pileVariables.pop_back ();*/
		Var* var = &pileVariables.top ();
		
		cout << "\t\tVar : " << var->valeur << endl;
		
		bool consistance = false;
		
		for (int d = (var->solution + 1); ((d <= 9) && (!consistance)); d++)
		{
			cout << "\tDomaine : " << d << endl;
			//solutions.erase (&var->valeur);
			//solutions.insert (&var->valeur, d);
			int oldValue = solutions [var->valeur];
			//solutions [var->valeur] = d;
			var->solution = d;
			consistance = estConsistant (solutions);
			if (consistance)
			{
				cout << "estConsistant" << endl;
				
				if (estComplet (solutions) )
				{
					cout << "estComplet" << endl;
					return solutions;
				}
				
				// Retirer la variable dans variablesEnCours
				/*for (int i = 0; i <= variablesEnCours.size (); i++)
					if (variablesEnCours.at (i).valeur == var->valeur)
						variablesEnCours.erase (variablesEnCours.begin() + i);*/
						
				// Empiler la nouvelle variable
				pileVariables.push(variablesEnCours.at (idVar++));

				

			}
			else
			{
				//solutions [var->valeur] = oldValue;
				var->solution = oldValue;
				
				// Retirer le haut de la pile
				pileVariables.pop ();
				idVar = (--idVar < 0?0:idVar) ;
				
			}
			// Si A U {x <- v} est consistant
				// solutions = backtrack (solutions, pileVariables);
			
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

