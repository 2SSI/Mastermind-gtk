# ______________ parametres de compilation
CFLAGS= -Wall --pedantic

# ______________ excecutables
all: main.exe

# main.exe
main.exe: main.o mastermind.o stat-essai.o combinaison.o vue.o controleur.o
	gcc $^ -o $@ `pkg-config gtk+-3.0 --cflags --libs`

main.o: main.c mastermind.h combinaison.h stat-essai.h vue.h controleur.h
	gcc -c $< -o $@ $(CFLAGS) `pkg-config gtk+-3.0 --cflags --libs`

# ______________ objets bibliothèque

# mastermind
mastermind.o: mastermind.c mastermind.h combinaison.h stat-essai.h
	gcc -c $< -o $@ $(CFLAGS)

# combinaison
combinaison.o: combinaison.c combinaison.h
	gcc -c $< -o $@ $(CFLAGS)

# stat-essai
stat-essai.o: stat-essai.c stat-essai.h
	gcc -c $< -o $@ $(CFLAGS)
	

# vue
vue.o: vue.c vue.h mastermind.h combinaison.h stat-essai.h
	gcc -c $< -o $@ $(CFLAGS) `pkg-config gtk+-3.0 --cflags --libs`
	
# controleur
controleur.o: controleur.c controleur.h vue.h mastermind.h combinaison.h stat-essai.h
	gcc -c $< -o $@ $(CFLAGS) `pkg-config gtk+-3.0 --cflags --libs`

# ______________ nettoyage
clean:
	rm -rf *.o *~

mrproper: clean
	rm -rf *.exe

