/* Vincent Cozzo 						*/
/* A test for moving the player entity around using WASD	*/

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "general_engine.h"

/* N = Tentative max number of entities on screen at a time,	*/
/* unless of course we take a different approach, like storing 	*/
/* all entities in a linked list or arraylist instead of array.	*/
#define N 200
#define MAX_LEN 64

const int WIDTH = 710;
const int HEIGHT = 365;

static void logSDLError (std::ostream &os, const std::string &message);
static SDL_Texture *loadTextureFromPNG (const std::string &file, SDL_Renderer *ren);
static void renderTexture (SDL_Texture *texture, SDL_Renderer *ren, int x, int y);
static void renderTextureFactor (SDL_Texture *texture, SDL_Renderer *ren, int x, int y, float fact);
static const char* getBGStringFromNum (unsigned int ref_num);
/*static int** getMapFromBG (std::string bg_name);	*/

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

/* We will need the following arrays,
 *  when we generalize this entire .cpp document to make the engine: */

	int ind, indc, num_entities = 3;
	int map_matrix[WIDTH][HEIGHT];
	for (ind = 0; ind < WIDTH; ind ++) {
		for (indc = 0; indc < HEIGHT; indc ++) {
			map_matrix[ind][indc] = 0;
		}
	}

	entity* master_entity_list[N] = {NULL};

/* We will need a game state and background number, for general
 *   background loading */
	unsigned int /*game_state = 11,*/ bg_num = 99;

	const std::string path = "/home/vjcozzo/Name-TBD/res/";
	const char *data = "bg_data.txt";
	const char *bg = getBGStringFromNum (bg_num);

	SDL_Texture *bgTex = loadTextureFromPNG (path + "" + bg, ren);
	SDL_Texture *playerTex = loadTextureFromPNG (path + "player_transparent.png", ren);
	SDL_Texture *health_bar_base = loadTextureFromPNG (path + "health_base.png", ren);
	SDL_Texture *building = loadTextureFromPNG (path + "building.png", ren);
	SDL_Texture *tent = loadTextureFromPNG (path + "tent_small_blue.png", ren);

	ind = 0;
	int found = 0;
/* Invocation of the general solution here:
 * map_matrix = getMapFromBG (bg);
 * */
	int indr/*, indc*/;
	for (indr = 0; indr < WIDTH; indr ++) {
		for (indc = 0; indc < 373; indc ++) {
			map_matrix[indr][indc] = 1;
		}
	}
	FILE *database = fopen (data, "r");
	char whole_line[4*MAX_LEN] = {'\0'};
	while ((!found) && fgets (whole_line, 4*MAX_LEN + 1, database)) {
		int indc = 0, initX, initY, subtraction;
		char pngName[MAX_LEN] = {'\0'};
		char excludeFirst[MAX_LEN] = {'\0'};
/*		sscanf (whole_line, "%s,%d,%d\n", pngName, &initX, &initY);*/
		while (whole_line[indc] != ',') {
			pngName[indc] = whole_line[indc];
			indc ++;
		}
		indc ++;
		subtraction = indc;
		while ((whole_line[indc] != '\n') && (indc < MAX_LEN)) {
			excludeFirst[indc - subtraction] = whole_line[indc];
			indc ++;
		}
		sscanf (excludeFirst, "%d, %d", &initX, &initY);
/*		printf ("Debug: just read from the line in the database\n");
		printf ("%d\n", initX);
		printf ("%d\n", initY);
		printf ("Value of bg (backgroudn file name): %s\n", bg);
		printf ("Value of pngName (background file name as read from database: %s\n", pngName);
		printf ("About to compare these to each other\n");
*/
		if (strcmp (bg, pngName)) {
			int t;
			for (t = 0; t < indc; t ++) {
				pngName[t] = '\0';
			}
		} else {
			char pngFileName[MAX_LEN] = {'\0'};
			ind = 0;
			int cap = strlen (whole_line);
/*			printf ("Found our png file in the master database!\n");*/
			while (ind <= cap) {
				whole_line[ind] = '\0';
				ind ++;
			}
			ind = 0;
			while (pngName[ind] != '.') {
				pngFileName[ind] = pngName[ind];
				ind ++;
			}
			pngFileName[ind] = '.';
			pngFileName[ind+1] = 't';
			pngFileName[ind+2] = 'x';
			pngFileName[ind+3] = 't';
/* This texture is redundant, since we already have a background texture initialized above. However, in a general engine, this may need to be uncommented. */
/*			SDL_Texture *bg_texture = loadTextureFromPNG (path + "" + pngName, ren);	*/
			FILE* specific_entities_list = fopen (pngFileName, "r");
			ind = 0;
			printf ("Debug msg: About to start inner while loop, to read individual entity data\n");
			while (fgets (whole_line, 4*MAX_LEN + 1, specific_entities_list)) {
				char postLine[MAX_LEN] = {'\0'};
				char nextPngName[MAX_LEN] = {'\0'};
				int ent_initX, ent_initY;
				indc = 0;
				printf ("Debug msg: %s", whole_line);
				while (whole_line[indc] != ',') {
					nextPngName[indc] = whole_line[indc];
					indc ++;
				}
/*				printf ("Debug: Extracted the next png name.\n");*/
/*				printf ("It's %s!\n", nextPngName);*/
				indc ++;
				subtraction = indc;
				while ((whole_line[indc] != '\n') && (indc < MAX_LEN)) {
					postLine[indc - subtraction] = whole_line[indc];
					indc ++;
				}
/*				indc = 0;*/
				sscanf (postLine, "%d,%d", &ent_initX, &ent_initY);
/*				printf ("ind:%d\n", ind);	*/
/*				printf ("%s\n", nextPngName);	*/
/*				printf ("Debug: about to allocate memory\n");*/
				*(master_entity_list + ind) = (entity *) malloc (sizeof (entity));
				printf ("Debug msg: The sizeof returns %lu\n", sizeof(entity));
				if (master_entity_list[ind] == NULL) {
					std::cout << "Nooo! Failure. COULD NOT ALLOCATE MEMORY" << std::endl;
					return -1;
				}
				printf ("Debug: Successfully allocated memory\n");
				SDL_Texture *next_texture = loadTextureFromPNG (path + "" + nextPngName, ren);
				printf ("Debug: made SDL Texture for this entity.\n");
			       	master_entity_list[ind]->x_comp = initX;
				master_entity_list[ind]->y_comp = initY;
				printf ("Debug: SET THE X AND Y\n");
				master_entity_list[ind]->visual = next_texture;
				printf ("Deubg: attached the texture to the dynamically allocated entity struct.\n");
				printf ("ind:%d\n%s\nx:%d\ny:%d\n", ind, nextPngName, master_entity_list[ind]->x_comp, master_entity_list[ind]->y_comp);
				ind ++;
			}
			found = 1;
		}
	}
	fclose (database);
