#include "parser.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


enum couleur {BLANCHE,NOIRE};   /* les deux couleurs possibles pour les cases */

typedef struct
{
  enum couleur coul;      /* la nature de la case */
  int num;                /* le numero de la case s'il s'agit d'une case noire */
  int somme_horizontale;  /* la valeur de la somme horizontale le cas écheant, -1 sinon */
  int somme_verticale;    /* la valeur de la somme verticale le cas écheant, -1 sinon */
} Case;


void Csp::parse (char * nom_fichier)
{
  FILE * f;         /* le descripteur du fichier à lire */
  int c;            /* le caractère courant */
  int nb_lignes;    /* le nombre de lignes de la grille */
  int nb_colonnes;  /* le nombre de colonnes de la grille */
  int num_ligne;    /* le numero de la ligne courante */
  int num_colonne;  /* le numero de la colonne courante */
  int nb_variables; /* le nombre de variables déjà trouvées */
  int somme;        /* une somme */
  
  Case ** grille;   /* la grille de jeu */
  int * portee;     /* la portee d'une contrainte */
  int arite;        /* l'arité d'une contrainte */
  int i,j;          /* des compteurs de boucle */
  
  f = fopen (nom_fichier,"r");
  
  if (f == NULL)
  {
    fprintf (stderr,"Fichier %s non trouve\n",nom_fichier);
    exit (-1);
  }

  /* on calcule la taille de la grille */
  
  nb_lignes = 0;
  nb_colonnes = 0;
  
  c = fgetc(f);
  
  while (! feof (f))
  {
    if (c == '\n')
      nb_lignes++;
    else      
      if ((nb_lignes == 0) && ((c == '.') || (c == '\\')))
        nb_colonnes++;

    c = fgetc(f);
  }
  fclose (f);
  
  printf ("Taille : %d x %d\n",nb_lignes,nb_colonnes);
  

  /* remplissage de la grille */

  grille = (Case **) malloc (sizeof(Case *) * nb_lignes);
  for (num_ligne = 0; num_ligne < nb_lignes; num_ligne++)
    grille[num_ligne] = (Case *) malloc (sizeof(Case) * nb_colonnes);

  f = fopen (nom_fichier,"r");
  
  if (f == NULL)
  {
    fprintf (stderr,"Fichier %s non trouve\n",nom_fichier);
    exit (-1);
  }  
  
  num_ligne = 0;
  num_colonne = 0;
  nb_variables = 0;
  
  c = fgetc(f);
  
  while (! feof (f))
  {
    if (c == '\n')
    {
      num_ligne++;
      num_colonne = 0;
    }
    else
      if (c == ' ')
        num_colonne++;
      else
        if (c == '.')     /* case blanche */
        {
          grille[num_ligne][num_colonne].coul = BLANCHE;
          grille[num_ligne][num_colonne].num = nb_variables;
          grille[num_ligne][num_colonne].somme_horizontale = -1;
          grille[num_ligne][num_colonne].somme_verticale = -1;
          
          Variable(nb_variables);
          
          nb_variables++;
        }
        else
          if (c == '\\')    /* case noire de la forme \y ou \ */
          {
            grille[num_ligne][num_colonne].coul = NOIRE;
            grille[num_ligne][num_colonne].num = -1;
            grille[num_ligne][num_colonne].somme_verticale = -1;

            c = fgetc(f);
            
            if (isdigit(c))    /* case noire de la forme \y */
            {
              ungetc (c,f);
              fscanf (f,"%d",&somme);
              grille[num_ligne][num_colonne].somme_horizontale = somme;
            }
            else  /* case noire de la forme \ */
            {
              grille[num_ligne][num_colonne].somme_horizontale = -1;
              
              ungetc (c,f);
            }
          }
          else
            if (isdigit(c))    /* case noire de la forme x\ ou x\y */
            {
              grille[num_ligne][num_colonne].coul = NOIRE;
              grille[num_ligne][num_colonne].num = -1;
              
              ungetc (c,f);
              fscanf (f,"%d",&somme);
              grille[num_ligne][num_colonne].somme_verticale = somme;
              
              c = fgetc(f);   /* on lit le caractère \ */
              
              c = fgetc(f);
              
              if (isdigit(c))  /* case noire de la forme x\y */
              {
                ungetc (c,f);
                fscanf (f,"%d",&somme);
                grille[num_ligne][num_colonne].somme_horizontale = somme;
              }
              else   /* case noire de la forme x\ */
              {
                grille[num_ligne][num_colonne].somme_horizontale = -1;
                
                ungetc (c,f);
              }
            }
    
    c = fgetc(f);
  }
  fclose (f);  
  
  printf ("Nombre de variables trouvées : %d\n",nb_variables);
  
  
  /* on crée les contraintes */
  
  if (nb_lignes < nb_colonnes)
    portee = (int *) malloc (sizeof(int) * nb_colonnes);
  else portee = (int *) malloc (sizeof(int) * nb_lignes);
  
  for (num_ligne = 0; num_ligne < nb_lignes; num_ligne++)
    for (num_colonne = 0; num_colonne < nb_colonnes; num_colonne++)
      if (grille[num_ligne][num_colonne].coul == NOIRE)
      {
        if (grille[num_ligne][num_colonne].somme_horizontale != -1)
        {
          arite = 0;
          i = num_colonne+1;
          while ((i < nb_colonnes) && (grille[num_ligne][i].coul == BLANCHE))
          {
            portee[arite] = grille[num_ligne][i].num;
            arite++;
            
            j = i+1;
            while ((j < nb_colonnes) && (grille[num_ligne][j].coul == BLANCHE))
            {
              Contrainte_Difference (grille[num_ligne][i].num,grille[num_ligne][j].num);
              j++;
            }
            i++;
          }
          
          Contrainte_Somme (portee,arite,grille[num_ligne][num_colonne].somme_horizontale);
        }
        
        if (grille[num_ligne][num_colonne].somme_verticale != -1)
        {
          arite = 0;
          i = num_ligne+1;
          while ((i < nb_lignes) && (grille[i][num_colonne].coul == BLANCHE))
          {
            portee[arite] = grille[i][num_colonne].num;
            arite++;
            
            j = i+1;
            while ((j < nb_lignes) && (grille[j][num_colonne].coul == BLANCHE))
            {
              Contrainte_Difference (grille[i][num_colonne].num,grille[j][num_colonne].num);
              j++;
            }
            i++;
          }
          
          Contrainte_Somme (portee,arite,grille[num_ligne][num_colonne].somme_verticale);          
        }
      }
      
      
  /* liberation de la mémoire allouée pour la grille et la portée des contraintes */
  free (portee);
  
  for (num_ligne = 0; num_ligne < nb_lignes; num_ligne++)
    free (grille[num_ligne]);
  free (grille);
}


