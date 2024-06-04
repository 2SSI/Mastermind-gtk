/* Essai d'une combinaison pour le jeu de Mastermind : fichier source
*/

#include "stat-essai.h"

/*	______________________________________
	Accesseurs en lecture / écriture		*/

/* Cette fonction attribue une valeur à la propriété ordo d'une variable de type stat_essai
*/
void stat_essai_set_nb_couleurs_placees(stat_essai* stat, int val)
{
	stat->ordo = val;
}

/* Cette fonction renvoie la valeur de la propriété ordo d'une variable de type stat_essai
*/
int stat_essai_get_nb_couleurs_placees(stat_essai* stat)
{
	return stat->ordo;
}

/* Cette fonction attribue une valeur à la propriété non_ordo d'une variable de type stat_essai
*/
void stat_essai_set_nb_couleurs_correctes(stat_essai* stat, int val)
{
	stat->non_ordo = val;
}

/* Cette fonction renvoie la valeur de la propriété non_ordo d'une variable de type stat_essai
*/
int stat_essai_get_nb_couleurs_correctes(stat_essai* stat)
{
	return stat->non_ordo;
}

