/* Jeu de Mastermind : fichier d'entete

	________________________ Le jeu :
	Dans le jeu de mastermind, un joueur dispose de 12 tentatives pour deviner une combinaison de 4 couleurs consécutives. 

	Les étapes du jeu :
	- Définir une combinaison secrète de 4 couleurs ;
	- tant que le joueur n'a encore ni deviné la combinaison secrète, ni fait 12 propositions :
		- le joueur propose une combinaison de 4 couleurs ;
		- il est informé du nombre de couleurs de sa proposition (que l'on appelle aussi essai par la suite) qui coïncident avec celles de la combinaison secrète, 
			ainsi que du nombre de telles couleurs qui se trouvent à la même place dans la combinaison secrète.

	________________________ Sont définis / déclarées ici :
	Les constantes symboliques:
		#define NB_ESSAIS 12

	Les types :
		typedef enum e_etat_partie etat_partie;
		typedef struct s_mastermind mastermind ;

	Les fonctions de manipulation :

		Initialisation :
			void mastermind_initialiser(mastermind* mm);	
			void mastermind_initialiser_avec_secret(mastermind* mm);

		Définir une combinaison secrète :
			void mastermind_set_secret(mastermind* mm, int cle, couleur val);
			int mastermind_est_secret_valide(mastermind* mm);
			void mastermind_valider_secret(mastermind* mm);

		Proposer une combinaison (essai en cours) :
			void mastermind_set_essai_encours(mastermind* mm, int cle, couleur val);
			int mastermind_est_essai_encours_valide(mastermind* mm);
			void mastermind_valider_essai_encours(mastermind* mm);

		Accesseurs en lecture :
			etat_partie mastermind_get_etat(mastermind* mm);
			int mastermind_get_num_essai_encours(mastermind* mm);
			int mastermind_get_num_dernier_essai(mastermind* mm);

			couleur mastermind_get_secret(mastermind* mm, int cle);
			couleur mastermind_get_essai_encours(mastermind* mm, int cle);
			couleur mastermind_get_essai(mastermind* mm, int num, int cle);

			int mastermind_get_nb_couleurs_correctes(mastermind* mm, int num);
			int mastermind_get_nb_couleurs_placees(mastermind* mm, int num);

	________________________ TODO :
		Nombre de couleurs correctes et mal placées d'un essai
			int mastermind_get_nb_couleurs_correctes_mal_placees(mastermind* mm, int num);

		Affectation d'une composante donnée de la combinaison secrète / l'essai en cours à la couleur suivante de sa couleur actuelle :
			void mastermind_set_secret_next(mastermind* mm, int cle);
			void mastermind_set_essai_encours_next(mastermind* mm, int cle);
*/

#ifndef MASTERMIND_H

#define MASTERMIND_H

/*	__________________________
	Structure de donnees		*/

/* ____ Combinaison de couleurs
	Les combinaisons de couleurs sont utilisées pour :
	- définir la combinaison secrète
	- tenter de deviner la combinaison secrète (essais du joueur)
*/
#include "combinaison.h"

/* ____ Statistiques d'un essai
	Pour chaque combinaison qu'il propose, le joueur doit être informé :
	- du nombre de couleurs correctes,
	- du nombre de couleurs bien placées
	de la combinaison qu'il propose relativement à la combinaison secrète.
*/
#include "stat-essai.h"

/* ____ Mastermind
	Un jeu de mastermind est caractérisé par :
	- une combinaison secrète
	- un certain nombre (constante symbolique NB_ESSAIS) de combinaisons pouvant être proposées par le joueur
	- un etat de la partie (type énuméré etat_partie)
	- un numéro d'essai en cours (valeur de 1 à NB_ESSAIS si la partie est en cours, de 2 à NB_ESSAIS +1 si la partie est finie, indéterminée sinon)
*/

/* Nombre d'essais */
#define NB_ESSAIS 12

/* État du jeu
	Le jeu peut se trouver dans 4 états distincts :
	- la combinaison secrète est à définir ;
	- une partie est en cours ;
	- une partie est finie et gagnée par le joueur ;
	- une partie est finie et perdue par le joueur.
*/
enum e_etat_partie {
	ETAT_MM_SECRET = 0,	/* la combinaison secrète est à définir */
	ETAT_MM_ENCOURS,		/* une partie est en cours */
	ETAT_MM_GAGNE,	/* le joueur a deviné la combinaison secrète en au plus NB_ESSAIS tentatives */
	ETAT_MM_PERDU	/* après NB_ESSAIS tentatives, le joueur n'est pas parvenu à deviner la combinaison secrète */
};

typedef enum e_etat_partie etat_partie;

