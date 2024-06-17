#include "vue.h"
#include <gtk/gtk.h>
#include "combinaison.h"
#include "stat-essai.h"
#include "mastermind.h"
#include "common.h"
#include <stdio.h>
#include <gst/gst.h>

// --> Commande pour installer la librairie (à faire dans le ficher source) // Pour le son du jeu
// test sudo apt-get install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev

#define DEFAULT_COLOR "darkgrey"
#define ACTIVE_COLOR "lightgrey"
#define RESET_BACKGOURND(x) set_button_color(GTK_WIDGET(x), DEFAULT_COLOR)
#define SET_ACTIVE(x) set_button_color(GTK_WIDGET(x), ACTIVE_COLOR)

const char *colors[] = {
    "red", "green", "blue", "magenta", 
    "orange", "yellow", "white", "black"
};

void set_button_color(GtkWidget *button, const char *color) {
    char css[256];
    snprintf(css, sizeof(css), "button { background: %s; }", color);
    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(css_provider, css, -1, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(button);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(css_provider);
}

void add_class(GtkWidget *widget, const char *class) {
    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    gtk_style_context_add_class(context, class);
}
GtkButton* rounded_button(GtkButton *button)
{
    add_class(GTK_WIDGET(button), "circular");
    return button;
}

couleur mastermind_colors[] = {
    COULEUR_ROUGE,
	COULEUR_VERT,
	COULEUR_BLEU,
	COULEUR_MAUVE,
	COULEUR_ORANGE,
	COULEUR_JAUNE,
	COULEUR_BLANC,
	COULEUR_NOIR
};

void enable_line(vue_t* vue, int line){
    for(int i=0; i<TAILLE_COMBI; i++){
        gtk_widget_set_sensitive(GTK_WIDGET(vue->combi[line][i].button), TRUE);
        SET_ACTIVE(vue->combi[line][i].button);
    }
    if(vue->mode == PLAYER_MODE)
    {
        vue->line = line;
    }
}

vue_t* init_vue() {
    vue_t* vue = (vue_t*) malloc(sizeof(vue_t));
    vue->game_state = GAME_STARTED;
    vue->rules_open = 0;
    vue->line = 12;
    vue->mode = PLAYER_MODE;
    if (vue == NULL) {
        perror("échec malloc");
        exit(-1);
    }
    //gst_init(&argc, &argv);

    vue->f = (GtkWindow*) gtk_window_new(GTK_WINDOW_TOPLEVEL);    
    gtk_window_set_title(GTK_WINDOW(vue->f), "Mastermind");
    gtk_window_set_default_size(vue->f, 800, 900);
    gtk_window_set_resizable(vue->f, FALSE);

    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css_provider, "./design.css", NULL);
    gtk_style_context_add_provider(gtk_widget_get_style_context(GTK_WIDGET(vue->f)), GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Conteneur vertical global
    GtkWidget *vbox_global = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(vue->f), vbox_global);

    // Création de la boîte horizontale pour le score, centrée
    GtkWidget *hbox_score = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *score_label = gtk_label_new("Bienvenue sur le jeu Mastermind"); //"Score" (à modifier après)
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
            vue->combi[i][j].button = rounded_button((GtkButton*) gtk_button_new());
            vue->combi[i][j].color_index = COULEUR_INDETERMINEE;
            char button_id[32];
            snprintf(button_id, sizeof(button_id), "button%d_%d", i, j); 
            gtk_widget_set_name(GTK_WIDGET(vue->combi[i][j].button), button_id);

            gtk_widget_set_size_request(GTK_WIDGET(vue->combi[i][j].button), 40, 40); // Taille du bouton
            gtk_box_pack_start(GTK_BOX(vue->b_essai[i]), GTK_WIDGET(vue->combi[i][j].button), TRUE, TRUE, 0);
            g_signal_connect(GTK_WIDGET(vue->combi[i][j].button), "clicked", G_CALLBACK(on_combi_button_clicked), NULL);
            g_object_set_data(G_OBJECT(GTK_WIDGET(vue->combi[i][j].button)), "color-index", GINT_TO_POINTER(-1)); // Initialiser l'index de couleur
            RESET_BACKGOURND(vue->combi[i][j].button);
            gtk_style_context_add_provider(gtk_widget_get_style_context(GTK_WIDGET(vue->combi[i][j].button)), GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

            if (i == 0) {
                gtk_button_set_label(vue->combi[i][j].button, "?");
                gtk_widget_set_sensitive(GTK_WIDGET(vue->combi[i][j].button), FALSE);
            }

            vue->button_ind[i][j] = (GtkButton*) gtk_button_new();
            gtk_widget_set_size_request(GTK_WIDGET(vue->button_ind[i][j]), 10, 40);
            gtk_widget_set_sensitive(GTK_WIDGET(vue->button_ind[i][j]), FALSE);
            RESET_BACKGOURND(vue->button_ind[i][j]);
            gtk_box_pack_start(GTK_BOX(vue->b_ind[i]), GTK_WIDGET(vue->button_ind[i][j]), FALSE, FALSE, 0);
        }
        gtk_box_pack_start(GTK_BOX(vue->b_gauche), GTK_WIDGET(vue->b_essai[i]), TRUE, TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vue->b_droite), GTK_WIDGET(vue->b_ind[i]), TRUE, TRUE, 0);
        
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

    g_signal_connect(G_OBJECT(btn_rejouer), "clicked", G_CALLBACK(on_rejouer_clicked), vue);
    g_signal_connect(G_OBJECT(btn_regles), "clicked", G_CALLBACK(on_regles_clicked), vue);
    g_signal_connect(G_OBJECT(btn_valider), "clicked", G_CALLBACK(on_valider_clicked), vue);
    g_signal_connect(G_OBJECT(btn_abandonner), "clicked", G_CALLBACK(on_abandonner_clicked), vue);
    g_signal_connect(G_OBJECT(btn_mode), "clicked", G_CALLBACK(on_mode_clicked), vue);

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

    gtk_widget_set_name(GTK_WIDGET(btn_rejouer), "menuButtons");

    // Ajout de la boîte verticale à la vue
    gtk_box_pack_start(GTK_BOX(vue->b_choix), vbox_buttons, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vue->b_plateau), GTK_WIDGET(vue->b_gauche), TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vue->b_plateau), GTK_WIDGET(vue->b_droite), TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vue->b_main), GTK_WIDGET(vue->b_choix), TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vue->b_main), GTK_WIDGET(vue->b_plateau), TRUE, TRUE, 0);

    gtk_widget_show_all(GTK_WIDGET(vue->f));
    enable_line(vue, vue->mode == PLAYER_MODE ? 12 : 0);
    
    g_object_unref(css_provider);
    play_background_music();
    return vue;
}

