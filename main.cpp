#include "csp.hpp"
#include "parser.hpp"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main (int argc, char * argv[])
{
	if (argc < 2)
	{
	fprintf (stderr,"Utilisation: %s [-bt] [-fc] [-h] fichier\nLes parametres entoures de [] sont des options.\n",argv[0]);
	exit (-1);
	}

	bool flag_BT = false;
	bool flag_FC = false;
	bool flag_H = false;

	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-bt") == 0)
			flag_BT = true;
		else if (strcmp(argv[i], "-fc") == 0)
			flag_FC = true;
		else if (strcmp(argv[i], "-h") == 0)
			flag_H = true;

	}

	Csp *csp = new Csp ();

	csp->initialisation (argv[argc - 1]);
	//csp->show ();

	if (flag_H)
		csp->calculer_heuristique();

	if (flag_BT)
	{
		cout << "\t~~ Debut Backtrack ~~" << endl;

		vector<Var*> solutionsBT = csp->backtrack ();

		// Affichage solution
		for (int i = 0; i < solutionsBT.size (); i++)
			cout << "Solution pour " << solutionsBT.at(i)->valeur << " : " << solutionsBT.at(i)->solution << endl;
	}
	if (flag_FC)
	{
		cout << "\t~~ Debut Forward Checking ~~" << endl;

		vector<Var*> solutionsFC = csp->forward_checking ();

		// Affichage solution
		for (int i = 0; i < solutionsFC.size (); i++)
			cout << "Solution pour " << solutionsFC.at(i)->valeur << " : " << solutionsFC.at(i)->solution << endl;
	}
	

	return 0;
}