/* Mastermind :
	- La combinaison secrète et les NB_ESSAIS du joueur son stockés dans le tableau 'plateau' de NB_ESSAIS +1 variables de type combinaison :
		- les essais n°1 à NB_ESSAIS du joueur sont les entrées d'indice 0 à NB_ESSAIS -1 de ce tableau,
		- la combinaison secrète est l'entrée d'indice NB_ESSAIS de ce tableau.
	- Les statistisques des essais du joueur sont stockées dans le tableau 'stat' de NB_ESSAIS variables de type stat-essai :
		- les statistiques des essais n°1 à NB_ESSAIS du joueur sont les entrées d'indice 0 à NB_ESSAIS -1 de ce tableau.
	- L'état de la partie est stocké dans le champ 'etat' de type etat_partie indiquant l'état de la partie.
	- Le numéro d'essai en cours est stocké dans le champ entier 'num_essai_encours'. Ce champ :
		- n'est pas exploité (mais est initialisé à 0) quand la partie est en état ETAT_MM_SECRET ;
		- est utilisé pour connaître le nombre d'essais qui ont été joués dans une partie en cours ou finie (il peut alors, dans une partie finie, valoir NB_ESSAIS +1).
*/
struct s_mastermind {
	combinaison plateau[NB_ESSAIS +1];	/* essais du joueur et combinaison secrète */
	stat_essai stat[NB_ESSAIS];			/* statistiques associées aux essais du joueur */
	etat_partie etat;					/* état de la partie */
	int num_essai_encours;				/* numéro de l'essai en cours (factice si la partie est en attente de combinaison secrète ou finie) */
};

typedef struct s_mastermind mastermind ;

/*	__________________________________________________
	Initialisation									*/

/* Cette fonction initialise toutes les composantes de la combinaison secrète d'un jeu de mastermind à la valeur COULEUR_INDETERMINEE
	Pré-condition : mm contient bien l'adresse d'une variable de type mastermind
	Post-condition (outre la combinaison secrète) : l'état de la partie est ETAT_MM_SECRET, le numéro d'essai en cours est 0
*/
void mastermind_initialiser(mastermind* mm);

/* Cette fonction lance une nouvelle partie de mastermind en affectant à chaque composante de la combinaison secrète une valeur tirée au hasard 
		dans {COULEUR_MIN ,..., COULEUR_MAX} 
	Pré-conditions :
	- mm contient bien l'adresse d'une variable de type mastermind
	- pour obtenir des générations aléatoires d'une exécution à l'autre, la graine aléatoire doit avoir été préalablement initialisée par appel à srand()
	Post-condition (outre la combinaison secrète) : 
	- l'état de la partie est ETAT_MM_ENCOURS
	- le numéro d'essai en cours vaut 1, les composantes de cet essai valent toutes COULEUR_INDETERMINEE
*/
void mastermind_initialiser_avec_secret(mastermind* mm);

/*	__________________________________________________
	Définir une combinaison secrète					*/

/* Cette fonction affecte une couleur donnée à la composante n°cle (à valeur dans {1 ,..., TAILLE_COMBI}) de la combinaison secrète d'un jeu de mastermind
	Pré-conditions : mm contient bien l'adresse d'une variable de type mastermind, cle à valeur dans {1 ,..., TAILLE_COMBI}
	Post-condition (outre la combinaison secrète) : l'état de la partie est ETAT_MM_SECRET, le numéro d'essai en cours est 0
*/
void mastermind_set_secret(mastermind* mm, int cle, couleur val);

/* Cette fonction renvoie 1 si la combinaison secrète d'un jeu de mastermind contient des couleurs toutes à valeur dans {COULEUR_MIN ,..., COULEUR_MAX} et 0 sinon
	Pré-condition : mm contient bien l'adresse d'une variable de type mastermind
*/
int mastermind_est_secret_valide(mastermind* mm);

/* Cette fonction lance une nouvelle partie de mastermind en actant la combinaison secrète courante. Toutefois cette fonction ne fait rien si 
		la partie n'est pas en état initial ETAT_MM_SECRET, ou la combinaison secrète contient une composante < COULEUR_MIN ou > COULEUR_MAX.
	Pré-condition : mm contient bien l'adresse d'une variable de type mastermind
	Post-condition *si la partie est en état initial ETAT_MM_SECRET et la combinaison secrète est valide* :
		le numéro d'essai en cours vaut 1, les composantes de cet essai valent toutes COULEUR_INDETERMINEE, l'état de la partie est ETAT_MM_ENCOURS
*/
void mastermind_valider_secret(mastermind* mm);

/*	__________________________________________________
	Proposer une combinaison (essai en cours)		*/

/* Cette fonction affecte une couleur donnée à la composante n°cle (à valeur dans {1 ,..., TAILLE_COMBI}) de l'essai en cours d'une partie de mastermind. 
	Toutefois cette fonction ne fait rien si la partie n'est pas en état initial ETAT_MM_ENCOURS.
	Pré-conditions : mm contient bien l'adresse d'une variable de type mastermind, cle à valeur dans {1 ,..., TAILLE_COMBI}
*/
void mastermind_set_essai_encours(mastermind* mm, int cle, couleur val);

/* Cette fonction renvoie 1 si toutes les composantes de l'essai en cours d'un jeu de mastermind sont à valeur dans {COULEUR_MIN ,..., COULEUR_MAX},  
		elle renvoie 0 si tel n'est pas le cas, ou si la partie n'est pas en état ETAT_MM_ENCOURS.
	Pré-condition : mm contient bien l'adresse d'une variable de type mastermind
*/
int mastermind_est_essai_encours_valide(mastermind* mm);

