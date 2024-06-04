#ifndef

#DEFINE CONTROLEUR_H


typedef struct{
	mastermind* m;
	vue_t* vue;
}ctrl_t;


ctrl_t ctrl_construire();
void lib_ctrl(ctrl_t*);





#endif
