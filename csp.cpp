#include "csp.hpp"


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

bool Csp::estConsistant ()
{
	for (int i = 0; i < currentVar->contraintes.size (); i++)
	{
		bool estVerifiable = true;
		
		for (int j = 0; ((j < currentVar->contraintes.at (i)->arite) && (estVerifiable)); j++)
		{
			// On verifie si la contrainte peut etre satisfaite :
			// toutes les variables associées a la contrainte sont instanciés
			if ((currentVar->contraintes.at (i)->portee.at (j))->solution == 0)
				estVerifiable = false;
		}
		if (estVerifiable)
		{
			if (currentVar->contraintes.at (i)->nat == DIFFERENCE)
			{
				if ( (currentVar->contraintes.at (i)->portee.at (0))->solution == (currentVar->contraintes.at (i)->portee.at (1))->solution)
					return false;
			}
			else
			{
				int somme = 0;
				for (int j = 0; j < currentVar->contraintes.at (i)->arite; j++)
				{
					somme += currentVar->contraintes.at (i)->portee.at (j)->solution;
				}
				//cout << "Somme a atteindre = " << contraintes.at (i).valeur << "; resultat = " << somme << endl;
				if (currentVar->contraintes.at (i)->valeur != somme)
					return false;
			}
			
		}
		//else
			//cout << "Contrainte " << i << " non verifiable" << endl;
	}
	return true;
}

vector<Var*> Csp::backtrack ()
{
	// Pile de recursion
	stack<Var*> process;
	
	int nbNoeuds = 1;
	int k = 0;
	
	process.push(variables.at (k));
	
	while(!process.empty())
	{
		currentVar = process.top(); // Choisir x dans V
		
		//cout << "\t Var : " << currentVar->valeur << endl;
		
		bool consistant = false;
		
		for(int i = currentVar->solution; i < currentVar->domaine.size() && !consistant; ++i)
		{
			currentVar->solution = currentVar->domaine.at (i);
			//cout << "\t\t Domaine : " << currentVar->solution << endl;
			
			consistant = estConsistant();
			
			if(consistant)
			{
				if(estComplet())
					return variables;
				// Empiler la nouvelle variable
				process.push( variables.at (++k));
				++nbNoeuds;
			}
			
		}
		if(!consistant)
		{
			// Retirer le haut de la pile
			process.pop();
			--k;
			currentVar->solution = 0;
		}
	}
	return variables;
}

bool Csp::domaineVide (Var* var)
{
	if (var->domaine.size() == 0)
	{
		cout << "\t\t\t Var " << var->valeur << " : DOMAINE VIDE" << endl;
		// On reinsere les valeurs a remettre
		int nbElementRetiresTour = nbElementRetires.at (tour);
		for (int i = 0; i < nbElementRetiresTour; i++)
		{
			ElementRetire element = elementsRetires.top();
			elementsRetires.pop();
			element.variable->domaine.push_back(element.domaine);
			//cout << element.domaine << " rajouté à la variable " << element.variable->valeur << endl;
		}
		nbElementRetires.at (tour) = 0;
		currentVar->solution = 0;
		return true;
	}
	return false;
}

