/*	Jeu de mastermind : programme de démonstration
*/

#include "mastermind.h"

#include <string.h>	/* fonctions strlen() (pour gérer les arguments de la fonction main()) */
#include <stdlib.h>	/* fonction srand(), constante EXIT_SUCCESS (retour de la fonction main()) */
#include <time.h>	/* fonction time() (pour appel fonction srand()) */
#include <stdio.h>	/* fonctions d'entrée /sortie */

/* _______________________________
	Constantes symboliques		*/

#define STR_AVERTISSEMENT "requete non pertinente dans le contexte actuel de la partie"	/* message d'avertissement requête invalide  */

/* _______________________________
	Declaration des fonctions	*/

/* ____ Scenarii de test
*/

/* Scenario 1 : 
	- l'utilisateur saisit une combinaison secrète
	- l'ordinateur tente de la deviner par tirages aléatoires
*/
void mastermind_ordinateur_joue();

/* Scenario 2 : 
	- l'ordinateur tire une combinaison secrète au hasard
	- l'utilisateur tente de la devier
*/
void mastermind_utilisateur_joue();

/* ____ Sous-routines
*/

/* Cette fonction affiche une partie de mastermind 
*/
void mastermind_afficher(mastermind* mm);

/* Cette fonction affiche la combinaison secrète d'un jeu de mastermind (un message d'avertissement si la partie est en cours)
*/
void mastermind_afficher_secret(mastermind* mm);

/* Cette fonction affiche l'essai en cours dans une partie de mastermind (un message d'avertissement si la partie n'est pas en cours)
*/
void mastermind_afficher_essai_encours(mastermind* mm);

/* Cette fonction affiche une tentative faite par le joueur dans une partie de mastermind
	(un message d'avertissement si la partie n'est ni en cours, ni finie, ou l'essai dont le numéro est passé en paramètre n'a pas encore été validé par le joueur)
*/
void mastermind_afficher_essai_passe(mastermind* mm, int num);

/* Cette fonction permet de saisir la combinaison secrète d'un jeu de mastermind 
*/
void mastermind_saisir_secret(mastermind* mm);

/* Cette fonction attribue une combinaison aléatoire à l'essai en cours d'une partie de mastermind en cours (elle ne fait rien si une partie n'est pas en cours)
*/
void mastermind_tirer_au_hasard_essai_encours(mastermind* mm);

/* Cette fonction permet de saisir l'essai en cours d'une partie de mastermind en cours (elle ne fait rien si une partie n'est pas en cours)
*/
void mastermind_saisir_essai_encours(mastermind* mm);

/* Cette fonction vide le buffer
*/
void purger_buffer();

/* _______________________________
	Programme de test			*/

int main(int argc, char* argv[])
{
	char choix;
	int ok = 1;

	/* ____ Lecture des arguments du main */
	if (argc != 2)
	{
		ok = 0;
	}
	else if (strlen(argv[1]) != 1)
	{
		ok = 0;
	}
	else if ((argv[1][0] < '1') || (argv[1][0] > '2'))
	{
		ok = 0;
	}

	if(! ok)
	{
		printf("Utilisation :\t%s numero\tou numero vaut 1 ou 2\n", argv[0]);
	}
	else
	{
		/* ____ Déroulement du scenario demandé */
		choix = argv[1][0];

		/* Scenario demandé */
		if(choix == '1')
		{
			mastermind_ordinateur_joue();
		}
		else
		{
			mastermind_utilisateur_joue();
		}
	}

	/* ____ Programme terminé */
	return EXIT_SUCCESS;
}

/* _______________________________
	Scenarii						*/

/* ____ Scenarii de test
*/

/* Scenario 1 : l'ordinateur joue au hasard
*/
void mastermind_ordinateur_joue()
{
	mastermind mm;

	printf("________________ %s : l'ordinateur trouvera-t-il la combinaison secrete en au plus %d essai ?\n", __func__, NB_ESSAIS);

	/* __ Initialisations */
	srand(time(NULL));
	mastermind_initialiser(& mm);

	/* __ Combinaison secrète */
	printf("\n____ %s : definition de la combinaison secrete\n", __func__);

	/* saisie par l'utilisateur */
	mastermind_saisir_secret(& mm);

	/* validation */
	mastermind_valider_secret(& mm);

	/* __ Déroulement d'une partie */
	printf("\n____ %s : lancement de la partie (l'ordinateur jour au hasard) !\n\n", __func__);
	do
	{
		/* __ Affichage partie en cours */
		mastermind_afficher(& mm);
		getchar();

		/* __ Attribution essai en cours */

		/* tirage aléatoire d'une combinaison */
		mastermind_tirer_au_hasard_essai_encours(& mm);

		/* validation de l'essai en cours */
		mastermind_valider_essai_encours(& mm);
	} while(mastermind_get_etat(& mm) == ETAT_MM_ENCOURS);

	/* __ Affichage partie finie */
	mastermind_afficher(& mm);
}

