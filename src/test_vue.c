#include <gtk/gtk.h>
#include<stdlib.h>

#include "combinaison.h"
#include "stat-essai.h"
#include "mastermind.h"


typedef struct{
	GtkWindow* f;
	GtkBox* b_main;
	GtkBox* b_choix;
	GtkBox* b_plateau;
	GtkBox* b_gauche;
	GtkBox* b_droite;
	GtkBox* b_essai[NB_ESSAIS + 1];
	GtkButton* combi[TAILLE_COMBI];
	GtkBox* b_ind[NB_ESSAIS + 1];
	GtkButton* button_ind[TAILLE_COMBI];
	GtkButton* valider;
}vue_t;


vue_t* init_vue();
void lib_vue(vue_t*);

//void design_interface(vue_t*);



int main(){
	gtk_init(NULL, NULL);
	
	GtkCssProvider *cssProvider = gtk_css_provider_new();
   	gtk_css_provider_load_from_path(cssProvider, "design.css", NULL);
    	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                          GTK_STYLE_PROVIDER(cssProvider),
                                          GTK_STYLE_PROVIDER_PRIORITY_USER);
	 
	vue_t* vue;
	vue = init_vue();
	
	
	gtk_widget_show_all(GTK_WIDGET(vue->f));
	g_signal_connect(G_OBJECT(vue->f), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	gtk_main();
	lib_vue(vue);
	
	return EXIT_SUCCESS;
}



vue_t* init_vue(){
	vue_t* vue = (vue_t*) malloc(sizeof(vue_t));
	
	if(vue == NULL){
		perror("échec malloc");
		exit(-1);
	}
	
	vue->f = (GtkWindow*) gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(vue->f, "Mastermind");
	
	vue->b_main = (GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	vue->b_choix = (GtkBox*) gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	vue->b_plateau = (GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	
	vue->b_gauche = (GtkBox*) gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	vue->b_droite = (GtkBox*) gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	
	for(int i=0; i<NB_ESSAIS + 1; i=i+1){
		vue->b_essai[i] = (GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
		vue->b_ind[i] = (GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
		
		for(int j=0; j<TAILLE_COMBI; j=j+1){
			vue->combi[j] = (GtkButton*) gtk_button_new();
			gtk_box_pack_start(vue->b_essai[i], GTK_WIDGET(vue->combi[j]), TRUE, TRUE, 0);
			
			vue->button_ind[j] = (GtkButton*) gtk_button_new();
			gtk_box_pack_start(vue->b_ind[i], GTK_WIDGET(vue->button_ind[j]), TRUE, TRUE, 0);
			gtk_widget_set_sensitive(GTK_WIDGET(vue->button_ind[j]), FALSE);
		}
		gtk_box_pack_start(vue->b_gauche, GTK_WIDGET(vue->b_essai[i]), TRUE, TRUE, 0);
		gtk_box_pack_start(vue->b_droite, GTK_WIDGET(vue->b_ind[i]), TRUE, TRUE, 0);	
	}
	
	gtk_widget_set_sensitive(GTK_WIDGET(vue->b_essai[0]), FALSE);
	
	vue->valider = (GtkButton*) gtk_button_new_with_label("Valider");
	gtk_box_pack_start(vue->b_choix, GTK_WIDGET(vue->valider), TRUE, FALSE, 15);
	
	gtk_box_pack_start(vue->b_plateau, GTK_WIDGET(vue->b_gauche), TRUE, TRUE, 0);
	gtk_box_pack_start(vue->b_plateau, GTK_WIDGET(vue->b_droite), TRUE, TRUE, 0);
	
	gtk_box_pack_start(vue->b_main, GTK_WIDGET(vue->b_choix), TRUE, TRUE, 0);
	gtk_box_pack_start(vue->b_main, GTK_WIDGET(vue->b_plateau), TRUE, TRUE, 0);
	
	gtk_container_add(GTK_CONTAINER(vue->f), GTK_WIDGET(vue->b_main));
	//design_interface(vue);

	return vue;
}

void lib_vue(vue_t* vue){
	if(vue != NULL)
		free(vue);
}


/*
void design_interface(vue_t* vue){
	gtk_widget_set_name(GTK_WIDGET(vue->valider), "myValider");
	
	char *tab_color[8] = {"rouge", "vert", "bleu", "mauve", "orange", "jaune", "blanc", "noir"};
	
	for(int i=0; i<)
}
*/
