#include "csp.hpp"
#include "parser.hpp"
#include <stdio.h>
#include <stdlib.h>


int main (int argc, char * argv[])
{
	if (argc != 2)
	{
	fprintf (stderr,"Utilisation: %s fichier\n",argv[0]);
	exit (-1);
	}

	Csp *csp = new Csp ();

	csp->parse (argv[1]);
	csp->show ();
	
	vector<Solution> solutions;
	csp->backtrack (solutions, csp->getVariables ());

	return 0;
}