void reset(vue_t *vue)
{
    for (int i = 0; i < NB_ESSAIS + 1; i++) {
        for (int j = 0; j < TAILLE_COMBI; j++) {
            vue->combi[i][j].color_index = COULEUR_INDETERMINEE;
            g_object_set_data(G_OBJECT(GTK_WIDGET(vue->combi[i][j].button)), "color-index", GINT_TO_POINTER(-1)); // Initialiser l'index de couleur
            RESET_BACKGOURND(vue->combi[i][j].button);

            if (i == 0) {
                gtk_button_set_label(vue->combi[i][j].button, vue->mode == PLAYER_MODE ? "?" : "");
                gtk_widget_set_sensitive(GTK_WIDGET(vue->combi[i][j].button), FALSE);
            }
            gtk_widget_set_sensitive(GTK_WIDGET(vue->button_ind[i][j]), FALSE);
            RESET_BACKGOURND(vue->button_ind[i][j]);
        }
    }
    vue->line = 12;
    enable_line(vue, vue->mode == PLAYER_MODE ? 12 : 0);
    vue->game_state = GAME_STARTED;
}

void lib_vue(vue_t* vue) {
    if (vue != NULL) free(vue);
}

int couleur_index(couleur c){
    for (int i = 0; i < 8; i++) {
        if (mastermind_colors[i] == c) {
            return i;
        }
    }
    return 0;
}

