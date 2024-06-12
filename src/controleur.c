#include "controleur.h"
#include "mastermind.h"
#include "stat-essai.h"
#include "combinaison.h"
#include "vue.h"


ctrl_t ctrl_construire(){
	mastermind m;
	vue_t* vue;
	ctrl_t c;
	
	mastermind_initialiser(&m);
	vue = init_vue();
	
	c.m = m;
	c.vue = vue;
	
	return c;
}


void lib_ctrl(ctrl_t* c){
	lib_vue(c->vue);
	if(c != NULL)
		free(c);
}


ctrl_t ctrl_color(ctrl_t c){
	if(mastermind_get_etat(&c.m) == ETAT_MM_SECRET){
		int num = mastermind_get_num_essai_encours(&c.m);
		
		if(valider_choix(c.vue, num) == 0){
			set_color(c.vue, num);
		}
		enable(c.vue, num);	
	}
	return c;
}



void disable_all(vue_t* vue) {
    for (int i = 0; i < NB_ESSAIS; i++) {
        for (int j = 0; j < TAILLE_COMBI; j++) {
            gtk_widget_set_sensitive(GTK_WIDGET(vue->combi[i][j].button), FALSE);
            gtk_widget_set_sensitive(GTK_WIDGET(vue->button_ind[i][j]), FALSE);
        }
    }
    for (int j = 0; j < TAILLE_COMBI; j++) {
        gtk_widget_set_sensitive(GTK_WIDGET(vue->button_ind[NB_ESSAIS][j]), FALSE);
    }
}

void enable(vue_t* vue, int nb_boite) {
    if (valider_choix(vue, nb_boite + 1) == 1) {
        for (int j = 0; j < TAILLE_COMBI; j++) {
            gtk_widget_set_sensitive(GTK_WIDGET(vue->combi[nb_boite][j].button), TRUE);
        }
    }
}

int valider_choix(vue_t* vue, int nb_boite) {
    int tot = 0;
    for (int i = 0; i < TAILLE_COMBI; i++) {
        if (vue->combi[nb_boite][i].color_index != -1) {
            tot++;
        }
    }
    if (tot == TAILLE_COMBI) {
        g_signal_connect(G_OBJECT(vue->valider), "clicked", G_CALLBACK(on_valider_clicked), vue->b_essai[nb_boite]);
        return 1;
    }
    return 0;
}

void on_valider_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *essai_box = GTK_WIDGET(data);
    gtk_widget_set_sensitive(essai_box, FALSE);
}
