/* Combinaison de couleurs pour le jeu de Mastermind : fichier source
*/

#include "combinaison.h"

#include <stdlib.h>  /* pour la fonction rand() */

/*	______________________________________
	Intialisation						*/

/* Cette fonction initialise toutes les composantes d'une combinaison à COULEUR_INDETERMINEE
*/
void combinaison_initialiser(combinaison c)
{
	int i;

    /* reinitialisation de chaque composante de la combinaison */
    for (i = 0 ; i < TAILLE_COMBI ; i ++)
	{
        c[i] = COULEUR_INDETERMINEE;
	}
}

/* Cette fonction attribue aléatoirement une couleur dans {COULEUR_MIN ,..., COULEUR_MAX} à chaque composante d'une combinaison
*/
void combinaison_tirer_au_hasard(combinaison c)
{
	int i;

    /* tirage au hasard de chaque composante de la combinaison */
    for (i = 0 ; i < TAILLE_COMBI ; i ++)
	{
        c[i] = (rand() % NB_COULEURS) +COULEUR_MIN;
	}
}

/*	______________________________________
	Accesseurs en lecture / écriture		*/

/* Cette fonction affecte une couleur à une composante d'une combinaison
*/
void combinaison_set_couleur(combinaison c, int cle, couleur val)
{
	int i;	/* indice identifiant la composante concernée par la mise à jour dans le tableau c : pour la clareté du code */

    /* mise à jour demandée de la combinaison */
	i = cle -1;
	c[i] = val;
}

/* Cette fonction renvoie la couleur d'une composante d'une combinaison
*/
couleur combinaison_get_couleur(combinaison c, int cle)
{
	int i;	/* indice identifiant la composante concernée par la mise à jour dans le tableau c : pour la clareté du code */

	/* on retourne la valeur de la composante demandée */
	i = cle -1;
	return c[i];
}

/*	______________________________________
	Accesseurs en lecture avancés		*/

/* Cette fonction teste si toutes les couleurs d'une combinaison sont à valeur dans {COULEUR_MIN ,..., COULEUR_MAX}
*/
int combinaison_est_valide(combinaison c)
{
	int res = 1;		/* variable contenant le résultat à retourner : res vaut 1 "jusqu'à preuve du contraire"  */
	int i;

    /* on recherche dans la combinaison une valeur à l'extérieur de l'intervalle [COULEUR_MIN, COULEUR_MAX] */
    for (i = 0 ; (i < TAILLE_COMBI) && (res == 1) ; i ++)
	{
        res = (c[i] >= COULEUR_MIN) && (c[i] <= COULEUR_MAX) ;
	}

	/* on retourne le résultat */
	return res;
}

/* Cette fonction compare la combinaison c_source à la combinaison c_cible en tenant compte de l'ordre des éléments
*/
int combinaison_cmp_ordonnee(combinaison c_source, combinaison c_cible)
{
	int res = 0;		/* variable contenant le résultat à retourner, que l'on augmente de 1 chaque fois que l'on rencontre un nouvel indice 
						sur lequel les deux tableaux c_source et c_cible coincident  */
	int i;

    /* on compte le nombre d'indices i dans {0 ,..., TAILLE_COMBI -1} pour lesquels c_source[i] et c_cible[i] coïncident */
    for (i = 0 ; i < TAILLE_COMBI ; i ++)
	{
        if (c_source[i] == c_cible[i])
		{
			res ++;
		}
	}

	/* on retourne le résultat */
	return res;
}

/* Cette fonction compare la restriction des combinaisons c_source et c_cible aux couleurs {COULEUR_MIN ,..., COULEUR_MAX} sans tenir compte de l'ordre des éléments
*/
int combinaison_cmp_ensembliste(combinaison c_source, combinaison c_cible)
{
	int res = 0;		/* variable contenant le résultat à retourner, que l'on augment pour chaque couleur coul dans {COULEUR_MIN ,..., COULEUR_MAX}
						du plus petit nombre d'occurrences de cette couleur dans les deux tableaux c_source et c_cible	*/
	couleur coul;
	int i;
	int nb_source =0, nb_cible =0;	/* nombres de cases des tableaux c_source et c_cible qui ont déjà été parcourues 
										(pour éviter de considérer inutilement des couleurs lorsque l'un des deux tableaux a déjà été entièrement parcouru)  */
	int nb_occ_source, nb_occ_cible;

    /* Dénombrement du nombre d'occurrences de chaque couleur dans les tableaux c_source et c_cible :
		- seulent nous intéressent les couleurs COULEUR_MIN ,..., COULEUR_MAX
		- on sort de la boucle dès lors que les cases de c_source, ou celles de c_cible, ont toutes été déjà dénombrées  */
    for (coul = COULEUR_MIN ; (coul <= COULEUR_MAX) && (nb_source < TAILLE_COMBI) && (nb_cible < TAILLE_COMBI) ; coul ++)
	{
    		/* dénombrement du nombre d'occurrences de la couleur coul dans les tableaux c_source et c_cible */
		nb_occ_source = 0;
		nb_occ_cible = 0;

	    for (i = 0 ; i < TAILLE_COMBI ; i ++)
		{
		    nb_occ_source = nb_occ_source +(c_source[i] == coul);
		    nb_occ_cible = nb_occ_cible +(c_cible[i] == coul);
		}

		/* on augment res du nombre d'occurrences de la couleur coul dans
			- c_source si nb_occ_source <= nb_occ_cible
			- c_cible si nb_occ_source >= nb_occ_cible		*/
		res = res + (nb_occ_source <= nb_occ_cible ? nb_occ_source : nb_occ_cible);

		/* mise à jour des nombres de cases des tableaux c_source et c_cible qui ont été dénombrées jusque lors  */
		nb_source = nb_source +nb_occ_source;
		nb_cible = nb_cible +nb_occ_cible;
	}

	/* on retourne le résultat */
	return res;
}