/* Scenario 2 : l'utilisateur joue
*/
void mastermind_utilisateur_joue()
{
	mastermind mm;

	printf("________________ %s : l'ordinateur peut-il tricher en remportant la partie du premier coup ?\n", __func__);

	/* __ Initialisations */
	srand(time(NULL));

	/* __ Combinaison secrète */
	printf("\n____ %s : definition de la combinaison secrete (l'ordinateur la tire au hasard).\n", __func__);
	mastermind_initialiser_avec_secret(& mm);

	/* __ Déroulement d'une partie */
	printf("\n____ %s : lancement de la partie !\n\n", __func__);
	do
	{
		/* __ Affichage partie en cours */
		mastermind_afficher(& mm);

		/* __ Attribution essai en cours */

		/* saisie par l'utilisateur */
		mastermind_saisir_essai_encours(& mm);

		/* validation */
		mastermind_valider_essai_encours(& mm);
	} while(mastermind_get_etat(& mm) == ETAT_MM_ENCOURS);

	/* __ Affichage partie finie */
	mastermind_afficher(& mm);
}

/* ____ Sous-routines
*/

/* Cette fonction affiche une partie de mastermind en par utilisation des accesseurs et acces directs 
	- partie en état ETAT_MM_SECRET 	 : on n'affiche que la combinaison secrète
	- partie en état ETAT_MM_ENCOURS : on n'affiche que la combinaison secrète
*/
void mastermind_afficher(mastermind* mm)
{
	int num;

	printf("Partie de mastermind : ");

	/* ____ Combinaison secrète : elle n'est affichée que si une partie n'est pas en cours */
	if(mastermind_get_etat(mm) != ETAT_MM_ENCOURS)
	{
		/* entête affichage dédiée selon état de la partie */
		if(mastermind_get_etat(mm) == ETAT_MM_SECRET)
		{
			printf("combinaison secrete en cours de composition\t");
		}
		else if(mastermind_get_etat(mm) == ETAT_MM_GAGNE)
		{
			printf("%d essais (parmi %d permis) auront suffi pour determiner la combinaison secrete ", mastermind_get_num_dernier_essai(mm), NB_ESSAIS);
		}
		else		/* partie en etat ETAT_MM_PERDU */
		{
			printf("les %d essais n'ont pas permis de determiner la combinaison secrete ", mastermind_get_num_dernier_essai(mm));
		}

		/* affichage combinaison secrète */
		mastermind_afficher_secret(mm);
		printf("\n");
	}
	/* ____ Essai en cours : il n'est défini que si une partie est en cours */
	else
	{
		/* entête affichage */
		printf("essai n°%d (parmi %d permis) en cours de composition\t", mastermind_get_num_essai_encours(mm), NB_ESSAIS);

		/* affichage combinaison de l'essai en cours */
		printf("\n\tessai n°%d :\t", mastermind_get_num_essai_encours(mm));
		mastermind_afficher_essai_encours(mm);
		printf("\n");
	}

	/* ____ Essais validés par le joueur : cas d'une partie en cours en finie */
	if(mastermind_get_etat(mm) != ETAT_MM_SECRET)
	{
		for(num = mastermind_get_num_dernier_essai(mm) ; num >= 1 ; num --)
		{
			printf("\tessai n°%d :\t", num);
			mastermind_afficher_essai_passe(mm, num);
			printf("\n");
		}
	}
}

/* Cette fonction affiche la combinaison secrète d'un jeu de mastermind (un message d'avertissement si la partie est en cours)
*/
void mastermind_afficher_secret(mastermind* mm)
{
	int cle;

	if(mastermind_get_etat(mm) == ETAT_MM_ENCOURS)
	{
		printf("%s: %s", __func__, STR_AVERTISSEMENT);
	}
	else
	{
		for(cle = 1 ; cle <= TAILLE_COMBI ; cle ++)
		{
			printf("%d ", mastermind_get_secret(mm, cle));
		}
	}
}

/* Cette fonction affiche l'essai en cours dans une partie de mastermind (un message d'avertissement si la partie n'est pas en cours)
*/
void mastermind_afficher_essai_encours(mastermind* mm)
{
	int cle;

	if(mastermind_get_etat(mm) != ETAT_MM_ENCOURS)
	{
		printf("%s: %s", __func__, STR_AVERTISSEMENT);
	}
	else
	{
		for(cle = 1 ; cle <= TAILLE_COMBI ; cle ++)
		{
			printf("%d ", mastermind_get_essai_encours(mm, cle));
		}

		printf("\t(essai %s en cours de composition)", (mastermind_est_essai_encours_valide(mm) ? "valide" : "invalide"));
	}
}

