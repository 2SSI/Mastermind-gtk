#include "controleur.h"
#include "mastermind.h"
#include "stat-essai.h"
#include "combinaison.h"
#include "vue.h"


ctrl_t ctrl_construire(){
	mastermind m;
	vue_t vue;
	ctrl_t c;
	
	c.m = &m;
	c.vue = &vue;
	
	mastermind_initialiser(&m);
	init_vue(&vue);
	
	return c;
}


void lib_ctrl(ctrl_t* c){
	lib_vue(&(c->vue));
	if(c != NULL)
		free(c);
}


ctrl_t ctrl_color(ctrl_t c /* tab de couleurs */){
	if(mastermind_get_etat(c.m) == ETAT_MM_ENCOURS){
		int num = mastermind_get_num_essai_encours(c.m);
		gtk_widget_set_sensitive(GTK_WIDGET(c.vue->b_essai[num]), TRUE);
		
		for(int j=0; j<TAILLE_COMBI; j=j+1){
				/* condition bouton cliqué */
			g_signal_connect(G_OBJECT((c.vue)->button_essai[num + 1][j]), "clicked", G_CALLBACK( /* fonction changer couleur */ ), /* pointeur tab */);
		}
	}
}


