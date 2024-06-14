#include <stdlib.h>
#include "mastermind.h"
#include "stat-essai.h"
#include "combinaison.h"
#include "vue.h"
#include "controleur.h"
#include <gtk/gtk.h>

int main(){
	
	ctrl_t c;
	gtk_init(NULL, NULL);

	c = ctrl_construire();
	
	/*c = ctrl_color(c);*/
	
   	gtk_widget_show_all(GTK_WIDGET(c.vue->f));
    g_signal_connect(G_OBJECT(c.vue->f), "destroy", G_CALLBACK(gtk_main_quit), NULL);
   	gtk_main();
    
    lib_ctrl(&c);
	return EXIT_SUCCESS;
}