void reveal_combi(vue_t* vue){
    for(int i=0; i<TAILLE_COMBI; i++){
        GtkButton *button = vue->combi[0][i].button;
        set_button_color(GTK_WIDGET(button), colors[couleur_index(vue->mastermind->plateau[12][i])]);
        gtk_button_set_label(button, "");

    }
}

void disable_line(vue_t* vue, int line){
    printf("Disable line %d\n", line);
    for(int i=0; i<TAILLE_COMBI; i++){
        printf("Disabled block:%d\n", i);
        gtk_widget_set_sensitive(GTK_WIDGET(vue->combi[line][i].button), FALSE);
    }
}


void on_rejouer_clicked(GtkWidget *widget, gpointer data) {
    vue_t *vue = (vue_t *)data;
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(vue->f),
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_QUESTION,
                                               GTK_BUTTONS_YES_NO,
                                               "Souhaitez-vous rejouer?");
    gtk_window_set_title(GTK_WINDOW(dialog), "Confirm restart");

    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_YES) {
        if(vue->on_game_restart != NULL)
        {
            vue->on_game_restart(vue->controller_state);
        }
    }

    gtk_widget_destroy(dialog);
}

void on_regles_clicked(GtkWidget *widget, gpointer data) {
    vue_t *vue = (vue_t *)data;
    if(vue->rules_open == 0)
    {
        vue->rules_open = 1;
        GtkWidget *popup = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(popup), "Règles du jeu");
        gtk_window_set_default_size(GTK_WINDOW(popup), 800, 900);
        g_signal_connect(G_OBJECT(popup), "destroy", G_CALLBACK(on_rules_destroyed), vue);

        GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("./assets/regles-bis.jpg", NULL);
        GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(pixbuf, 800, 900, GDK_INTERP_BILINEAR);

        GtkWidget *background_image = gtk_image_new_from_pixbuf(scaled_pixbuf);
        gtk_container_add(GTK_CONTAINER(popup), background_image);

        g_object_unref(G_OBJECT(pixbuf));
        g_object_unref(G_OBJECT(scaled_pixbuf));

        gtk_widget_show_all(popup);
    }
}

void on_rules_destroyed(GtkWidget *widget, gpointer data) {
    vue_t *vue = (vue_t *)data;
    vue->rules_open = 0;
}

// Evènements pour le bouton "Abandonner"
void on_abandonner_clicked(GtkWidget *widget, gpointer data) {
    vue_t *vue = (vue_t *)data;
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(vue->f),
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_QUESTION,
                                               GTK_BUTTONS_YES_NO,
                                               "Voulez vous abandonner la partie ?");
    gtk_window_set_title(GTK_WINDOW(dialog), "Confirmation d'abandon");

    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_YES) {
        reveal_combi(vue);
        disable_line(vue, vue->line);
        vue->game_state = GAME_ENDED;
    }

    gtk_widget_destroy(dialog);
}

// Evènement pour le mode de jeu
void on_mode_clicked(GtkWidget *widget, gpointer data) {
    vue_t *vue = (vue_t *)data;
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Choisissez votre mode de jeu",
                                                    GTK_WINDOW(vue->f),
                                                    GTK_DIALOG_MODAL,
                                                    "Mode 1 : Joueur",
                                                    PLAYER_MODE,
                                                    "Mode 2 : Ordinateur",
                                                    COMPUTER_MODE,
                                                    NULL);
    gtk_window_set_title(GTK_WINDOW(dialog), "Mode de jeu");
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    switch (response) {
        case PLAYER_MODE:
            vue->mode = PLAYER_MODE;
            vue->on_game_restart(vue->controller_state);
            break;
        case COMPUTER_MODE:
            vue->mode = COMPUTER_MODE;
            vue->on_game_restart(vue->controller_state);
            break;
        default:
            break;
    }

    gtk_widget_destroy(dialog);
}

void on_combi_button_clicked(GtkWidget *button, gpointer data) {

    // Récupération de l'index actuel de la couleur
    int color_index = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(button), "color-index"));
    color_index = (color_index + 1) % (sizeof(colors) / sizeof(colors[0]));
    g_object_set_data(G_OBJECT(button), "color-index", GINT_TO_POINTER(color_index));

    //play_clicked(); // PB : ralentit le clic sur les boutons combi
    set_button_color(button, colors[color_index]);
}

