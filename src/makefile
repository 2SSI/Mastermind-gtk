# ______________ parametres de compilation
CFLAGS= -Wall --pedantic

# ______________ excecutables
all: mastermind-demo.exe

# mastermind-demo
mastermind-demo.exe: mastermind-demo.o mastermind.o stat-essai.o combinaison.o 
	gcc $^ -o $@

mastermind-demo.o: mastermind-demo.c mastermind.h combinaison.h stat-essai.h
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

# ______________ nettoyage
clean:
	rm -rf *.o *~

mrproper: clean
	rm -rf *.exe

