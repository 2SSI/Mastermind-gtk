/* Jeu de Mastermind : fichier source

	Une fonction de classe 'static' est une fonction qui n'est visible (et donc, à laquelle on ne peut faire appel) que du fichier dans lequel elle est définie.
*/

#include "mastermind.h"

#include <stdlib.h>  /* pour la fonction rand() */

/*	______________________________________________________
	Déclaration des fonctions statiques (sous-routines) */

/* Cette fonction permet de passer à l'étape suivante dans le cours d'une une partie de mastermind. Elle est appelée quand :
	- l'on acte la combinaison secrète : 		fonctions 	mastermind_initialiser_avec_secret()		mastermind_valider_secret()
	- l'on acte l'essai en cours du joueur : 	fonction 	mastermind_valider_essai()
	Pré-conditions :
	- mm contient bien l'adresse d'une variable mastermind
	- 		la partie est en état ETAT_MM_SECRET et la combinaison secrète est valide
		ou	la partie est en état ETAT_MM_ENCOURS et le numero d'essai en cours est à valeur dans {1 ,..., NB_ESSAIS} et l'essai en cours est valide
	Post-conditions :
	- Si la partie est en état ETAT_MM_SECRET en entrée de fonction, alors en sortie de fonction : 
		- le numéro d'essai en cours devient 1, les composantes de cet essai sont toutes initialisées à COULEUR_INDETERMINEE
		- l'état de la partie est ETAT_MM_ENCOURS
	- Si la partie est en état ETAT_MM_ENCOURS en entrée de fonction, alors en sortie de fonction :
		- les statistiques de l'essai que le joueur vient de valider sont mises à jour
		- le numéro d'essai en cours est incrémenté de 1
		- Si l'essai que le joueur vient de valider coïncide avec la combinaison secrète, alors l'état de la partie devient ETAT_MM_GAGNE 
		  Sinon et s'il s'agissait du dernier essai du joueur, alors l'état de la partie devient ETAT_MM_PERDU
		  Sinon, les composantes du prochain essai du joueur sont toutes initialisées à COULEUR_INDETERMINEE
*/
static void mastermind_etape_suivante(mastermind* mm);

/*	__________________________________________________
	Initialisation									*/

/* Cette fonction initialise toutes les composantes de la combinaison secrète d'un jeu de mastermind à la valeur COULEUR_INDETERMINEE
*/
void mastermind_initialiser(mastermind* mm)
{
    /* initialisation de la combinaison secrete */
	combinaison_initialiser(mm->plateau[NB_ESSAIS]);

	/* mise à jour de l'état de la partie et du numéro d'essai en cours */
	mm->etat = ETAT_MM_SECRET;
	mm->num_essai_encours = 0;
}

/* Cette fonction lance une nouvelle partie de mastermind en affectant à chaque composante de la combinaison secrète une valeur tirée au hasard 
		dans {COULEUR_MIN ,..., COULEUR_MAX} 
*/
void mastermind_initialiser_avec_secret(mastermind* mm)
{
    /* mise a jour aléatoire de la combinaison secrete */
	combinaison_tirer_au_hasard(mm->plateau[NB_ESSAIS]);

	/* lancement de la partie */
	mm->etat = ETAT_MM_SECRET;	/* pour un bon usage de la fonction mastermind_etape_suivante(); */
	mastermind_etape_suivante(mm);
}

/*	__________________________________________________
	Définir une combinaison secrète					*/

/* Cette fonction affecte une couleur donnée à la composante n°cle (à valeur dans {1 ,..., TAILLE_COMBI}) de la combinaison secrète d'un jeu de mastermind
*/
void mastermind_set_secret(mastermind* mm, int cle, couleur val)
{
    /* mise a jour de la combinaison secrète  */
	combinaison_set_couleur(mm->plateau[NB_ESSAIS], cle, val);

	/* mise à jour de l'état de la partie et du numéro d'essai en cours */
	mm->etat = ETAT_MM_SECRET;
	mm->num_essai_encours = 0;
}

