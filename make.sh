#!/bin/bash
rm main.o a.out
echo -e "\e[01;32m****************** COMPILE ******************\e[0m $1"
g++ -Wall -g -c -std=c++11 -I/usr/local/include $1 -o main.o  || echo -e "\e[01;31m****************** FUCK !!! ******************\e[0m" #&& exit 1
#g++ -o main.o $1 -std=c++11 `sdl-config --libs --cflags`
#g++ -Wall -Wextra -pedantic -std=c++11 main.o  -o  -rdynamic /usr/local/lib/libSDL2main.a /usr/local/lib/libSDL2.so -lpthread -lSDL2_ttf -Wl,-rpath,/usr/local/lib:

echo -e "\e[01;32m****************** BUILD *******************\e[0m $1"
g++ main.o -L/usr/local/lib -g -lSDL2_image -lSDL2 -lSDL2_ttf -Wl,-rpath=/usr/local/lib:

echo -e "\e[01;32m****************** RUN *********************\e[0m $1"
#./a.out >> 1.out 2>&1
#rm main.o a.out
