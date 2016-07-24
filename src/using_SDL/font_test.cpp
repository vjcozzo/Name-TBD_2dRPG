/* Vincent Cozzo 						*/
/* A test for moving the player entity around using WASD	*/

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

/* N = Tentative max number of entities on screen at a time,	*/
/* unless of course we take a different approach, like storing 	*/
/* all entities in a linked list or arraylist instead of array.	*/
#define N 200

const int WIDTH = 710;
const int HEIGHT = 365;

void logSDLError (std::ostream &os, const std::string &message);
SDL_Texture *loadTextureFromPNG (const std::string &file, SDL_Renderer *ren);
void renderTexture (SDL_Texture *texture, SDL_Renderer *ren, int x, int y);
void renderTextureFactor (SDL_Texture *texture, SDL_Renderer *ren, int x, int y, float fact);
SDL_Texture *renderText (const std::string &msg, const std::string &fontFile, SDL_Color color, int fontSize, SDL_Renderer *ren);

int main (int argc, char** argv) {
	if (SDL_Init(SDL_INIT_VIDEO)) {
		logSDLError (std::cout, "SDL_Init");
		return 1;
	}
	if (TTF_Init()) {
		logSDLError (std::cout, "TTF_Init");
		SDL_Quit();
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

/* We will need the following arrays,
 *  when we generalize this entire .cpp document to make the engine: */
/*
	int map_matrix[WIDTH][LENGTH];
	SDL_Texture* master_entity_list[N];
*/

	const std::string path = "/home/vjcozzo/Name-TBD/res/";
	const std::string bg = "plains_fort.png";
	SDL_Color color = {255, 255, 255, 255};

	SDL_Texture *font_ex = renderText ("THIS IS A FONT TEST LOL\nPress x to exit", path + "fonts/Ubuntu-M_font.ttf", color, 34, ren);
	if (font_ex == NULL) {
		SDL_DestroyRenderer (ren);
		SDL_DestroyWindow (win);
		TTF_Quit();
		SDL_Quit();
		return 1;
	}
	SDL_Texture *bgTex = loadTextureFromPNG (path + "" + bg, ren);
	SDL_Texture *playerTex = loadTextureFromPNG (path + "player_transparent.png", ren);
	SDL_Texture *health_bar_base = loadTextureFromPNG (path + "health_base.png", ren);
	SDL_Texture *building = loadTextureFromPNG (path + "building.png", ren);
	SDL_Texture *tent = loadTextureFromPNG (path + "tent_small_blue.png", ren);
/* How would we generalize this initialization of SDL_Textures?
 *	Let's think about it... something like... the following?
	for (ind = 0; ind < num_of_default_entities; ind ++) {
		master_entity_list[ind]	= loadTextureFromPNG (path + ???);
	}
	
 * On second thought, this is happening outside the main loop, so...
 * We may just want to initialize like two or three main entities,
 * Namely the ones that exist by default on the TITLE screen.
 * Then update the entities inside the while loop ? - Vince
*/

	if ((bgTex == NULL) || (playerTex == NULL) ||
	    (building == NULL) || (health_bar_base == NULL) || (tent == NULL)) {
		SDL_DestroyRenderer (ren);
		SDL_DestroyWindow (win);
		SDL_Quit();
		return 1;
	}

	/* These are variables to store the player's initial position */
	/* Obviously they will be changed later on and will change where
	 * the player object entity is renderred. */
	int playerX = 100, playerY = 300, winstate = 0;

	/* Warning: please don't make this speed variable too large, 
		else the player might go off screen in the left direction... */
	int speed = 2;
	SDL_Event ev;
	int alive = 1/*, iteration = 0*/;
	float scale = 2.0f;

	/* Next, determine winstate */
	if (strcmp (bg.c_str(), "plains_fort.png") == 0) {
		winstate = 12;
	}
	else if (strcmp (bg.c_str(), "woods_rev2.png") == 0) {
		winstate = 13;
	}
	while (alive) {
		while (SDL_PollEvent (&ev)) {
			/* Polled an event for this iteration. */
			if (ev.type == SDL_QUIT) {
			/* Quit - by exiting the loop / closing the window. */
				alive = 0;
			}
			else if (ev.type == SDL_KEYDOWN) {
			/* Process the WASD key presses: */
				if (winstate == 12) {
					if (ev.key.keysym.sym == SDLK_d) {
						if ((playerX < 333) || (playerX > 333 && playerY > 269)) {
							playerX += speed;
						}
					} else if (ev.key.keysym.sym == SDLK_a) {
						if ((playerX > 1)) {
							playerX -= speed;
						}
					}
					if (ev.key.keysym.sym == SDLK_w) {
/* Could uncomment the following given the woods background, but the woods are still being drawn: */
/*
					if (playerY > 270) {
						playerY -= speed;
					}
*/
						if (playerY > 271) {
							playerY -= speed;
						} else if ((playerX < 373) && (playerY > 201)) {
							playerY -= speed;
						}
					} else if (ev.key.keysym.sym == SDLK_s) {
/*
					if (playerY < HEIGHT - 30) {
						playerY += speed;
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
/*				if (playerX < 0) {
					playerX += WIDTH;
				}  */
					playerX %= WIDTH;
				}
			}
		}
		
		SDL_RenderClear (ren);
		renderTexture (bgTex, ren, 0, 0);
		renderTexture (health_bar_base, ren, 5, 5);
		renderTexture (building, ren, 367, 79);
		renderTexture (tent, ren, 231, 263);
		renderTexture (font_ex, ren, 30, 30);
/* Eventually we need a general statement, probably within a for-loop, ike: */
/*		for (ind  = 0; ind < num_entities; num ++) {
			int x_comp = entity[ind]->x_pos;
			int y_comp = entity[ind]->y_pos;
			renderTexture (entity[ind], ren, x_comp, y_comp);
		}
*/
		renderTextureFactor (playerTex, ren, playerX, playerY, scale);

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

	SDL_DestroyTexture (font_ex);
	SDL_DestroyTexture (building);
	SDL_DestroyTexture (bgTex);
	SDL_DestroyTexture (playerTex);
	SDL_DestroyTexture (health_bar_base);
/* Again, here we'll need a new for-loop, something like: */
/*	for (ind = 0; ind < num_entities; ind ++) {
		SDL_Destroy_Texture (entity[ind]);
	}
*/
	SDL_DestroyRenderer (ren);
	SDL_DestroyWindow (win);
	SDL_Quit();
	return 0;
}

void logSDLError (std::ostream &os, const std::string &message) {
	os << message << " ERR: " << SDL_GetError() << std::endl;
}

SDL_Texture *loadTextureFromPNG (const std::string &file, SDL_Renderer *ren) {
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

void renderTexture (SDL_Texture *texture, SDL_Renderer *ren, int x, int y) {
	SDL_Rect destination;
	destination.x = x;
	destination.y = y;

	SDL_QueryTexture (texture, NULL, NULL, &destination.w, &destination.h);
	SDL_RenderCopy (ren, texture, NULL, &destination);
}

void renderTextureFactor (SDL_Texture *texture, SDL_Renderer *ren, int x, int y, float fact) {
	SDL_Rect destination;
	destination.x = x;
	destination.y = y;

	SDL_QueryTexture (texture, NULL, NULL, &destination.w, &destination.h);
	destination.w *= fact;
	destination.h *= fact;
	SDL_RenderCopy (ren, texture, NULL, &destination);
}

SDL_Texture *renderText (const std::string &msg, const std::string &fontFile, SDL_Color color, int fontSize, SDL_Renderer *ren) {
	TTF_Font *font = TTF_OpenFont (fontFile.c_str(), fontSize);
	if (font == NULL) {
		logSDLError (std::cout, "TTF_OpenFont");
		return NULL;
	}

	SDL_Surface *font_surface = TTF_RenderText_Blended (font, msg.c_str(), color);
	if (font_surface == NULL) {
		TTF_CloseFont (font);
		logSDLError (std::cout, "TTF_RenderText");
		return NULL;
	}
	SDL_Texture *textFromFont = SDL_CreateTextureFromSurface (ren, font_surface);
	if (textFromFont == NULL) {
		logSDLError (std::cout, "CreateTextureFromSurface");
	}

	SDL_FreeSurface (font_surface);
	TTF_CloseFont (font);
	return textFromFont;
}
