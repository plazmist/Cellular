#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "res_path.h"
#include "cleanup.h"

/* 
 * Lesson 5: Clipping Sprite Sheets
 */
//Screen attributes
const int SCREEN_WIDTH  = 923;
const int SCREEN_HEIGHT = 860;

const int CLIP_W = 263;
const int CLIP_H = 263;
const int N = 500;

unsigned short FieldOld[N][N],Field[N][N];

void FieldFillRand(){
	for (int i = 1; i < N-1; ++i) {
		printf("\n");
		for (int j = 1; j < N-1; ++j)
		{
			Field[i][j] = rand() % 2;
			printf("%u ",Field[i][j]);
		}
	}
}

void addGlider(int shift_x = 0, int shift_y = 0){
/*	for (int i = 1; i < N-1; ++i) 
		for (int j = 1; j < N-1; ++j)
			Field[i][j] = 0;*/
	Field[1+shift_x][2+shift_y]=1;
	Field[2+shift_x][3+shift_y]=1;
	Field[3+shift_x][1+shift_y]=1;
	Field[3+shift_x][2+shift_y]=1;
	Field[3+shift_x][3+shift_y]=1;
	printf("Glider added on [%d][%d]\n",shift_x,shift_y);
}

int cntNghbr(int i, int j){
	int left = (j == 0) ? N-1 : j-1;
	int right = (j == N-1) ? 0 : j+1;
	int upper = (i == 0) ? N-1 : i-1;
	int bootom = (i == N-1) ? 0 : i+1;
	return FieldOld[upper][left] + FieldOld[upper][j] + FieldOld[upper][right] + FieldOld[i][left] + FieldOld[i][right] + FieldOld[bootom][left] + FieldOld[bootom][j] + FieldOld[bootom][right];
}

void ConwayCA(){
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j){
			FieldOld[i][j] = Field[i][j]; 
			Field[i][j] = 0;
		}
	short cur_cntNghbr;
/*	for (int i = 1; i < N-1; ++i)
		for (int j = 1; j < N-1; ++j){
			cur_cntNghbr=cntNghbr(i,j);
			if (cur_cntNghbr == 3) Field[i][j]=1;
			if ((cur_cntNghbr == 2) && (FieldOld[i][j] == 1)) Field[i][j]=1;
		}*/
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j){
			cur_cntNghbr=cntNghbr(i,j);
			if (cur_cntNghbr == 3) Field[i][j]=1;
			if ((cur_cntNghbr == 2) && (FieldOld[i][j] == 1)) Field[i][j]=1;
		}

}


SDL_Texture* renderText(const std::string &message, const std::string &fontFile, SDL_Color color,
		int fontSize, SDL_Renderer *renderer)
{ 
	TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
	SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);
	return texture;
}

int main(int, char**){
	
	srand (time(NULL));
	//FieldFillRand();
/*	addGlider(0,0);
	addGlider(0,5);
	addGlider(0,10);
	addGlider(0,15);
	addGlider(0,20);*/
	
	SDL_Init(SDL_INIT_VIDEO); 

	if (TTF_Init() != 0){
		
		SDL_Quit();
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow("Lesson 5", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	const std::string resPath = getResourcePath("Lesson5");

	SDL_Texture *tex = IMG_LoadTexture(renderer, "cell_green.png");
	SDL_Texture *texI = IMG_LoadTexture(renderer, "hex_102_88_black.png");

	SDL_Color color = { 255, 255, 255, 255 };
	SDL_Texture *image = renderText("TTF fonts are cool!", "sample.ttf", color, 64, renderer);


		double ratio = ((double)SCREEN_WIDTH / N) / CLIP_W;
		SDL_Rect dst, clip, dst2, text_dst;
		clip.x = 0;
		clip.y = 0;
		clip.w = CLIP_W;
		clip.h = CLIP_H;
		
		dst.x = 400;
		dst.y = 400;
		dst.w = CLIP_W * ratio;
		dst.h = CLIP_H * ratio;
		
		dst2.w = CLIP_W;
		dst2.h = CLIP_H;
		dst2.x = 400 + dst2.w/2;
		dst2.y = 400 + dst2.h/2;

		text_dst.w = 800;
		text_dst.h = 30;
		text_dst.x = 0;
		text_dst.y = 400;

		printf("dst.w = %d, dst.h = %d, ratio = %lf\n",dst.w,dst.h,ratio);
		//SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
		SDL_QueryTexture(texI, NULL, NULL, &clip.w, &clip.h);
		SDL_QueryTexture(tex, NULL, NULL, &clip.w, &clip.h);
		int iW, iH;
		SDL_QueryTexture(image, NULL, NULL, &iW, &iH);

	SDL_Event e;
	bool quit = false;
	double angle=0.0,res_ratio=1.05;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	int direction=0;
	while (!quit){
		//Event Polling
		while (SDL_PollEvent(&e)){
			if (e.type == SDL_QUIT){
				quit = true;
			}
			//Use number input to select which clip should be drawn
			if (e.type == SDL_KEYDOWN){
				switch (e.key.keysym.sym){
					case SDLK_ESCAPE:
						quit = true;
						break;
					default:
						break;
				}
			}
		}

		SDL_RenderClear(renderer);
		for (int i = 0; i < N; ++i)
		{
			dst.y = dst.h*i;
			for (int j = 0; j < N; ++j) {
				dst.x = j*dst.w;
				if (Field[i][j]) SDL_RenderCopy(renderer, tex, &clip, &dst);
				//else SDL_RenderCopy(renderer, texI, &clip, &dst);
			}
		}
		if (rand() % 10) addGlider(rand() % (N-3),rand() % (N-3)); 
		ConwayCA();

		//Update the screen
		SDL_RenderCopy(renderer, image, NULL, &text_dst);
		SDL_RenderPresent(renderer);
		//SDL_Delay(100);
	}
	//Clean up
	cleanup(tex, renderer, window);
	cleanup(texI, renderer, window);
	IMG_Quit();
	SDL_Quit();

	return 0;
}