bool Csp::verificationDomaine ()
{
	bool emptyDomain = false;
	for (int i = 0; i < currentVar->contraintes.size (); i++)
	{
		Contrainte* contrainte = currentVar->contraintes.at (i);
		if (contrainte->nat == DIFFERENCE)
		{
			for (int j = 0; j < contrainte->arite; j++)
			{
				if (contrainte->portee.at (j) != currentVar)
				{
					Var* varEnnemie = contrainte->portee.at (j);
					for (int k = 0; k < varEnnemie->domaine.size(); k++)
					{
						if (varEnnemie->domaine.at (k) == currentVar->solution)
						{
							ElementRetire element;
							element.variable = varEnnemie;
							element.domaine = varEnnemie->domaine.at (k);
							elementsRetires.push(element);
							//cout << "Tour = " << tour << "nbElementRetires = " << nbElementRetires.at (0) << endl;
							(nbElementRetires.at (tour))++;
							//cout << varEnnemie->domaine.at (k) << " retiré(diff) du domaine de " << varEnnemie->valeur << endl;
							varEnnemie->domaine.erase (varEnnemie->domaine.begin () + k);
							k--;
							if (domaineVide (varEnnemie))
								return true;
						}
					}
				}
			}
		}
		else // Contrainte de somme
		{
			int somme = 0;
			int varNonInnits = 0;
			Var* varTestee = NULL;
			for (int j = 0; (j < contrainte->arite) && (varNonInnits < 2); j++)
			{
				Var* var = contrainte->portee.at (j);
				if (var->solution == 0)
				{
					varNonInnits++;
					varTestee = var;
				}
				else
				{
					somme += var->solution;
				}
			}
			if (varNonInnits == 1)
			{
				if (varTestee != NULL)
				{
					for (int k = 0; k < varTestee->domaine.size(); k++)
					{
						if ((varTestee->domaine.at (k) + somme) != contrainte->valeur)
						{
							ElementRetire element;
							element.variable = varTestee;
							element.domaine = varTestee->domaine.at (k);
							elementsRetires.push(element);
							(nbElementRetires.at (tour))++;
							//cout << varTestee->domaine.at (k) << " retiré(somme) du domaine de " << varTestee->valeur << endl;
							varTestee->domaine.erase (varTestee->domaine.begin () + k);
							k--;
							if (domaineVide (varTestee))
								return true;
						}
					}
				}
			}
		}
	}
	return emptyDomain;
}

vector<Var*> Csp::forward_checking ()
{
	// Pile de recursion
	//stack<Var*> process;
	
	int nbNoeuds = 1;
	int k = 0;
	
	tour = 0;
	
	process.push(variables.at (k));
	nbElementRetires.push_back (0);

	while(!process.empty())
	{
		currentVar = process.top(); // Choisir x dans V
		cout << "\t Tour : " << tour << endl;
		
		cout << "\t Var : " << currentVar->valeur << endl;
		cout << "\t Domaine : " ;
		for (int j = 0; j < currentVar->domaine.size(); j++)
			cout << currentVar->domaine.at (j) << " ";
		cout << endl;
		
		bool domaineVide = true;
		
		int begin = 0;
		if (currentVar->solution > 0)
		{
			for (int i = 0; i < currentVar->domaine.size(); i++)
				if (currentVar->solution == currentVar->domaine.at (i))
					begin = i + 1;
		}
		//cout << "solution : " << currentVar->solution << endl;
		//cout << "begin : " << begin << endl;
		for(int i = begin; i < currentVar->domaine.size() && domaineVide; ++i)
		{
			currentVar->solution = currentVar->domaine.at (i);
			cout << "\t\t Domaine : " << currentVar->solution << endl;
			
			domaineVide = verificationDomaine();
			
			if(!domaineVide)
			{
				if(estComplet())
				{
					cout << "Fin estComplet" << endl;
					return variables;
				}
				// Empiler la nouvelle variable
				process.push( variables.at (++k));
				++nbNoeuds;
					++tour;
					nbElementRetires.push_back (0);
			}
			
		}
		if(domaineVide)
		{
			// Retirer le haut de la pile
			process.pop();
			--k;
			tour--;
			// On reinsere les valeurs a remettre
			int nbElementRetiresTour = nbElementRetires.at (tour);
			for (int i = 0; i < nbElementRetiresTour; i++)
			{
				ElementRetire element = elementsRetires.top();
				elementsRetires.pop();
				element.variable->domaine.push_back(element.domaine);
				//cout << element.domaine << " rajouté à la variable " << element.variable->valeur << endl;
			}
			nbElementRetires.at (tour) = 0;
			currentVar->solution = 0;
		}
	}
	cout << "Fin non Complet" << endl;
	return variables;
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

