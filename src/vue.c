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
    gtk_window_set_resizable(GTK_WINDOW(vue->f), FALSE);

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, "button { transition: background-color 0.3s ease; }", -1, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(GTK_WIDGET(vue->f));
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);

    // Conteneur vertical global
    GtkWidget *vbox_global = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(vue->f), vbox_global);

    // Création de la boîte horizontale pour le score, centrée
    GtkWidget *hbox_score = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *score_label = gtk_label_new("Score");
    gtk_widget_set_halign(score_label, GTK_ALIGN_CENTER); 
    gtk_box_pack_start(GTK_BOX(hbox_score), score_label, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox_global), hbox_score, FALSE, FALSE, 10);

    // Configuration principale
    vue->b_main = (GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(vbox_global), GTK_WIDGET(vue->b_main), TRUE, TRUE, 0);

    vue->b_choix = (GtkBox*) gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    vue->b_plateau = (GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 30);

    vue->b_gauche = (GtkBox*) gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    vue->b_droite = (GtkBox*) gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    GtkWidget *vbox_buttons = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);  

    for (int i = 0; i < NB_ESSAIS + 1; i++) {
        vue->b_essai[i] = (GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
        vue->b_ind[i] = (GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

        for (int j = 0; j < TAILLE_COMBI; j++) {
            vue->combi[i][j].button = (GtkButton*) gtk_button_new();
            vue->combi[i][j].color_index = COULEUR_INDETERMINEE;
            //gtk_widget_set_size_request(GTK_WIDGET(vue->combi[i][j].button), 40, 40);
            //gtk_box_pack_start(vue->b_essai[i], GTK_WIDGET(vue->combi[i][j].button), TRUE, TRUE, 0);
            char button_id[32];
            snprintf(button_id, sizeof(button_id), "button%d_%d", i, j); 
            gtk_widget_set_name(GTK_WIDGET(vue->combi[i][j].button), button_id);




            gtk_widget_set_size_request(GTK_WIDGET(vue->combi[i][j].button), 40, 40); // Taille du bouton
            gtk_box_pack_start(GTK_BOX(vue->b_essai[i]), GTK_WIDGET(vue->combi[i][j].button), TRUE, TRUE, 0);
            g_signal_connect(GTK_WIDGET(vue->combi[i][j].button), "clicked", G_CALLBACK(on_combi_button_clicked), NULL);
            g_object_set_data(G_OBJECT(GTK_WIDGET(vue->combi[i][j].button)), "color-index", GINT_TO_POINTER(0)); // Initialiser l'index de couleur



            if (i == 0) {
                gtk_button_set_label(vue->combi[i][j].button, "?");
                gtk_widget_set_sensitive(GTK_WIDGET(vue->combi[i][j].button), FALSE);
            }

            vue->button_ind[i][j] = (GtkButton*) gtk_button_new();
            gtk_widget_set_size_request(GTK_WIDGET(vue->button_ind[i][j]), 10, 40);
            gtk_widget_set_sensitive(GTK_WIDGET(vue->button_ind[i][j]), FALSE);
            gtk_box_pack_start(vue->b_ind[i], GTK_WIDGET(vue->button_ind[i][j]), FALSE, FALSE, 0);


            //g_signal_connect(G_OBJECT(vue->combi[i][j].button), "clicked", G_CALLBACK(set_color), &vue);

        }
        gtk_box_pack_start(vue->b_gauche, GTK_WIDGET(vue->b_essai[i]), TRUE, TRUE, 0);
        gtk_box_pack_start(vue->b_droite, GTK_WIDGET(vue->b_ind[i]), TRUE, TRUE, 0);
        
        // Gestion des marges 
        gtk_widget_set_margin_start(GTK_WIDGET(vue->b_main), 20); 
        gtk_widget_set_margin_end(GTK_WIDGET(vue->b_main), 10);  
        gtk_widget_set_margin_top(GTK_WIDGET(vue->b_main), 10);  
        gtk_widget_set_margin_bottom(GTK_WIDGET(vue->b_main), 20);    
    }

    GtkWidget *btn_rejouer = gtk_button_new_with_label("Rejouer");
    GtkWidget *btn_regles = gtk_button_new_with_label("Règles");
    GtkWidget *btn_valider = gtk_button_new_with_label("Valider");
    GtkWidget *btn_abandonner = gtk_button_new_with_label("Abandonner");
    GtkWidget *btn_mode = gtk_button_new_with_label("Mode");

    g_signal_connect(G_OBJECT(btn_regles), "clicked", G_CALLBACK(on_regles_clicked), &vue);
    g_signal_connect(G_OBJECT(btn_valider), "clicked", G_CALLBACK(valider_essai_actuel), &vue);
    g_signal_connect(G_OBJECT(btn_abandonner), "clicked", G_CALLBACK(on_abandonner_clicked), &vue);
    g_signal_connect(G_OBJECT(btn_mode), "clicked", G_CALLBACK(on_mode_clicked), &vue);

    // Taille uniforme pour les boutons 
    int button_width = 150;  
    int button_height = 30; 
    gtk_widget_set_size_request(btn_rejouer, button_width, button_height);
    gtk_widget_set_size_request(btn_regles, button_width, button_height);
    gtk_widget_set_size_request(btn_valider, button_width, button_height);
    gtk_widget_set_size_request(btn_abandonner, button_width, button_height);
    gtk_widget_set_size_request(btn_mode, button_width, button_height);

    // Alignement des boutons au centre 
    gtk_widget_set_halign(btn_rejouer, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(btn_regles, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(btn_valider, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(btn_abandonner, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(btn_mode, GTK_ALIGN_CENTER);

    // Insertion des boutons dans la boîte verticale
    gtk_box_pack_start(GTK_BOX(vbox_buttons), btn_rejouer, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox_buttons), btn_regles, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox_buttons), btn_valider, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox_buttons), btn_abandonner, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox_buttons), btn_mode, FALSE, FALSE, 0);

    // Ajout de la boîte verticale à la vue
    gtk_box_pack_start(GTK_BOX(vue->b_choix), vbox_buttons, TRUE, FALSE, 0);
    gtk_box_pack_start(vue->b_plateau, GTK_WIDGET(vue->b_gauche), TRUE, TRUE, 0);
    gtk_box_pack_start(vue->b_plateau, GTK_WIDGET(vue->b_droite), TRUE, TRUE, 0);
    gtk_box_pack_start(vue->b_main, GTK_WIDGET(vue->b_choix), TRUE, TRUE, 0);
    gtk_box_pack_start(vue->b_main, GTK_WIDGET(vue->b_plateau), TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(vue->f), GTK_WIDGET(vue->b_main));

    return vue;
}

void lib_vue(vue_t* vue) {
    if (vue != NULL) free(vue);
}

/*

void on_button_clicked(GtkWidget *widget, gpointer data) {
    buttondata_t* button_data = (buttondata_t*) data;
    button_data->color_index = (button_data->color_index + 1) % NB_COULEURS;

    GtkCssProvider *provider = gtk_css_provider_new();
    char css[256];
    snprintf(css, sizeof(css), "#button%d { background : %s; }", GPOINTER_TO_INT(g_object_get_data(G_OBJECT(button_data->button), "button-id")), colors[button_data->color_index]);

    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(GTK_WIDGET(button_data->button));
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    g_object_unref(provider);
}

void set_color(vue_t* vue, int num) {
		for (int j = 0; j < TAILLE_COMBI; j++) {
	        // vue->combi[i][j].color_index = (i+j) % NB_COULEURS;
			char button_name[16];
			snprintf(button_name, sizeof(button_name), "button%d", j);
			gtk_widget_set_name(GTK_WIDGET(vue->combi[num][j].button), button_name);

			g_object_set_data(G_OBJECT(vue->combi[num][j].button), "button-id", GINT_TO_POINTER(j));

			
			g_signal_connect(G_OBJECT(vue->combi[num][j].button), "clicked", G_CALLBACK(on_button_clicked), &vue->combi[num][j]);

            GtkCssProvider *provider = gtk_css_provider_new();
            char css[256] = "";
			
			if(vue->combi[num][j].color_index != COULEUR_INDETERMINEE)
				snprintf(css, sizeof(css), "#button%d { background: %s; }", j, colors[vue->combi[num][j].color_index]);

			gtk_css_provider_load_from_data(provider, css, -1, NULL);
			GtkStyleContext *context = gtk_widget_get_style_context(GTK_WIDGET(vue->combi[num][j].button));
			gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

			g_object_unref(provider);
		}
}
	
*/



void on_regles_clicked(GtkWidget *widget, gpointer data) {
    //vue_t *vue = (vue_t *)data;
    GtkWidget *popup = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(popup), "Règles du jeu");
    gtk_window_set_default_size(GTK_WINDOW(popup), 800, 900);

    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("./regles.png", NULL);
    GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(pixbuf, 800, 900, GDK_INTERP_BILINEAR);

    GtkWidget *background_image = gtk_image_new_from_pixbuf(scaled_pixbuf);
    gtk_container_add(GTK_CONTAINER(popup), background_image);

    g_object_unref(G_OBJECT(pixbuf));
    g_object_unref(G_OBJECT(scaled_pixbuf));

    gtk_widget_show_all(popup);
}