/* fonctions à compléter pour remplir vos structures de données */


void Csp::Variable (int num)
/* fonction permettant la création d'une nouvelle variable ayant pour numéro num */
{
  printf ("Variable %d\n",num);
  
  Var* var = new Var;
  var->valeur = num;
  var->solution = 0;
  
  for (int i = 1; i < 10; i++)
  {
    var->domaine.push_back (i);
    var->domaineComplet.push_back (i);
  }
  
  this->variables.push_back (var);
  printf ("Ajout Variable %d\n",num);
  
}


void Csp::Contrainte_Difference (int var1, int var2)
/* fonction permettant la création d'une nouvelle contrainte binaire de différence entre les variables var1 et var2*/
{
  printf ("Contrainte binaire de difference entre %d et %d\n",var1,var2);
  
  
  
  Contrainte *contrainte = new Contrainte;
  contrainte->ints.push_back(var1);
  contrainte->ints.push_back(var2);
  contrainte->arite = 2;
  contrainte->nat = DIFFERENCE;
  contrainte->valeur = 0; // temp
  
  this->contraintes.push_back (contrainte);
}


void Csp::Contrainte_Somme (int portee [], int arite, int val)
/* fonction permettant la création d'une nouvelle contrainte n-aire de somme portant sur les variables contenant dans le tableau portee de taille arite et dont la valeur est val */
{
  int i;
  
  printf ("Contrainte n-aire de somme portant sur");
  for (int i = 0; i < arite; i++)
    printf (" %d",portee[i]);
  printf (" et de valeur %d\n",val);  
  
  Contrainte *contrainte = new Contrainte;
  for (int i = 0; i < arite; i++)
    contrainte->ints.push_back(portee[i]);
  contrainte->arite = arite;
  contrainte->nat = NAIRES;
  contrainte->valeur = val;
  
  this->contraintes.push_back (contrainte);
  
}

void Csp::init_containtes ()
{
  for (int i = 0; i < contraintes.size(); i++)
  {
    //cout << "lol" << endl;  
    for (int j = 0; j < contraintes.at (i)->arite; j++)
    {
      //cout << "lol" << endl;  
      //cout  << contraintes.at (i)->ints.at (j) << endl;
      variables.at(contraintes.at (i)->ints.at (j))->contraintes.push_back ( contraintes.at (i));
      contraintes.at (i)->portee.push_back ( variables.at(contraintes.at (i)->ints.at (j)));
    }
  }

  for (int i = 0; i < this->variables.size(); i++)
  {
    cout << "Variable : " << i << " : " << variables.at (i)->contraintes.size() << endl;
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


void Csp::initialisation (char * nom_fichier)
{
  parse (nom_fichier); 
  init_containtes ();
  //calculer_heuristique ();
}
