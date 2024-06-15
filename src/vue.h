#ifndef VUE_H
#define VUE_H

#include "combinaison.h"
#include "stat-essai.h"
#include "mastermind.h"
#include <gtk/gtk.h>
#include "common.h"

typedef struct {
    GtkButton* button;
    int color_index;
} buttondata_t;

typedef struct {
    GtkWindow* f;
    GtkBox* b_main;
    GtkBox* b_choix;
    GtkBox* b_plateau;
    GtkBox* b_gauche;
    GtkBox* b_droite;
    GtkBox* b_essai[NB_ESSAIS + 1];
    buttondata_t combi[NB_ESSAIS + 1][TAILLE_COMBI];
    GtkBox* b_ind[NB_ESSAIS + 1];
    GtkButton* button_ind[NB_ESSAIS + 1][TAILLE_COMBI];
    GtkButton* valider;
    mastermind mastermind;
} vue_t;

/*
const char *colors[] = {
    "red", "green", "blue", "magenta", 
    "orange", "yellow", "white", "black"
};*/

vue_t* init_vue();

void lib_vue(vue_t*);
void on_button_clicked(GtkWidget*, gpointer);
void set_color(vue_t*, int);
void on_regles_clicked(GtkWidget*, gpointer);
void valider_essai_actuel(GtkWidget*, gpointer );
void initialliser_combi(vue_t* vue);

//void afficher_fin_partie(GtkWidget*, vue_t*);
//void initialiser_dialog_box(vue_t*);

#endif
