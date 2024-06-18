#ifndef VUE_H
#define VUE_H

#include "combinaison.h"
#include "stat-essai.h"
#include "mastermind.h"
#include <gtk/gtk.h>
#include "common.h"
//#include <gst/gst.h>

#define GAME_STARTED 0
#define GAME_ENDED 1
#define PLAYER_MODE 1
#define COMPUTER_MODE 2


/******** Déclaration des structures pour la vue et les boutons   *///////////

typedef void (*callback)(void* data);

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
    GtkBox* b_ind[NB_ESSAIS + 1];
    GtkButton* button_ind[NB_ESSAIS + 1][TAILLE_COMBI];
    GtkButton* valider;
    buttondata_t combi[NB_ESSAIS + 1][TAILLE_COMBI];

    mastermind* mastermind;
    callback on_game_restart;
    void* controller_state;
    int mode;
    int rules_open;
    int line;
    int game_state;
} vue_t;

/*
const char *colors[] = {
    "red", "green", "blue", "magenta", 
    "orange", "yellow", "white", "black"
};*/



/***********       Déclaration des fonctions       ****************/



// Cette fonction initialise la vue et alloue dynamiquement la mémoire
vue_t* init_vue();

// Cette fonction libère la mémoire dynamiquement alloué précédemment
void lib_vue(vue_t*);

// Cette fonction réinitialise la vue
void reset(vue_t*);

// Cette fonction affiche les règles à l'écran 
void on_regles_clicked(GtkWidget*, gpointer);

// Cette fonction affiche la combianison secrète et met fin au jeu si le bouton "Abondonner" est appuyé
void on_abandonner_clicked(GtkWidget *widget, gpointer data);

// Cette fonction donne le choix à l'utilisateur de jouer ou de faire jouer l'ordianteur
void on_mode_clicked(GtkWidget *widget, gpointer data);

// Cette fonction lance une nouvelle partie lorsque le bouton "Rejouer" est appuyé 
void on_rejouer_clicked(GtkWidget *wigdget, gpointer data);

// Cette fonction assure qu'une seule fenêtre est ouverte pour les "Règles"
void on_rules_destroyed(GtkWidget *widget, gpointer data);

// Cette fonction change la couleur du bouton lorqu'il est appuyé
void on_combi_button_clicked(GtkWidget *, gpointer );

/*   Cette fonction désactive la boîte d'essai courante si tous les boutons de la boîte ont une couleur
     Indique si les couleurs sont bonnes et/ou bien placées 
     Révéle la combinaison secrète si le dernier essai est faux ou si l'on a trouvé la combinaison */
void on_valider_clicked(GtkWidget *widget, gpointer data);

// Fonction pour activer une ligne de notre plateau contenant les boutons "combi"
void enable_line(vue_t*, int);

// Désactivation d'une ligne
void disable_line(vue_t*, int);

// Fonction pour changer la couleur des boutons
void set_button_color(GtkWidget *, const char *);

// Cette fonction associe l'apparence des boutons à une forme passée en paramètre
void add_class(GtkWidget *, const char *);

// Cette fonction assoicie la forme circulaire pour les boutons
GtkButton* rounded_button(GtkButton *);




//void enable(vue_t*, int);
//int valider_choix(vue_t* , int );
//void afficher_fin_partie(GtkWidget*, vue_t*);
//void initialiser_dialog_box(vue_t*);

/*
void play_background_music();
void on_pad_added(GstElement *element, GstPad *pad, gpointer data);
void play_valider(const char *file_path);
void play_win();
*/

//void play_clicked();
#endif
