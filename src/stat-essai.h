/* Essai d'une combinaison pour le jeu de Mastermind : fichier d'entete

	Dans le jeu de mastermind, une combinaison est une séquence de 4 couleurs dans laquelle une même couleur peut apparaître plusieurs fois. 
	Un essai du joueur est une combinaison qui doit être comparée à la combinason secrète. 
	Pour chacune des tentatives du joueur, il faut lui indiquer :
	- le nombre de composantes de sa séquence qui apparaissent à la même place dans la séquence à deviner ;
	- le nombre de composantes de sa séquence qui apparaissent aussi dans la séquence à deviner, possiblement à des places différentes.

	________________________ Sont définis / déclarées ici :
	Les constantes symboliques :
		#define STAT_VAL_DEF -1

	Les types :
		typedef struct s_stat_essai stat_essai;

	Les fonctions de manipulation :
		Accesseurs en lecture/écriture pour le nombre de couleurs correctement placées :
			- void stat_essai_set_nb_couleurs_placees(stat_essai* stat, int val);
			- int stat_essai_get_nb_couleurs_placees(stat_essai* stat);
		Accesseurs en lecture/écriture pour le nombre de couleurs correctes :
			- void stat_essai_set_nb_couleurs_correctes(stat_essai* stat, int val);
			- int stat_essai_get_nb_couleurs_correctes(stat_essai* stat);
*/

#ifndef STAT_ESSAI_H

#define STAT_ESSAI_H

/*	______________________________________
	Structure de donnees					*/

/* ____ Les statistiques d'un essai consistent en deux chiffres :
	- le nombre de couleurs correctement placées
	- le nombre de couleurs correctes
*/

struct s_stat_essai {
	int ordo;		/* nombre de couleurs bien placées d'un essai */	
	int non_ordo;	/* nombre de couleurs correctes d'un essai */
};

typedef struct s_stat_essai stat_essai;

/* ____ Valeur par défaut : -1 (pour distinguer une statistique non renseignée d'une statistique renseignée, à valeur alors >= 0)
*/

#define STAT_VAL_DEF -1

/*	______________________________________
	Accesseurs en lecture / écriture		*/

/* Cette fonction attribue une valeur à la propriété ordo d'une variable de type stat_essai
	Pré-condition : stat contient bien l'adresse d'une variable de type stat_essai
	Post-condition : la propriété ordo de la variable *stat est mise à jour à val
*/
void stat_essai_set_nb_couleurs_placees(stat_essai* stat, int val);

/* Cette fonction renvoie la valeur de la propriété ordo d'une variable de type stat_essai
	Pré-condition : stat contient bien l'adresse d'une variable de type stat_essai
	Valeur retournée : la valeur de la propriété ordo de la variable *stat
*/
int stat_essai_get_nb_couleurs_placees(stat_essai* stat);

/* Cette fonction attribue une valeur à la propriété non_ordo d'une variable de type stat_essai
	Pré-condition : stat contient bien l'adresse d'une variable de type stat_essai
	Post-condition : la propriété non_ordo de la variable *stat est mise à jour à val
*/
void stat_essai_set_nb_couleurs_correctes(stat_essai* stat, int val);

/* Cette fonction renvoie la valeur de la propriété non_ordo d'une variable de type stat_essai
	Pré-condition : stat contient bien l'adresse d'une variable de type stat_essai
	Valeur retournée : la valeur de la propriété non_ordo de la variable *stat
*/
int stat_essai_get_nb_couleurs_correctes(stat_essai* stat);

#endif

