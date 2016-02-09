#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "res_path.h"
#include "cleanup.h"

/*
 * Lesson 5: Clipping Sprite Sheets
 */
//Screen attributes
const int SCREEN_WIDTH  = 923;
const int SCREEN_HEIGHT = 860;

const int CLIP_W = 88;
const int CLIP_H = 102;
const int N = 30;
/*
 * Log an SDL error with some error message to the output stream of our choice
 * @param os The output stream to write the message too
 * @param msg The error message to write, format will be msg error: SDL_GetError()
 */
void logSDLError(std::ostream &os, const std::string &msg){
	os << msg << " error: " << SDL_GetError() << std::endl;
}
/*
 * Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
 * the texture's width and height and taking a clip of the texture if desired
 * If a clip is passed, the clip's width and height will be used instead of the texture's
 * @param tex The source texture we want to draw
 * @param rend The renderer we want to draw too
 * @param x The x coordinate to draw too
 * @param y The y coordinate to draw too
 * @param clip The sub-section of the texture to draw (clipping rect)
 *		default of nullptr draws the entire texture
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip = nullptr){
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	if (clip != nullptr){
		dst.w = clip->w;
		dst.h = clip->h;
	}
	else {
		SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	}
	//renderTexture(tex, ren, dst, clip);
	SDL_RenderCopy(ren, tex, clip, &dst);
}

int main(int, char**){
	srand (time(NULL));
	unsigned short Field[N][N];
	for (int i = 0; i < N; ++i) {

		if (i%2) printf("\n");
		else printf("\n ");
		for (int j = 0; j < N; ++j)
		{
			Field[i][j]= rand() % 2;
			printf("%u ",Field[i][j]);
		}
	}

	//Start up SDL and make sure it went ok
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		logSDLError(std::cout, "SDL_Init");
		return 1;
	}

	//Setup our window and renderer
	SDL_Window *window = SDL_CreateWindow("Lesson 5", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	const std::string resPath = getResourcePath("Lesson5");
	SDL_Texture *tex = IMG_LoadTexture(renderer, "hex_102_88_green.png");
	SDL_Texture *texI = IMG_LoadTexture(renderer, "hex_102_88_black.png");


		double ratio = ((double)SCREEN_WIDTH / N) / CLIP_W;
		SDL_Rect dst,clip;
		clip.x = 0;
		clip.y = 0;
		clip.w = CLIP_W;
		clip.h = CLIP_H;
		dst.x = 400;
		dst.y = 400;
		//dst.w = CLIP_W/2;
		dst.w =  CLIP_W * ratio;
		dst.h = CLIP_H * ratio;
		printf("dst.w = %d, dst.h = %d, ratio = %lf\n",dst.w,dst.h,ratio);
		//SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
		SDL_QueryTexture(texI, NULL, NULL, &clip.w, &clip.h);
		SDL_QueryTexture(tex, NULL, NULL, &clip.w, &clip.h);


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
					default:
						break;
				}
			}
		}
		//Rendering
		SDL_RenderClear(renderer);
		//Draw the image
		//renderTexture(image, renderer, x, y, &clips[useClip]);
		


		//SDL_RenderCopy(renderer, tex, &clip, &dst);


		for (int i = 0; i < N; ++i)
		{
			dst.y = (dst.h*0.7)*i;
			for (int j = 0; j < N; ++j) {
				if (i%2) dst.x = j*dst.w;
				else dst.x = dst.w/2 + j*dst.w;
				if (Field[i][j]) SDL_RenderCopy(renderer, tex, &clip, &dst);
				//else SDL_RenderCopy(renderer, texI, &clip, &dst);
			}
		}
/*		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		SDL_Rect rectangle;

		rectangle.x = 100;
		rectangle.y = 100;
		rectangle.w = 250;
		rectangle.h = 450;
		SDL_RenderFillRect(renderer, &rectangle);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);*/
		//Update the screen
		SDL_RenderPresent(renderer);
	}
	//Clean up
	cleanup(tex, renderer, window);
	cleanup(texI, renderer, window);
	IMG_Quit();
	SDL_Quit();

	return 0;
}