/*	master_entity_list[0] = { playerTex, 100, 300};
	master_entity_list[1] = { building, 367, 79};
	master_entity_list[2] = { tent, 231, 263};		*/
/* How would we generalize this initialization of SDL_Textures?
 *	Let's think about it... something like... the following?  */
/*	for (ind = 0; ind < num_of_default_entities; ind ++) {
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
	int playerX = 100, playerY = 300/*, winstate = 0*/;

	/* Warning: please don't make this speed variable too large, 
		else the player might go off screen in the left direction... */
	int speed = 2;
	SDL_Event ev;
	int alive = 1/*, iteration = 0*/;
	float scale = 2.0f;

	/* Next, determine winstate */
	/*
	if (strcmp (bg, "plains_fort.png") == 0) {
		winstate = 12;
	}
	else if (strcmp (bg, "woods_rev2.png") == 0) {
		winstate = 13;
	}*/
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
					if (map_matrix[playerX + speed][playerY] == 0) {
						playerX += speed;
/* In the future...? Would it be:
 * 						master_entity_list[0]->x_commp += speed;
 * 						*/
					}
				} else if (ev.key.keysym.sym == SDLK_a) {
					if (map_matrix[playerX - speed][playerY] == 0) {
						playerX -= speed;
					}
				} else if (ev.key.keysym.sym == SDLK_w) {
					if (map_matrix[playerX][playerY - speed] == 0) {
						playerY -= speed;
					}
				} else if (ev.key.keysym.sym == SDLK_s) {
					if (map_matrix[playerX][playerY + speed] == 0) {
						playerY += speed;
					}
				} else if (ev.key.keysym.sym == SDLK_x) {
/* Allow exit on X key: */
					alive = 0;
				}
/* Allow player to loop around in the x axis direction*/
				if (playerX < 0) {
					playerX += WIDTH;
				} 
				playerX %= WIDTH;
			}
		}
		SDL_RenderClear (ren);
/*		renderTexture (bgTex, ren, 0, 0);
		renderTexture (health_bar_base, ren, 5, 5);
		renderTexture (building, ren, 367, 79);
		renderTexture (tent, ren, 231, 263);*/
/* Eventually we need a general statement, probably within a for-loop, ike: */
		for (ind = 1; ind < num_entities; ind ++) {
			int x_comp = master_entity_list[ind]->x_comp;
			int y_comp = master_entity_list[ind]->y_comp;
			renderTexture (master_entity_list[ind]->visual, ren, x_comp, y_comp);
		}

		renderTextureFactor (playerTex, ren, playerX, playerY, scale);

		SDL_RenderPresent (ren);
	}

/*	SDL_DestroyTexture (building);*/
	SDL_DestroyTexture (bgTex);
/*	SDL_DestroyTexture (playerTex);*/
	SDL_DestroyTexture (health_bar_base);
/* Again, here we'll need a new for-loop, something like: */
	for (ind = 0; ind < num_entities; ind ++) {
		SDL_DestroyTexture (master_entity_list[ind]->visual);
		free (master_entity_list[ind]);
		master_entity_list[ind] = NULL;
	}

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

static void renderTextureFactor (SDL_Texture *texture, SDL_Renderer *ren, int x, int y, float fact) {
	SDL_Rect destination;
	destination.x = x;
	destination.y = y;

	SDL_QueryTexture (texture, NULL, NULL, &destination.w, &destination.h);
	destination.w *= fact;
	destination.h *= fact;
	SDL_RenderCopy (ren, texture, NULL, &destination);
}

static const char* getBGStringFromNum (unsigned int ref_num) {
	return "plains_fort.png";
}

/* FUNCTION UNDER CONSTRUCTION ****
 * should eventually deal with any general background file name!
 *  * /
static int** getMapFromBG (std::string bg_name) {
	/ *
	 * Future/TO DO :the  more general design will be to:
	 * Look up the relevant file, return a 2d array
	 * with 1s and 0s based solely on the landscape (where there is land to walk on)
	 * but for now do a hard-coded solution:
	 * */
/*	int newMap[WIDTH][HEIGHT] = {0};
	if (strcmp(bg_name.c_str(), "plains_fort.png") == 0) {
		int indr, indc;
		for (indr = 0; indr < WIDTH; indr ++) {
			for (indc = 0; indc < 373; indc ++) {
				newMap[indr][indc] = 1;
			}
		}
	}
	return (int **) (&(*(newMap)));
}		*/