/* Cette fonction renvoie 1 si la combinaison secrète d'un jeu de mastermind contient des couleurs toutes à valeur dans {COULEUR_MIN ,..., COULEUR_MAX} et 0 sinon
*/
int mastermind_est_secret_valide(mastermind* mm)
{
	return combinaison_est_valide(mm->plateau[NB_ESSAIS]);
}

/* Cette fonction lance une nouvelle partie de mastermind en actant la combinaison secrète courante. Toutefois cette fonction ne fait rien si 
		la partie n'est pas en état initial ETAT_MM_SECRET, ou la combinaison secrète contient une composante < COULEUR_MIN ou > COULEUR_MAX.
*/
void mastermind_valider_secret(mastermind* mm)
{
	/* Si le jeu est en attente de validation de la combinaison secrète et cette combinaison est valide : une nouvelle partie peut commencer */
	if((mm->etat == ETAT_MM_SECRET) && (mastermind_est_secret_valide(mm) == 1))
	{
		mastermind_etape_suivante(mm);
	}
}

/*	__________________________________________________
	Proposer une combinaison (essai en cours)		*/

/* Cette fonction affecte une couleur donnée à la composante n°cle (à valeur dans {1 ,..., TAILLE_COMBI}) de l'essai en cours d'une partie de mastermind. 
	Toutefois cette fonction ne fait rien si la partie n'est pas en état initial ETAT_MM_ENCOURS.
*/
void mastermind_set_essai_encours(mastermind* mm, int cle, couleur val)
{
	int i;	/* indice identifiant dans les tableaux (*mm).plateau et (*mm).stat l'essai en cours du joueur : pour la clareté du code */

	/* On ne peut modifier un essai en cours que si une partie est effectivement en cours */
	if(mm->etat == ETAT_MM_ENCOURS)
	{
		i = mm->num_essai_encours -1;
		combinaison_set_couleur(mm->plateau[i], cle, val);
	}
}

/* Cette fonction renvoie 1 si toutes les composantes de l'essai en cours d'un jeu de mastermind sont à valeur dans {COULEUR_MIN ,..., COULEUR_MAX},  
		elle renvoie 0 si tel n'est pas le cas, ou si la partie n'est pas en état ETAT_MM_ENCOURS.
*/
int mastermind_est_essai_encours_valide(mastermind* mm)
{
	int res = 0;
	int i;	/* indice identifiant dans les tableaux (*mm).plateau et (*mm).stat l'essai en cours du joueur : pour la clareté du code */

	/* La requête n'a des sens que si une partie est effectivement en cours */
	if(mm->etat == ETAT_MM_ENCOURS)
	{
		i = mm->num_essai_encours -1;
		res = combinaison_est_valide(mm->plateau[i]);
	}

	/* On retourne le résultat */
	return res;
}

/* Cette fonction acte la combinaison proposée par le joueur pour l'essai en cours dans un jeu de mastermind. Toutefois cette fonction ne fait rien si
		la partie n'est pas en état initial ETAT_MM_ENCOURS, ou si l'essai en cours contient une composante < COULEUR_MIN ou > COULEUR_MAX.
*/
void mastermind_valider_essai_encours(mastermind* mm)
{
	/* Un essai est en cours si et seulement si une partie est effectivement en cours */
	if(mm->etat == ETAT_MM_ENCOURS)
	{
		/* L'essai ne peut être acté que si la combinaison qu'il propose est valide */
		if(mastermind_est_essai_encours_valide(mm))
		{
			mastermind_etape_suivante(mm);
		}
	}
}

/*	__________________________________________________
	Accesseurs en lecture							*/

/*	____ Avancement de la partie
*/

/* Cette fonction renvoie l'etat (ETAT_MM_SECRET, ETAT_MM_ENCOURS, ETAT_MM_GAGNE ou ETAT_MM_PERDU) d'une partie de mastermind
*/
etat_partie mastermind_get_etat(mastermind* mm)
{
	return mm->etat;
}

