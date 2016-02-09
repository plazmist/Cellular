#include <string>
#include <unistd.h>
#include "conway.h"

const int N = 10;

int main(int, char**){
	printf("Before constructor\n");
	Conway Cellular(10);
	printf("After constructor\n");
	//Cellular.FieldFillRand();
	Cellular.addGlider(1,1);	

	int alive;
	bool quit = false;
	while (!quit){
		for (int i = 0; i < N; ++i)
		{	printf("\n");
			for (int j = 0; j < N; ++j) {
				if (Cellular.Cell(i,j))
						printf("o ");
				else printf("  ");
				}
		}
		
		alive = Cellular.Step();
		printf("\nalive = %d\n", alive);
		sleep(1);
		system("clear");
	}
	return 0;
}