int button_to_color(GtkButton *button, couleur *color) {
    int color_index = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(button), "color-index"));
    if (color_index != -1) {
        *color = mastermind_colors[color_index];
        return 1;
    }
    return 0;
}


int set_secret(vue_t *vue) {
    int uninitialized = vue->mastermind->num_essai_encours == 0;
    for(int i = 1; i <= TAILLE_COMBI; i++)
    {
        couleur color;
        if(uninitialized && button_to_color(vue->combi[0][i - 1].button, &color))
        {
            mastermind_set_secret(vue->mastermind, i, color);
        }
        else
        {
            return 0;
        }
    }
    mastermind_valider_secret(vue->mastermind);
    return 1;
}

void on_valider_clicked(GtkWidget *widget, gpointer data) {
    vue_t *vue = (vue_t *)data;
    if(vue->game_state == GAME_ENDED)
    {
        return;
    }
    if(vue->mode == COMPUTER_MODE)
    {
        if(set_secret(vue))
        {
            disable_line(vue, 0);
        }

        if(mastermind_est_secret_valide(vue->mastermind))
        {
            for(int i = 0; i < TAILLE_COMBI; i++)
            {
                int color_index = rand()%8;
                set_button_color(GTK_WIDGET(vue->combi[vue->line][i].button), colors[color_index]);
                g_object_set_data(G_OBJECT(vue->combi[vue->line][i].button), "color-index", GINT_TO_POINTER(color_index));
                
            }
        }
    }
    couleur row_colors[TAILLE_COMBI] = {COULEUR_INDETERMINEE};
    int tot = 0;
    for (int i = 0 ; i < TAILLE_COMBI; i++) {
        if(button_to_color(vue->combi[vue->line][i].button, &row_colors[i]))
        {
            tot++;
        }
    }
    if(tot == TAILLE_COMBI)
    {
        disable_line(vue, vue->line);
        for(int i = 1; i <= tot; i++)
        {
            mastermind_set_essai_encours(vue->mastermind, i, row_colors[i-1]);
        }
        if(mastermind_est_essai_encours_valide(vue->mastermind))
        {
            int offset = 0;
            int stat = vue->mastermind->num_essai_encours-1;
            mastermind_valider_essai_encours(vue->mastermind);
            play_valider("assets/valider-button.mp3");
            for (int i = 0; i < vue->mastermind->stat[stat].ordo && offset < TAILLE_COMBI; i++, offset++)
            {
                GtkWidget *button = GTK_WIDGET(vue->button_ind[vue->line][offset]);
                set_button_color(button, "black");
            }
            for (int i = 0; i < (vue->mastermind->stat[stat].non_ordo-vue->mastermind->stat[stat].ordo) && offset < TAILLE_COMBI; i++, offset++)
            {
                GtkWidget *button = GTK_WIDGET(vue->button_ind[vue->line][offset]);
                set_button_color(button, "white");
            }

            if(vue->mastermind->stat[stat].ordo == TAILLE_COMBI)
            {
                reveal_combi(vue);
                play_win();
                vue->game_state = GAME_ENDED;
            }
            else
            {
                if(vue->mode == PLAYER_MODE)
                {
                    enable_line(vue, vue->line-1);
                }
                else
                {
                    vue->line--;
                }
            }
        }
    }
    if(vue->line == 0)
    {
        reveal_combi(vue);
        vue->game_state = GAME_ENDED;
        disable_line(vue, vue->line);
    }
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

void play_background_music() {
    GstElement *pipeline, *source, *decoder, *sink;

    // Création du "pipeline"
    pipeline = gst_pipeline_new("audio-pipeline");
    source = gst_element_factory_make("filesrc", "audio-source");
    decoder = gst_element_factory_make("decodebin", "audio-decoder");
    sink = gst_element_factory_make("autoaudiosink", "audio-output");

    if (!pipeline || !source || !decoder || !sink) {
        g_printerr("Erreur, un son est déjà joué\n");
        return;
    }
    g_object_set(G_OBJECT(source), "location", "assets/game-music.mp3", NULL);
    gst_bin_add_many(GST_BIN(pipeline), source, decoder, sink, NULL);
    gst_element_link(source, decoder);
    g_signal_connect(decoder, "pad-added", G_CALLBACK(on_pad_added), sink);
    gst_element_set_state(pipeline, GST_STATE_PLAYING);
}

void on_pad_added(GstElement *element, GstPad *pad, gpointer data) {
    GstPad *sinkpad;
    GstElement *sink = (GstElement *)data;
    sinkpad = gst_element_get_static_pad(sink, "sink");
    gst_pad_link(pad, sinkpad);
    gst_object_unref(sinkpad);
}

void play_valider(const char *file_path) {
    GstElement *pipeline, *source, *decoder, *sink;
    pipeline = gst_pipeline_new("sound-effect-pipeline");
    source = gst_element_factory_make("filesrc", "source");
    decoder = gst_element_factory_make("decodebin", "decoder");
    sink = gst_element_factory_make("autoaudiosink", "sink");

    if (!pipeline || !source || !decoder || !sink) {
        g_printerr("Failed to create pipeline elements for sound effect\n");
        return;
    }
    g_object_set(G_OBJECT(source), "location", file_path, NULL);
    gst_bin_add_many(GST_BIN(pipeline), source, decoder, sink, NULL);
    gst_element_link(source, decoder);
    g_signal_connect(decoder, "pad-added", G_CALLBACK(on_pad_added), sink);

    // Début de la lecture
    gst_element_set_state(pipeline, GST_STATE_PLAYING);
    // Clean du son 
    GstBus *bus = gst_element_get_bus(pipeline);
    GstMessage *msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_EOS | GST_MESSAGE_ERROR);

    if (msg != NULL) {
        gst_message_unref(msg);
    }
    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(pipeline));
}


