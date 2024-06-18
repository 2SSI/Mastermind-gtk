#ifndef CONTROLEUR_H
#define CONTROLEUR_H
#include "vue.h"
#include "mastermind.h"
#include <gtk/gtk.h>

// Définition de la structure du contrôleur

typedef struct{
	mastermind* m;
	vue_t* vue;
}ctrl_t;

// Fonction d'initialisation du contrôleur
ctrl_t ctrl_construire();

// Fonction pour libérer la mémoire
void lib_ctrl(ctrl_t*);

// Fonction qui désactive toutes les boîtes essais sauf la première
void disable_all(vue_t*);

#endif
