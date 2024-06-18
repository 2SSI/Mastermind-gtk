#include "controleur.h"
#include "mastermind.h"
#include "stat-essai.h"
#include "combinaison.h"
#include "vue.h"


void on_game_restart(void* data)
{
    vue_t *vue = (vue_t *)data;
    mastermind_initialiser(vue->mastermind);
    if(vue->mode == PLAYER_MODE)
    {
        mastermind_initialiser_avec_secret(vue->mastermind);
    }
    reset(vue);
}

ctrl_t ctrl_construire(){
    srand(time(NULL));
    mastermind* m = malloc(sizeof(mastermind));
    vue_t* vue;
    ctrl_t c;
    mastermind_initialiser(m);
    mastermind_initialiser_avec_secret(m);
    vue = init_vue();
    disable_all(vue);  
    c.m = m;
    c.vue = vue;
    vue->mastermind = m;
    vue->on_game_restart = &on_game_restart;
    vue->controller_state = vue;
    return c;
}


void lib_ctrl(ctrl_t* c){
    if(c != NULL)
    {
	    lib_vue(c->vue);
        free(c->m);
    }
}



void disable_all(vue_t* vue) {
    for (int i = 0; i < NB_ESSAIS; i++) {  
        if (i != NB_ESSAIS) {  
            for (int j = 0; j < TAILLE_COMBI; j++) {  
                gtk_widget_set_sensitive(GTK_WIDGET(vue->combi[i][j].button), FALSE);
            }
        }
    }
}
