# ______________ parametres de compilation
CFLAGS= -Wall --pedantic -g3 `pkg-config gtk+-3.0 --cflags` ##gstreamer-1.0

# ______________ lien des bibliothèques
LIBS = `pkg-config gtk+-3.0  --libs` #gstreamer-1.0

# ______________ excecutables
all: main.exe

# main.exe
main.exe: main.o mastermind.o stat-essai.o combinaison.o vue.o controleur.o
	gcc $^ -o $@ $(CFLAGS) $(LIBS)

main.o: main.c mastermind.h combinaison.h stat-essai.h vue.h controleur.h
	gcc -c $< -o $@ $(CFLAGS)

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
	gcc -c $< -o $@ $(CFLAGS)

# controleur
controleur.o: controleur.c controleur.h vue.h mastermind.h combinaison.h stat-essai.h
	gcc -c $< -o $@ $(CFLAGS)

# ______________ nettoyage
clean:
	rm -rf *.o *~

mrproper: clean
	rm -rf *.exe
