#include "vue.h"
#include <gtk/gtk.h>
#include<stdlib.h>

vue_t* init_vue(){
	vue_t* vue = (vue_t*) malloc(sizeof(vue_t));
	
	if(vue == NULL){
		perror("échec malloc");
		exit(-1);
	}
	
	vue->f = (GtkWindow) gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(vue->f, "Mastermind");
	
	vue->b_main = (GtkBox*) gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	vue->b_choix = (GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	vue->b_plateau = (GtkBox*) gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	
	vue->b_gauche = (GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	vue->b_droite = (GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	
	for(int i=0; i<NB_ESSAIS + 1; i=i+1){
		vue->b_essai[i] = (GtkBox*) gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
		vue->b_ind[i] = (GtkBox*) gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
		
		for(int j=0; j<TAILLE_COMBI; j=j+1){
			vue->combi[j] = (GtkButton*) gtk_button_new();
			gtk_box_pack_start(vue->b_essai[i], GTK_WIDGET(vue->combi[j]), TRUE, TRUE, 0);
			
			vue->button_ind[j] = (GtkButton*) gtk_button_new();
			gtk_box_pack_start(vue->b_ind[i], GTK_WIDGET(vue->button_ind[j]), TRUE, TRUE, 0);
		}
		gtk_box_pack_start(vue->b_gauche, GTK_WIDGET(vue->b_essai[i]), TRUE, TRUE, 0);
		gtk_box_pack_start(vue->b_droite, GTK_WIDGET(vue->b_ind[i]), TRUE, TRUE, 0);	
	}
	
	vue->valider = (GtkButton*) gtk_button_new_with_label("Valider");
	gtk_box_pack_start(vue->b_choix, GTK_WIDGET(vue->valider), TRUE, TRUE, 0);
	
	gtk_box_pack_start(vue->b_plateau, GTK_WIDGET(vue->b_gauche), TRUE, TRUE, 0);
	gtk_box_pack_start(vue->b_plateau, GTK_WIDGET(vue->b_droite), TRUE, TRUE, 0);
	
	gtk_box_pack_start(vue->b_main, GTK_WIDGET(vue->b_choix), TRUE, TRUE, 0);
	gtk_box_pack_start(vue->b_main, GTK_WIDGET(vue->b_plateau), TRUE, TRUE, 0);
	
	gtk_container_add(GTK_CONTAINER(vue->f), GTK_WIDGET(vue->b_main));

	return vue;
}

void lib_vue(vue_t* vue){
	if(vue != NULL)
		free(vue);
}
