#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "res_path.h"
#include "cleanup.h"
#include "conway.h"

const int SCREEN_HEIGHT = 860;
const int SCREEN_WIDTH  = SCREEN_HEIGHT * 1.25;

const int CLIP_W = 263;
const int CLIP_H = 263;
const int PIXELS_PER_CELL=20;
const int N = SCREEN_HEIGHT / PIXELS_PER_CELL;

unsigned short AliveLine[int(SCREEN_HEIGHT*0.25)];
unsigned short aliveLineHeight = SCREEN_HEIGHT / 2;
long AliveMaxed = N * N;


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

short getDigitsNum(long x){
	short n=1;
	while (x >= 10) {n++; x/=10;}
	return n;
}

int main(int, char**){
	
	Conway Cellular(N);
	Cellular.FieldFillRand();	
	
	SDL_Init(SDL_INIT_VIDEO); 

	if (TTF_Init() != 0){
		
		SDL_Quit();
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow("Lesson 5", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	const std::string resPath = getResourcePath("Cellular Automaton");
	
	SDL_Texture *tex = IMG_LoadTexture(renderer, "cell_green.png");
	SDL_Texture *texI = IMG_LoadTexture(renderer, "hex_102_88_black.png");

	SDL_Color color = { 255, 255, 255, 255 };
	//SDL_Texture *image = renderText("TTF fonts are cool!", "sample.ttf", color, 64, renderer);
	SDL_Texture *image = renderText(std::to_string(153), "sample.ttf", color, 64, renderer);

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

		text_dst.w = SCREEN_HEIGHT / 5;
		text_dst.h = 30;
		text_dst.x = SCREEN_HEIGHT;
		text_dst.y = 10;

		printf("dst.w = %d, dst.h = %d, ratio = %lf\n",dst.w,dst.h,ratio);
		//SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
		SDL_QueryTexture(texI, NULL, NULL, &clip.w, &clip.h);
		SDL_QueryTexture(tex, NULL, NULL, &clip.w, &clip.h);
		int iW, iH;
		SDL_QueryTexture(image, NULL, NULL, &iW, &iH);

	SDL_Rect rectangle;
	rectangle.x = 10;
	rectangle.y = 10;
	rectangle.w = SCREEN_HEIGHT / N - 1;
	rectangle.h = SCREEN_HEIGHT / N - 1;

	int alive;
	SDL_Event e;
	bool quit = false;
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
					case SDLK_1:
						Cellular.addGlider(rand() % (N-3),rand() % (N-3)); ;
						break;
					default:
						break;
				}
			}
		}

		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
		for (int i = 0; i < N; ++i)
		{
			dst.y = dst.h*i;
			rectangle.y = (rectangle.h+1) * i;
			for (int j = 0; j < N; ++j) {
				dst.x = j*dst.w;
				if (Cellular.Cell(i,j)) {
						rectangle.x = (rectangle.w+1) * j;
						SDL_RenderFillRect(renderer, &rectangle);
				}
			}
		}
		alive = Cellular.Step();

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		//Update the screen
		image = renderText(std::to_string(alive), "sample.ttf", color, 64, renderer);
		text_dst.w = text_dst.h * getDigitsNum(alive);
		printf("alive = %d, = %f\n", alive, aliveLineHeight*1.0*alive/AliveMaxed);
		SDL_RenderCopy(renderer, image, NULL, &text_dst);
		SDL_RenderPresent(renderer);
		SDL_Delay(500);
	}
	//Clean up
	cleanup(tex, renderer, window);
	cleanup(texI, renderer, window);
	IMG_Quit();
	SDL_Quit();

	return 0;
}
