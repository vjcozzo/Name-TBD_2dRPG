/* Vincent Cozzo 						*/
/* A test for moving the player entity around using WASD	*/

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

const int WIDTH = 710;
const int HEIGHT = 365;

static void logSDLError (std::ostream &os, const std::string &message);
static SDL_Texture *loadTextureFromPNG (const std::string &file, SDL_Renderer *ren);
static void renderTexture (SDL_Texture *texture, SDL_Renderer *ren, int x, int y);
static void renderTextureDouble (SDL_Texture *texture, SDL_Renderer *ren, int x, int y);

int main (int argc, char** argv) {
	if (SDL_Init(SDL_INIT_VIDEO)) {
		logSDLError (std::cout, "SDL_Init");
		return 1;
	}

	SDL_Window *win = SDL_CreateWindow ("Old Crosscavern Map", 50, 50, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if (win == NULL) {
		logSDLError (std::cout, "SDL_CreateWindow");
		SDL_Quit();
		return 1;
	}
	
	SDL_Renderer *ren = SDL_CreateRenderer (win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == NULL) {
		logSDLError (std::cout, "SDL_CreateRenderer");
		SDL_DestroyWindow (win);
		SDL_Quit();
		return 1;
	}

	const std::string path = "/home/vjcozzo/Name-TBD/game_art/";

	SDL_Texture *bgTex = loadTextureFromPNG (path + "plains_fort.png"/*"woods.png*/, ren);
	SDL_Texture *playerTex = loadTextureFromPNG (path + "player.png", ren);

	if ((bgTex == NULL) || (playerTex == NULL)) {
		SDL_DestroyRenderer (ren);
		SDL_DestroyWindow (win);
		SDL_Quit();
		return 1;
	}

	/* These are variables to store the player's initial position */
	/* Obviously they will be changed later on and will change where
	 * the player object entity is renderred. */
	int playerX = 100, playerY = 300;
	SDL_Event ev;
	int alive = 1/*, iteration = 0*/;
	while (alive) {
		while (SDL_PollEvent (&ev)) {
			/* Polled an event for this iteration. */
			if (ev.type == SDL_QUIT) {
			/* Quit - by exiting the loop / closing the window. */
				alive = 0;
			}
			else if (ev.type == SDL_KEYDOWN) {
			/* Process the WASD key presses: */
				if (ev.key.keysym.sym == SDLK_d) {
					playerX += 2;
				} else if (ev.key.keysym.sym == SDLK_a) {
					playerX -= 2;
				}
				if (ev.key.keysym.sym == SDLK_w) {
				/* Could uncomment the following given the woods background, but the woods are still being drawn: */
/*
					if (playerY > 270) {
						playerY -= 2;
					}
*/
					if (playerY > 285) {
						playerY -= 2;
					}
				} else if (ev.key.keysym.sym == SDLK_s) {
/*
					if (playerY < HEIGHT - 30) {
						playerY += 2;
					}
*/
					if (playerY < 305) {
						playerY += 2;
					}
				} else if (ev.key.keysym.sym == SDLK_x) {
			/* Allow exit on X key: */
					alive = 0;
				}
			/* Allow player to loop around in the x axis direction*/
				playerX %= WIDTH;
			}

		}
		SDL_RenderClear (ren);
		renderTexture (bgTex, ren, 0, 0);
		renderTextureDouble (playerTex, ren, playerX, playerY);

		SDL_RenderPresent (ren);
	/* EDIT: No longer want to pause for so long in each iteration!
	 * We want the game to update quickly in response to key presses.
		if (iteration == 0) {
			SDL_Delay (7000);
		} else {
			alive = 0;
		}
		iteration ++;
	*/
	}

	SDL_DestroyTexture (bgTex);
	SDL_DestroyTexture (playerTex);
	SDL_DestroyRenderer (ren);
	SDL_DestroyWindow (win);
	SDL_Quit();
	return 0;
}

static void logSDLError (std::ostream &os, const std::string &message) {
	os << message << " ERR: " << SDL_GetError() << std::endl;
}

static SDL_Texture *loadTextureFromPNG (const std::string &file, SDL_Renderer *ren) {
	const char *mod = "rb";
	SDL_Texture *texture = NULL;
	SDL_RWops *rwop = SDL_RWFromFile (file.c_str(), mod);
	SDL_Surface *loadedImage = IMG_LoadPNG_RW (rwop);
	if (loadedImage != NULL) {
		texture = SDL_CreateTextureFromSurface (ren, loadedImage);
		SDL_FreeSurface (loadedImage);
		if (texture == NULL) {
			logSDLError (std::cout, "CreateTextureFromSurface");
			return NULL;
		}
	} else {
		logSDLError (std::cout, "LoadPNG");
		return NULL;
	}
	return texture;
}

static void renderTexture (SDL_Texture *texture, SDL_Renderer *ren, int x, int y) {
	SDL_Rect destination;
	destination.x = x;
	destination.y = y;

	SDL_QueryTexture (texture, NULL, NULL, &destination.w, &destination.h);
	SDL_RenderCopy (ren, texture, NULL, &destination);
}

static void renderTextureDouble (SDL_Texture *texture, SDL_Renderer *ren, int x, int y) {
	SDL_Rect destination;
	destination.x = x;
	destination.y = y;

	SDL_QueryTexture (texture, NULL, NULL, &destination.w, &destination.h);
	destination.w *= 2;
	destination.h *= 2;
	SDL_RenderCopy (ren, texture, NULL, &destination);
}
