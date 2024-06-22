# 🪅 Mastermind GTK 3.0

Graphic user interface of mastermind game in C using GTK 3.0 library.

https://github.com/I2S9/Mastermind-GTK/assets/111307883/d15fa269-5c49-415b-8f03-60af5fe57664

## Requirements 

1. Download the [GTK 3.0](https://www.gtk.org/docs/installations/index) version from your favorite OS.

2. Install Gstreamer library for song and uncomment lines that refers to it.

         test sudo apt-get install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev

## Try it out

        make all
        ./main.exe

## Further Step

- Enhance the graphic user interface.

- Create a score (indicator) for the user for the purpose of read his performance and his logical mind. 

- Improved user experience for avoid that the latter has to click several times to change the color of the buttons
of the combination.

- Display in a graphical window of the history of Scores/performance of the user to measure its evolution during the game.

- Modification of the function that randomly generates the secret combination to introduce different difficulty levels, making the game more interesting.

- Creation of a stopwatch to evaluate for each the length of time it takes the user (or the machine) to find the secret combination.