/* Cette fonction acte la combinaison proposée par le joueur pour l'essai en cours dans un jeu de mastermind. Toutefois cette fonction ne fait rien si
		la partie n'est pas en état initial ETAT_MM_ENCOURS, ou si l'essai en cours contient une composante < COULEUR_MIN ou > COULEUR_MAX.
	Pré-condition : mm contient bien l'adresse d'une variable de type mastermind
	Post-conditions *si la partie est en état initial ETAT_MM_ENCOURS* :
	- les statistiques de l'essai que le joueur vient de valider sont mises à jour
	- le numéro d'essai en cours est incrémenté de 1
	- Si l'essai que le joueur vient de valider coïncide avec la combinaison secrète, l'état de la partie devient ETAT_MM_GAGNE 
	  Sinon et s'il s'agissait du dernier essai du joueur, l'état de la partie devient ETAT_MM_PERDU
	  Sinon, les composantes du prochain essai du joueur sont toutes initialisées à COULEUR_INDETERMINEE
*/
void mastermind_valider_essai_encours(mastermind* mm);

/*	__________________________________________________
	Accesseurs en lecture							*/

/*	____ Avancement de la partie
*/

/* Cette fonction renvoie l'etat (ETAT_MM_SECRET, ETAT_MM_ENCOURS, ETAT_MM_GAGNE ou ETAT_MM_PERDU) d'une partie de mastermind
	Pré-condition : mm contient bien l'adresse d'une variable de type mastermind
*/
etat_partie mastermind_get_etat(mastermind* mm);

/* Cette fonction renvoie le numéro de l'essai en cours d'une partie de mastermind (0 si une partie n'est pas en cours)
	Pré-condition : mm contient bien l'adresse d'une variable de type mastermind
*/
int mastermind_get_num_essai_encours(mastermind* mm);

/* Cette fonction renvoie le numéro du dernier essai validé dans une partie de mastermind (0 si aucun essai validé jusque lors dans la partie)
	Pré-condition : mm contient bien l'adresse d'une variable de type mastermind
*/
int mastermind_get_num_dernier_essai(mastermind* mm);

/*	____ Combinaisons (combinaison secrète, essai en cours, essais passés)
*/

/* Cette fonction renvoie la couleur de la composante n°cle (à valeur dans {1 ,..., TAILLE_COMBI}) de la combinaison secrète d'une partie de mastermind
	Pré-conditions : mm contient bien l'adresse d'une variable de type mastermind, cle à valeur dans {1 ,..., TAILLE_COMBI}
*/
couleur mastermind_get_secret(mastermind* mm, int cle);

/* Cette fonction renvoie la couleur de la composante n°cle (à valeur dans {1 ,..., TAILLE_COMBI}) de l'essai en cours dans une partie de mastermind
		(COULEUR_INDETERMINEE si aucune partie n'est en cours)
	Pré-conditions : mm contient bien l'adresse d'une variable de type mastermind, cle à valeur dans {1 ,..., TAILLE_COMBI}
*/
couleur mastermind_get_essai_encours(mastermind* mm, int cle);

/* Cette fonction renvoie la couleur de la composante n°cle (à valeur dans {1 ,..., TAILLE_COMBI}) de l'essai n°num (à valeur dans {1 ,..., NB_ESSAIS}) 
		d'une partie de mastermind (COULEUR_INDETERMINEE si l'essai n°num n'est ni révolu ni en cours, ou la partie n'est ni en cours ni finie)
	Pré-conditions : mm contient bien l'adresse d'une variable de type mastermind, num à valeur dans {1 ,..., NB_ESSAIS}, cle à valeur dans {1 ,..., TAILLE_COMBI}
*/
couleur mastermind_get_essai(mastermind* mm, int num, int cle);

/*	____ Statististiques des essais passés
*/

/* Cette fonction renvoie le nombre de couleurs correctes de l'essai n°num (à valeur dans {1 ,..., NB_ESSAIS}) d'un jeu de mastermind 
		(-1 si l'essai n°num n'est pas révolu dans la partie en cours ou finie, ou la partie n'est ni en cours ni finie)
	Pré-conditions : mm contient bien l'adresse d'une variable de type mastermind, num à valeur dans {1 ,..., NB_ESSAIS}
*/
int mastermind_get_nb_couleurs_correctes(mastermind* mm, int num);

/* Cette fonction renvoie le nombre de couleurs correctement placées de l'essai n°num (à valeur dans {1 ,..., NB_ESSAIS}) d'un jeu de mastermind 
		(-1 si l'essai n°num n'est pas révolu dans la partie en cours ou finie, ou la partie n'est ni en cours ni finie)
	Pré-conditions : mm contient bien l'adresse d'une variable de type mastermind, num à valeur dans {1 ,..., NB_ESSAIS}
*/
int mastermind_get_nb_couleurs_placees(mastermind* mm, int num);

#endif

