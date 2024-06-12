#ifndef CONTROLEUR_H

#define CONTROLEUR_H

#include "vue.h"
#include "mastermind.h"
#include <gtk/gtk.h>

typedef struct{
	mastermind m;
	vue_t* vue;
}ctrl_t;


ctrl_t ctrl_construire();
void lib_ctrl(ctrl_t*);

ctrl_t ctrl_color(ctrl_t);

int valider_choix(vue_t*, int);
void on_valider_clicked(GtkWidget*, gpointer);

void enable(vue_t*, int);
void disable_all(vue_t*);



#endif
