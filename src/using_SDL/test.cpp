#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

/* Just a simple test program using SDL 2.0 */

int main (int argc, char** argv) {
	if (SDL_Init(SDL_INIT_VIDEO)) {
		std::cout << "SDL_Init Error: " <<  SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Window *win = SDL_CreateWindow ("Old Crosscavern Map", 50, 50, 717, 423, SDL_WINDOW_SHOWN);
	if (win == NULL) {
		std::cout << "SDL_CreateWindow ERR: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	
	SDL_Renderer *ren = SDL_CreateRenderer (win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == NULL) {
		SDL_DestroyWindow (win);
		std::cout << "SDL_CreateRenderer ERR: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	const char *imagePath = "/home/vjcozzo/Name-TBD/game_art/RPG_OLD_map_design.png";
	const char *s = "rb";
	SDL_RWops *map_rwop = /*SDL_LoadPNG (imagePath.c_str());*/SDL_RWFromFile(imagePath, s);
	SDL_Surface *bitmap = IMG_LoadPNG_RW (map_rwop);
	if (bitmap == NULL) {
		SDL_DestroyRenderer (ren);
		SDL_DestroyWindow (win);
		std::cout <<  "SDL_LoadBMP ERR: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	SDL_Texture *tex = SDL_CreateTextureFromSurface (ren, bitmap);
	SDL_FreeSurface (bitmap);
	if (tex == NULL) {
		SDL_DestroyRenderer (ren);
		SDL_DestroyWindow (win);
		std::cout << "SDL_CreateTextureFromSurface ERR: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	for (int iteration = 0; iteration < 3; iteration ++) {
		SDL_RenderClear (ren);
		SDL_RenderCopy (ren, tex, NULL, NULL);
		SDL_RenderPresent (ren);
		if (iteration == 0) {
			SDL_Delay (3500);
		} else if (iteration == 1) {
			SDL_Delay (500);
		} else {
			SDL_Delay (500);
		}
	}

	SDL_DestroyTexture (tex);
	SDL_DestroyRenderer (ren);
	SDL_DestroyWindow (win);
	SDL_Quit();
	return 0;
}