/* Cette fonction renvoie le numéro d'essai en cours d'une partie de mastermind (0 si une partie n'est pas en cours)
*/
int mastermind_get_num_essai_encours(mastermind* mm)
{
	/* La requête n'a de sens que si une partie est en cours */
	return (mm->etat == ETAT_MM_ENCOURS ? mm->num_essai_encours : 0);
}

/* Cette fonction renvoie le numéro du dernier essai validé dans une partie de mastermind (0 si aucun essai validé jusque lors dans la partie)
*/
int mastermind_get_num_dernier_essai(mastermind* mm)
{
	int res = 0;

	/* La requête n'a de sens que si une partie est en cours ou finie */
	if(mm->etat == ETAT_MM_ENCOURS || mm->etat == ETAT_MM_GAGNE || mm->etat == ETAT_MM_PERDU)
	{
		res = mm->num_essai_encours -1;
	}

	/* On retourne le résultat */
	return res;	
}

/*	____ Combinaisons (combinaison secrète, essai en cours, essais passés)
*/

/* Cette fonction renvoie la couleur de la composante n°cle (à valeur dans {1 ,..., TAILLE_COMBI}) de la combinaison secrète d'une partie de mastermind
*/
couleur mastermind_get_secret(mastermind* mm, int cle)
{
	return combinaison_get_couleur(mm->plateau[NB_ESSAIS], cle);
}

/* Cette fonction renvoie la couleur de la composante n°cle (à valeur dans {1 ,..., TAILLE_COMBI}) de l'essai en cours dans une partie de mastermind
		(COULEUR_INDETERMINEE si aucune partie n'est en cours)
*/
couleur mastermind_get_essai_encours(mastermind* mm, int cle)
{
	couleur res = COULEUR_INDETERMINEE;
	int i;	/* indice identifiant dans les tableaux (*mm).plateau et (*mm).stat l'essai en cours du joueur : pour la clareté du code */

	/* La requête n'a de sens que si une partie est en cours */
	if(mm->etat == ETAT_MM_ENCOURS)
	{
		i = mm->num_essai_encours -1;
		res = combinaison_get_couleur(mm->plateau[i], cle);
	}

	/* On retourne le résultat */
	return res;	
}

/* Cette fonction renvoie la couleur de la composante n°cle (à valeur dans {1 ,..., TAILLE_COMBI}) de l'essai n°num (à valeur dans {1 ,..., NB_ESSAIS}) 
		d'une partie de mastermind (COULEUR_INDETERMINEE si l'essai n°num n'est ni révolu ni en cours, ou la partie n'est ni en cours ni finie)
*/
couleur mastermind_get_essai(mastermind* mm, int num, int cle)
{
	int res = COULEUR_INDETERMINEE;

	/* La requête n'a de sens que si une partie est en cours et l'essai n°num est révolu ou en cours,
		ou la partie est finie et l'essai n°num a été validé par le joueur */
	if( (mm->etat == ETAT_MM_ENCOURS && num <= mm->num_essai_encours) 
			|| (mm->etat == ETAT_MM_GAGNE && num < mm->num_essai_encours)
			|| (mm->etat == ETAT_MM_PERDU && num < mm->num_essai_encours))
	{
		res = combinaison_get_couleur(mm->plateau[num -1], cle);
	}

	/* on retourne le résultat */
	return res;	
}

/*	____ Statististiques des essais passés
*/

/* Cette fonction renvoie le nombre de couleurs correctes de l'essai n°num (à valeur dans {1 ,..., NB_ESSAIS}) d'un jeu de mastermind 
		(-1 si l'essai n°num n'est pas révolu dans la partie en cours ou finie, ou la partie n'est ni en cours ni finie)
*/
int mastermind_get_nb_couleurs_correctes(mastermind* mm, int num)
{
	int res = STAT_VAL_DEF;

	/* la requête n'a de sens que si une partie est en cours ou finie */
	if(mm->etat == ETAT_MM_ENCOURS || mm->etat == ETAT_MM_GAGNE || mm->etat == ETAT_MM_PERDU)
	{
		/* la requête n'a de sens que si l'essai n°num a été validé par le joueur */
		if(num <= mm->num_essai_encours -1)
		{
			res = stat_essai_get_nb_couleurs_correctes(& (mm->stat[num -1]));
		}
	}

	/* on retourne le résultat */
	return res;	
}

