#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

class Conway
{
	private: 
		int **Field, **FieldOld;
		int FieldSize;
		int breed = 1;

		int cntNghbr(int i, int j){
			int cnt=0;
			int left = (j == 0) ? FieldSize-1 : j-1;
			int right = (j == FieldSize-1) ? 0 : j+1;
			int upper = (i == 0) ? FieldSize-1 : i-1;
			int bootom = (i == FieldSize-1) ? 0 : i+1;
			if (FieldOld[upper][left] == breed) ++cnt;
			if (FieldOld[upper][j] == breed) ++cnt;
			if (FieldOld[upper][right] == breed) ++cnt;
			if (FieldOld[i][left] == breed) ++cnt;
			if (FieldOld[i][right] == breed) ++cnt;
			if (FieldOld[bootom][left] == breed) ++cnt;
			if (FieldOld[bootom][j] == breed) ++cnt;
			if (FieldOld[bootom][right] == breed) ++cnt;
			return cnt;
		}

	public:
		Conway(int N, int varBreed = 1){
			FieldSize = N;
			breed = varBreed;
			Field = new int*[N];
			FieldOld = new int*[N];
			for(int i = 0; i < N; ++i)
	    		Field[i] = new int[N];
	    	for(int i = 0; i < N; ++i)
	    		FieldOld[i] = new int[N];
	    	printf("Base Fields[N][N] constructed...\n");
		}

		virtual ~Conway(){
			for(int i = 0; i < FieldSize; ++i)
	    		delete [] Field[i];
	    	for(int i = 0; i < FieldSize; ++i)
	    		delete [] FieldOld[i];
			delete [] Field;
			delete [] FieldOld;
		}

		void FieldFillRand(float rate){
			srand (time(NULL));
			for (int i = 1; i < FieldSize-1; ++i) {
				printf("\n");
				for (int j = 1; j < FieldSize-1; ++j)
				{
					if (rand() % 2) Field[i][j] = breed;
					printf("%d ",Field[i][j]);
				}
			}
			printf("Random Fill finished...\n");
		}


		void addGlider(int shift_x = 0, int shift_y = 0, int direction = 0){
			if (direction == 0) direction = rand()%4 + 1;
			switch (direction) {
				case 1 : Field[1+shift_x][2+shift_y]=Field[2+shift_x][3+shift_y]=Field[3+shift_x][1+shift_y]=Field[3+shift_x][2+shift_y]=Field[3+shift_x][3+shift_y]=breed; break;
				case 2 : Field[1+shift_x][2+shift_y]=Field[1+shift_x][3+shift_y]=Field[2+shift_x][1+shift_y]=Field[2+shift_x][3+shift_y]=Field[3+shift_x][3+shift_y]=breed; break;
				case 3 : Field[1+shift_x][1+shift_y]=Field[1+shift_x][2+shift_y]=Field[1+shift_x][3+shift_y]=Field[2+shift_x][1+shift_y]=Field[3+shift_x][2+shift_y]=breed; break;
				case 4 : Field[1+shift_x][1+shift_y]=Field[2+shift_x][1+shift_y]=Field[2+shift_x][3+shift_y]=Field[3+shift_x][1+shift_y]=Field[3+shift_x][2+shift_y]=breed; break;
			}
			printf("Glider %d added on [%d][%d]\n",direction,shift_x,shift_y);
		}

		int Step(){
			int alive=0;
			for (int i = 0; i < FieldSize; ++i)
				for (int j = 0; j < FieldSize; ++j){
					FieldOld[i][j] = Field[i][j]; 
					//Field[i][j] = 0;
				}
			short cur_cntNghbr;
			for (int i = 0; i < FieldSize; ++i)
				for (int j = 0; j < FieldSize; ++j){
					cur_cntNghbr=cntNghbr(i,j);
					if (cur_cntNghbr == 3) {Field[i][j] = breed; ++alive;}
					else if ((cur_cntNghbr == 2) && (FieldOld[i][j] == breed)) {Field[i][j] = breed; ++alive;}
						 else if (Field[i][j] > 0) --Field[i][j];
				}
			return alive;
		}

		int Cell(int x, int y){
			return Field[x][y];
		}

};