void play_win() {
    GstElement *pipeline, *source, *decoder, *sink;
    pipeline = gst_pipeline_new("win-sound-pipeline");
    source = gst_element_factory_make("filesrc", "source");
    decoder = gst_element_factory_make("decodebin", "decoder");
    sink = gst_element_factory_make("autoaudiosink", "sink");

    if (!pipeline || !source || !decoder || !sink) {
        g_printerr("Failed to create pipeline elements for win sound\n");
        return;
    }

    g_object_set(G_OBJECT(source), "location", "assets/win.mp3", NULL);
    gst_bin_add_many(GST_BIN(pipeline), source, decoder, sink, NULL);
    gst_element_link(source, decoder);
    g_signal_connect(decoder, "pad-added", G_CALLBACK(on_pad_added), sink);
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    GstBus *bus = gst_element_get_bus(pipeline);
    GstMessage *msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_EOS | GST_MESSAGE_ERROR);
    if (msg != NULL) {
        gst_message_unref(msg);
    }
    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(pipeline));
}

/*
void play_clicked() {
    GstElement *pipeline, *source, *decoder, *sink;
    pipeline = gst_pipeline_new("click-sound-pipeline");
    source = gst_element_factory_make("filesrc", "source");
    decoder = gst_element_factory_make("decodebin", "decoder");
    sink = gst_element_factory_make("autoaudiosink", "sink");

    if (!pipeline || !source || !decoder || !sink) {
        g_printerr("Failed to create pipeline elements for click sound\n");
        return;
    }

    g_object_set(G_OBJECT(source), "location", "assets/button-click.mp3", NULL);
    gst_bin_add_many(GST_BIN(pipeline), source, decoder, sink, NULL);
    gst_element_link(source, decoder);
    g_signal_connect(decoder, "pad-added", G_CALLBACK(on_pad_added), sink);
    gst_element_set_state(pipeline, GST_STATE_PLAYING);
    GstBus *bus = gst_element_get_bus(pipeline);
    GstMessage *msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_EOS | GST_MESSAGE_ERROR);

    if (msg != NULL) {
        gst_message_unref(msg);
    }
    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(pipeline));
}
*/