/* Cette fonction renvoie le nombre de couleurs correctement placées de l'essai n°num (à valeur dans {1 ,..., NB_ESSAIS}) d'un jeu de mastermind 
		(-1 si l'essai n°num n'est pas révolu dans la partie en cours ou finie, ou la partie n'est ni en cours ni finie)
*/
int mastermind_get_nb_couleurs_placees(mastermind* mm, int num)
{
	int res = STAT_VAL_DEF;

	/* la requête n'a de sens que si une partie est en cours ou finie */
	if(mm->etat == ETAT_MM_ENCOURS || mm->etat == ETAT_MM_GAGNE || mm->etat == ETAT_MM_PERDU)
	{
		/* la requête n'a de sens que si l'essai n°num a été validé par le joueur */
		if(num <= mm->num_essai_encours -1)
		{
			res = stat_essai_get_nb_couleurs_placees(& (mm->stat[num -1]));
		}
	}

	/* on retourne le résultat */
	return res;	
}

/*	______________________________________________________
	Définition des fonctions statiques (sous-routines)	*/

/* Cette fonction permet de passer à l'étape suivante dans le cours d'une une partie de mastermind. Elle est appelée quand :
	- l'on acte la combinaison secrète : 		fonctions 	mastermind_initialiser_avec_secret()		mastermind_valider_secret()
	- l'on acte l'essai en cours du joueur : 	fonction 	mastermind_valider_essai()
*/
static void mastermind_etape_suivante(mastermind* mm)
{
	int i;		/* indice identifiant dans les tableaux (*mm).plateau et (*mm).stat l'essai en cours du joueur : pour la clareté du code */
	int val;

	/* Partie en état initial ETAT_MM_SECRET : le prochain essai du joueur est l'essai n°1, l'état de la partie devient ETAT_MM_ENCOURS	*/
	if(mm->etat == ETAT_MM_SECRET)
	{
		mm->num_essai_encours = 1;
		mm->etat = ETAT_MM_ENCOURS;
	}
	/* Partie en état initial ETAT_MM_ENCOURS : */
	else if(mm->etat == ETAT_MM_ENCOURS)
	{
		/* - mise à jour des statistiques de l'essai en cours */
		i = mm->num_essai_encours -1;

		val = combinaison_cmp_ensembliste(mm->plateau[i], mm->plateau[NB_ESSAIS]);
		stat_essai_set_nb_couleurs_correctes(& (mm->stat[i]), val);

		val = combinaison_cmp_ordonnee(mm->plateau[i], mm->plateau[NB_ESSAIS]);
		stat_essai_set_nb_couleurs_placees(& (mm->stat[i]), val);

		/* - mise à jour éventuelle de l'état de la partie */
		/* l'essai du joueur et la combinaison secrète coïncident si et seulement si val == TAILLE_COMBI */
		if(val == TAILLE_COMBI)
		{
			mm->etat = ETAT_MM_GAGNE;	
		}
		/* le joueur perd si sa NB_ESSAISè tentative est infructueuse */
		else if(mm->num_essai_encours == NB_ESSAIS)
		{
			mm->etat = ETAT_MM_PERDU;
		}

		/* - mise à jour du numéro d'essai en cours */
		(mm->num_essai_encours) ++;
	}

	/* Partie en état final ETAT_MM_ENCOURS : le prochain essai du joueur est initialisé */
	if(mm->etat == ETAT_MM_ENCOURS)
	{
		i = mm->num_essai_encours -1;
		combinaison_initialiser(mm->plateau[i]);
	}
}

