#include <gtk/gtk.h>
#include <stdlib.h>
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
    GtkButton* button_regles;
    GtkButton* button_abandonner;
} vue_t;

vue_t* init_vue();
void lib_vue(vue_t*);

void on_valider_clicked(GtkWidget *widget, gpointer data) {
}

void show_rules(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(data),
                                               GTK_DIALOG_DESTROY_WITH_PARENT,
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_OK,
                                               "Contenu des Règles");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void show_abandon(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(data),
                                               GTK_DIALOG_DESTROY_WITH_PARENT,
                                               GTK_MESSAGE_QUESTION,
                                               GTK_BUTTONS_YES_NO,
                                               "Voulez-vous vraiment abandonner?");
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_YES) {
        gtk_main_quit();
    }
    gtk_widget_destroy(dialog);
}

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
    gtk_window_set_default_size(GTK_WINDOW(vue->f), 777, 900);  
    
    vue->b_main = (GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    vue->b_choix = (GtkBox*) gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    vue->b_plateau = (GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    
    vue->b_gauche = (GtkBox*) gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    vue->b_droite = (GtkBox*) gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    
    for(int i = 0; i < NB_ESSAIS + 1; i++){
        vue->b_essai[i] = (GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
        vue->b_ind[i] = (GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
        
        for(int j = 0; j < TAILLE_COMBI; j++){
            vue->combi[j] = (GtkButton*) gtk_button_new();
            gtk_widget_set_name(GTK_WIDGET(vue->combi[j]), "combi-button");
            gtk_box_pack_start(vue->b_essai[i], GTK_WIDGET(vue->combi[j]), TRUE, TRUE, 0);
            
            vue->button_ind[j] = (GtkButton*) gtk_button_new();
            gtk_widget_set_name(GTK_WIDGET(vue->button_ind[j]), "button-right");
            gtk_box_pack_start(vue->b_ind[i], GTK_WIDGET(vue->button_ind[j]), TRUE, TRUE, 0);
            gtk_widget_set_sensitive(GTK_WIDGET(vue->button_ind[j]), FALSE);
        }
        gtk_box_pack_start(vue->b_gauche, GTK_WIDGET(vue->b_essai[i]), TRUE, TRUE, 0);
        gtk_box_pack_start(vue->b_droite, GTK_WIDGET(vue->b_ind[i]), TRUE, TRUE, 0);
    }
    
    gtk_widget_set_sensitive(GTK_WIDGET(vue->b_essai[0]), FALSE);
    
    vue->valider = (GtkButton*) gtk_button_new_with_label("Valider");
    gtk_widget_set_name(GTK_WIDGET(vue->valider), "myValider");
    g_signal_connect(vue->valider, "clicked", G_CALLBACK(on_valider_clicked), NULL);
    gtk_box_pack_start(vue->b_choix, GTK_WIDGET(vue->valider), TRUE, FALSE, 15);
    
	/*
    vue->button_regles = (GtkButton*) gtk_button_new_with_label("Règles du jeu");
    g_signal_connect(vue->button_regles, "clicked", G_CALLBACK(show_rules), vue->f);
    gtk_box_pack_start(vue->b_choix, GTK_WIDGET(vue->button_regles), TRUE, FALSE, 5);
    
    vue->button_abandonner = (GtkButton*) gtk_button_new_with_label("Abandonner");
    g_signal_connect(vue->button_abandonner, "clicked", G_CALLBACK(show_abandon), vue->f);
    gtk_box_pack_start(vue->b_choix, GTK_WIDGET(vue->button_abandonner), TRUE, FALSE, 5);
    */
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
