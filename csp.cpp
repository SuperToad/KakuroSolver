#include "csp.hpp"


Csp::Csp()
{
	
}
Csp::~Csp()
{
	
}

void Csp::backtrack ()
{
	
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

