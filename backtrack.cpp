#include "csp.hpp"

/*
	estConsistant
	Retourne vrai si aucune contrainte est violée.
	Seules les contraintes associées à la variable courante sont testées.
	On vérifie aussi que toutes les variables de la portée de la contrainte sont instanciées avant d'éffectuer le test.
*/
bool Csp::estConsistant ()
{
	for (int i = 0; i < currentVar->contraintes.size (); i++)
	{
		bool estVerifiable = true;
		
		for (int j = 0; ((j < currentVar->contraintes.at (i)->arite) && (estVerifiable)); j++)
		{
			// On verifie si la contrainte peut etre satisfaite :
			// toutes les variables associées a la contrainte sont instanciées
			if ((currentVar->contraintes.at (i)->portee.at (j))->solution == 0)
				estVerifiable = false;
		}
		if (estVerifiable)
		{
			nbContraintesTestees++;
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

/*
	backtrack
	Retourne un vecteur de variables contenant la solution du problème.
*/
vector<Var*> Csp::backtrack ()
{
	
	int nbNoeuds = 1;
	int k = 0;
	
	nbContraintesTestees = 0;

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
				{
					cout << "Nombre de noeuds : " << nbNoeuds << endl;
					cout << "Nombre de contraintes testees : " << nbContraintesTestees << endl;
					return variables;
				}
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