#!/bin/sh

if (g++ src/main.cpp -o asteroids -I ./include -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio) then
	echo "Compiled successfully. Use ./asteroids to run."
else
	echo "Error occured while compiling. Exiting."
fi
