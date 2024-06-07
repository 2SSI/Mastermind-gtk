#include <gtk/gtk.h>
#include <stdlib.h>
#include "combinaison.h"
#include "stat-essai.h"
#include "mastermind.h"

typedef struct {
    GtkWindow* f;
    GtkBox* b_main;
    GtkBox* b_choix;
    GtkBox* b_plateau;
    GtkBox* b_gauche;
    GtkBox* b_droite;
    GtkBox* b_essai[NB_ESSAIS + 1];
    GtkButton* combi[NB_ESSAIS + 1][TAILLE_COMBI];
    GtkBox* b_ind[NB_ESSAIS + 1];
    GtkButton* button_ind[NB_ESSAIS + 1][TAILLE_COMBI];
    GtkButton* valider;
    GtkButton* supprimer;
} vue_t;

vue_t* init_vue();
void lib_vue(vue_t*);

void on_valider_clicked(GtkWidget *widget, gpointer data) {
    vue_t* vue = (vue_t*)data; // Marche pas mdr
    for (int i = 0; i < NB_ESSAIS + 1; i++) {
        for (int j = 0; j < TAILLE_COMBI; j++) {
            gtk_widget_set_sensitive(GTK_WIDGET(vue->combi[i][j]), FALSE);
        }
    }
}

void on_supprimer_clicked(GtkWidget *widget, gpointer data) {
    // Implémenter la logique pour supprimer une combinaison
}

void on_combi_button_clicked(GtkWidget *widget, gpointer data) {
    static int color_index = 0;
    const char* colors[] = {"combi-rouge", "combi-bleu", "combi-jaune", "combi-mauve", "combi-noir", "combi-blanc", "combi-vert", "combi-orange"};
    gtk_widget_set_name(widget, colors[color_index]);
    color_index = (color_index + 1) % 8;
}

int main() {
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

vue_t* init_vue() {
    vue_t* vue = (vue_t*) malloc(sizeof(vue_t));

    if (vue == NULL) {
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

    for (int i = 0; i < NB_ESSAIS + 1; i++) {
        vue->b_essai[i] = (GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
        vue->b_ind[i] = (GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

        for (int j = 0; j < TAILLE_COMBI; j++) {
            vue->combi[i][j] = (GtkButton*) gtk_button_new();
            gtk_widget_set_name(GTK_WIDGET(vue->combi[i][j]), "myCombi");
            g_signal_connect(vue->combi[i][j], "clicked", G_CALLBACK(on_combi_button_clicked), NULL);
            gtk_box_pack_start(vue->b_essai[i], GTK_WIDGET(vue->combi[i][j]), TRUE, TRUE, 0);

            vue->button_ind[i][j] = (GtkButton*) gtk_button_new();
            gtk_widget_set_name(GTK_WIDGET(vue->button_ind[i][j]), "myInd");
            gtk_box_pack_start(vue->b_ind[i], GTK_WIDGET(vue->button_ind[i][j]), TRUE, TRUE, 0);
            gtk_widget_set_sensitive(GTK_WIDGET(vue->button_ind[i][j]), FALSE);
        }
        gtk_box_pack_start(vue->b_gauche, GTK_WIDGET(vue->b_essai[i]), TRUE, TRUE, 0);
        gtk_box_pack_start(vue->b_droite, GTK_WIDGET(vue->b_ind[i]), TRUE, TRUE, 0);
    }

    gtk_widget_set_sensitive(GTK_WIDGET(vue->b_essai[0]), FALSE);

    GtkWidget *titre = gtk_label_new("Mastermind");
    gtk_widget_set_name(titre, "myMastermindTitre");
    gtk_box_pack_start(vue->b_choix, titre, TRUE, FALSE, 0);

    vue->valider = (GtkButton*) gtk_button_new_with_label("Valider");
    gtk_widget_set_name(GTK_WIDGET(vue->valider), "myValider");
    g_signal_connect(vue->valider, "clicked", G_CALLBACK(on_valider_clicked), vue);
    gtk_box_pack_start(vue->b_choix, GTK_WIDGET(vue->valider), TRUE, FALSE, 0);

    vue->supprimer = (GtkButton*) gtk_button_new_with_label("Supprimer");
    gtk_widget_set_name(GTK_WIDGET(vue->supprimer), "mySupprimer");
    g_signal_connect(vue->supprimer, "clicked", G_CALLBACK(on_supprimer_clicked), vue);
    gtk_box_pack_start(vue->b_choix, GTK_WIDGET(vue->supprimer), TRUE, FALSE, 0);

    gtk_box_pack_start(vue->b_plateau, GTK_WIDGET(vue->b_gauche), TRUE, TRUE, 0);
    gtk_box_pack_start(vue->b_plateau, GTK_WIDGET(vue->b_droite), TRUE, TRUE, 0);

    gtk_box_pack_start(vue->b_main, GTK_WIDGET(vue->b_choix), TRUE, TRUE, 0);
    gtk_box_pack_start(vue->b_main, GTK_WIDGET(vue->b_plateau), TRUE, TRUE, 0);

    gtk_container_add(GTK_CONTAINER(vue->f), GTK_WIDGET(vue->b_main));

    return vue;
}

void lib_vue(vue_t* vue) {
    if (vue != NULL)
        free(vue);
}
