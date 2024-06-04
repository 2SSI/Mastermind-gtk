#ifndef VUE_H

#define VUE_H

#include "combinaison.h"
#include "stat-essai.h"
#include "mastermind.h"
#include <gtk/gtk.h>


typedef struct{
	GtkWindow* f;
	GtkBox* b_main;
	GtkBox* b_choix;
	GtkBox* b_plateau;
	GtkBox* b_gauche;
	GtkBox* b_droite;
	GtkBox* b_essai[NB_ESSAIS + 1];
	GtkButton* combi[TAILLE_COMBI];
	
	GtkButton* button_essai[NB_ESSAIS + 1][TAILLE_COMBI];
	
	GtkBox* b_ind[NB_ESSAIS + 1];
	GtkButton* button_ind[TAILLE_COMBI];
	GtkButton* valider;
}vue_t;


void init_vue(vue_t*);
void lib_vue(vue_t*);

//void design_interface(vue_t*);





#endif