/* Cette fonction affiche une tentative faite par le joueur dans une partie de mastermind 
	(un message d'avertissement si la partie n'est ni en cours, ni finie, ou l'essai dont le numéro est passé en paramètre n'a pas encore été validé par le joueur)
*/
void mastermind_afficher_essai_passe(mastermind* mm, int num)
{
	int cle;

	if( mastermind_get_etat(mm) == ETAT_MM_SECRET || (mastermind_get_etat(mm) != ETAT_MM_SECRET && num > mastermind_get_num_dernier_essai(mm)) )
	{
		printf("%s: %s", __func__, STR_AVERTISSEMENT);
	}
	else
	{
		/* la combinaison */
		for(cle = 1 ; cle <= TAILLE_COMBI ; cle ++)
		{
			printf("%d ", mastermind_get_essai(mm, num, cle));
		}

		/* ses statistiques */
		printf("\t%d couleurs corectes, dont %d placees", mastermind_get_nb_couleurs_correctes(mm, num), mastermind_get_nb_couleurs_placees(mm, num));
	}
}

/* Cette fonction permet de saisir la combinaison secrète d'un jeu de mastermind 
*/
void mastermind_saisir_secret(mastermind* mm)
{
	char choix;
	int cle;
	couleur coul;

	/* __ Saisie de la combinaison secrète : tant que l'utilisateur le souhaite, ou la combinaison secrète est invalide */
	do
	{
		/* Saisie d'une nouvelle combinaison */
		printf("\nSaisir une nouvelle combinaison secrète (succession de %d valeurs entieres dans l'intervalle [%d, %d]) : ", TAILLE_COMBI, COULEUR_MIN, COULEUR_MAX);
		for(cle = 1 ; cle <= TAILLE_COMBI ; cle ++)
		{
			scanf("%d", &coul);
			mastermind_set_secret(mm, cle, coul);
		}

		/* Restitution et, si la combinaison saisie est valide, choix donné à l'utilisateur de la valider ou non */
		printf("Vous avez saisi la combinaison\t");
		mastermind_afficher_secret(mm);

		if(mastermind_est_secret_valide(mm))
		{
			printf(" : combinaison valide.\n");
			printf("Taper 'm' ou 'M' pour modifier la combinaison, toute autre touche pour la valider : ");
			purger_buffer();
			scanf("%c", &choix);
		}
		else
		{
			printf(" : combinaison invalide.\n");
			choix = 'm';
		}

		/* Purge du buffer en vue de saisies ultérieures */
		purger_buffer();
	}
	while(choix == 'm' || choix == 'M');
}

/* Cette fonction attribue une combinaison aléatoire à l'essai en cours d'une partie de mastermind en cours (elle ne fait rien si une partie n'est pas en cours)
*/
void mastermind_tirer_au_hasard_essai_encours(mastermind* mm)
{
	int cle;
	couleur coul;

	if(mastermind_get_etat(mm) == ETAT_MM_ENCOURS)
	{
		/* Tirage aléatoire une à une des composantes de la combinaison */
		for(cle = 1 ; cle <= TAILLE_COMBI ; cle ++)
		{
			coul = (rand() % NB_COULEURS) +COULEUR_MIN;
			mastermind_set_essai_encours(mm, cle, coul);
		}
	}
}

/* Cette fonction permet de saisir l'essai en cours d'une partie de mastermind en cours (elle ne fait rien si une partie n'est pas en cours)
*/
void mastermind_saisir_essai_encours(mastermind* mm)
{
	char choix;
	int cle;
	couleur coul;

	if(mastermind_get_etat(mm) == ETAT_MM_ENCOURS)
	{
		/* __ Saisie de l'essai en cours : tant que l'utilisateur le souhaite, ou l'essai en cours est invalide */
		do
		{
			/* Saisie d'une nouvelle combinaison */
			printf("\nSaisir une nouvelle combinaison pour l'essai en cours (succession de %d valeurs entieres dans l'intervalle [%d, %d]) : ", TAILLE_COMBI, COULEUR_MIN, COULEUR_MAX);
			for(cle = 1 ; cle <= TAILLE_COMBI ; cle ++)
			{
				scanf("%d", &coul);
				mastermind_set_essai_encours(mm, cle, coul);
			}

			/* Restitution et, si la combinaison saisie est valide, choix donné à l'utilisateur de la valider ou non */
			printf("Vous avez saisi la combinaison\t");
			mastermind_afficher_essai_encours(mm);

			if(mastermind_est_essai_encours_valide(mm))
			{
				printf(" : combinaison valide.\n");
				printf("Taper 'm' ou 'M' pour modifier la combinaison, toute autre touche pour la valider : ");
				purger_buffer();
				scanf("%c", &choix);
			}
			else
			{
				printf(" : combinaison invalide.\n");
				choix = 'm';
			}

			/* Purge du buffer en vue de saisies ultérieures */
			purger_buffer();
		}
		while(choix == 'm' || choix == 'M');
	}
}

/* Cette fonction vide le buffer
*/
void purger_buffer()
{
	char c;

	while ((c = getchar()) != '\n' && c != EOF)
	{}
}

