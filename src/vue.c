#include "vue.h"
#include <gtk/gtk.h>

#include "combinaison.h"
#include "stat-essai.h"
#include "mastermind.h"


#include "common.h"
const char *colors[] = {
    "red", "green", "blue", "magenta", 
    "orange", "yellow", "white", "black"
};

vue_t* init_vue() {
    vue_t* vue = (vue_t*) malloc(sizeof(vue_t));

    if (vue == NULL) {
        perror("échec malloc");
        exit(-1);
    }

    vue->f = (GtkWindow*) gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(vue->f, "Mastermind");
    gtk_window_set_default_size(vue->f, 800, 900);

    vue->b_main = (GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    vue->b_choix = (GtkBox*) gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    vue->b_plateau = (GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

    vue->b_gauche = (GtkBox*) gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    vue->b_droite = (GtkBox*) gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    for (int i = 0; i < NB_ESSAIS + 1; i++) {
        vue->b_essai[i] = (GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
        vue->b_ind[i] = (GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

        for (int j = 0; j < TAILLE_COMBI; j++) {
            vue->combi[i][j].button = (GtkButton*) gtk_button_new();
            vue->combi[i][j].color_index = COULEUR_INDETERMINEE;
            //gtk_widget_set_name(GTK_WIDGET(vue->combi[i][j].button), "myCombi");
            gtk_box_pack_start(vue->b_essai[i], GTK_WIDGET(vue->combi[i][j].button), TRUE, TRUE, 0);

            vue->button_ind[i][j] = (GtkButton*) gtk_button_new();
            //gtk_widget_set_name(GTK_WIDGET(vue->button_ind[i][j]), "myInd");
            gtk_box_pack_start(vue->b_ind[i], GTK_WIDGET(vue->button_ind[i][j]), TRUE, TRUE, 0);
        }
        gtk_box_pack_start(vue->b_gauche, GTK_WIDGET(vue->b_essai[i]), TRUE, TRUE, 0);
        gtk_box_pack_start(vue->b_droite, GTK_WIDGET(vue->b_ind[i]), TRUE, TRUE, 0);
    }

    GtkWidget *titre = gtk_label_new("Mastermind");
    //gtk_widget_set_name(titre, "myMastermindTitre");
    gtk_box_pack_start(vue->b_choix, titre, TRUE, FALSE, 0);

    vue->valider = (GtkButton*) gtk_button_new_with_label("Valider");
    //gtk_widget_set_name(GTK_WIDGET(vue->valider), "myValider");
    gtk_box_pack_start(vue->b_choix, GTK_WIDGET(vue->valider), TRUE, FALSE, 0);

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

void on_button_clicked(GtkWidget *widget, gpointer data) {
    buttondata_t* button_data = (buttondata_t*) data;
    button_data->color_index = (button_data->color_index + 1) % NB_COULEURS;

    GtkCssProvider *provider = gtk_css_provider_new();
    char css[256];
    snprintf(css, sizeof(css), "#button%d { background-color: %s; }", GPOINTER_TO_INT(g_object_get_data(G_OBJECT(button_data->button), "button-id")), colors[button_data->color_index]);

    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(GTK_WIDGET(button_data->button));
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    g_object_unref(provider);
}





void set_color(vue_t* vue, int num) {
		for (int j = 0; j < TAILLE_COMBI; j++) {
	 //    		vue->combi[i][j].color_index = (i+j) % NB_COULEURS;

				// Set an ID for each button
			char button_name[16];
			snprintf(button_name, sizeof(button_name), "button%d", j);
			gtk_widget_set_name(GTK_WIDGET(vue->combi[num][j].button), button_name);

			g_object_set_data(G_OBJECT(vue->combi[num][j].button), "button-id", GINT_TO_POINTER(j));

			
			g_signal_connect(G_OBJECT(vue->combi[num][j].button), "clicked", G_CALLBACK(on_button_clicked), &vue->combi[num][j]);

			GtkCssProvider *provider = gtk_css_provider_new();
			char css[256];
			
			if(vue->combi[num][j].color_index != COULEUR_INDETERMINEE)
				snprintf(css, sizeof(css), "#button%d { background-color: %s; }", j, colors[vue->combi[num][j].color_index]);

			gtk_css_provider_load_from_data(provider, css, -1, NULL);
			GtkStyleContext *context = gtk_widget_get_style_context(GTK_WIDGET(vue->combi[num][j].button));
			gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

			g_object_unref(provider);
		}
}