void valider_essai_actuel(GtkWidget *widget, gpointer data) {
    int num = NB_ESSAIS;
    vue_t *vue = (vue_t *)data;

    for (int i = 0; i < TAILLE_COMBI; i++) {
        if (vue->combi[num][i].color_index == COULEUR_INDETERMINEE) {
            return;
        }
    }

    for (int i = 0; i < TAILLE_COMBI; i++) {
        gtk_widget_set_sensitive(GTK_WIDGET(vue->combi[num][i].button), FALSE);
    }

    for (int i = 0; i < TAILLE_COMBI; i++) {
        gtk_widget_set_sensitive(GTK_WIDGET(vue->combi[num - 1][i].button), TRUE);
    }

    num--;
}


// Initialise le modèle du jeu de mastermind
void initialiser_modele(vue_t* vue)
{
    srand(time(NULL));
    mastermind_initialiser_avec_secret(&vue->mastermind);
}

// Evènements pour le bouton "Abandonner"
void on_abandonner_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(data),
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_QUESTION,
                                               GTK_BUTTONS_YES_NO,
                                               "Voulez vous abandonner la partie ?");
    gtk_window_set_title(GTK_WINDOW(dialog), "Confirmation d'abandon");

    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_YES) {
        // Code pour gérer l'abandon de la partie (on dévoilera la combinaison mystère)
    }

    gtk_widget_destroy(dialog);
}

// Evènement pour le mode de jeu
void on_mode_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Choisissez votre mode de jeu",
                                                    GTK_WINDOW(data),
                                                    GTK_DIALOG_MODAL,
                                                    "Mode 1 : Joueur",
                                                    1,
                                                    "Mode 2 : Utilisateur",
                                                    2,
                                                    NULL);
    gtk_window_set_title(GTK_WINDOW(dialog), "Mode de jeu");

    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    switch (response) {
        case 1:
            // Code pour le Mode 1 : Joueur
            break;
        case 2:
            // Code pour le Mode 2 : Utilisateur
            break;
        default:
            // Aucune action si la boîte de dialogue est fermée autrement
            break;
    }

    gtk_widget_destroy(dialog);
}



void on_combi_button_clicked(GtkWidget *button, gpointer data) {
    // Récupérer l'index actuel de la couleur
    int color_index = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(button), "color-index"));

    // Incrémenter l'index pour changer de couleur
    color_index = (color_index + 1) % (sizeof(colors) / sizeof(colors[0]));

    // Mettre à jour l'index de la couleur dans les données associées au bouton
    g_object_set_data(G_OBJECT(button), "color-index", GINT_TO_POINTER(color_index));

    // Construire la chaîne CSS pour changer la couleur du bouton
    char css[256];
    snprintf(css, sizeof(css), "button { background: %s; }", colors[color_index]);

    // Appliquer le CSS aux boutons
    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(css_provider, css, -1, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(button);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(css_provider);
}

/*
void initialiser_dialog_box(vue_t* vue)
{
    m->b_fin_partie = gtk_dialog_new_with_buttons("Le Jeu du Mastermind",
                                                  GTK_WINDOW(vue->window),
                                                  GTK_DIALOG_MODAL,
                                                  GTK_STOCK_OK,
                                                  GTK_RESPONSE_OK,
                                                  NULL);
}

void afficher_fin_partie(GtkWidget *widget, vue_t* vue)
{
    gchar chaine[200];
    GtkWidget* label;

    sprintf(chaine, "Vous avez réussi à trouver le code secret en %d essais !\n", mastermind_get_num_dernier_essai(&vue->mastermind));

    if(mastermind_get_etat(&vue->mastermind) == ETAT_MM_PERDU)
        g_stpcpy(chaine, "Vous avez perdu ! Vous aurez plus de chance la prochaine fois !");


    label = gtk_label_new(chaine);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(vue->b_fin_partie)->vbox), label, TRUE, FALSE, 0);

    // Création, initialisation et destruction de la boîte de dialogue
    gtk_widget_show_all(vue->b_fin_partie);
    gtk_dialog_run(GTK_DIALOG(vue->b_fin_partie));
    gtk_widget_destroy(vue->b_fin_partie);
}
*/
