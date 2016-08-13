/* Vincent Cozzo 						*/
/* A test for generalizing the movement capabilities, 		*/
/* With the eventual goal of rendering any general background	*/

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "engine_linkedList.h"

/* N = Tentative max number of entities on screen at a time,	*/
/* unless of course we take a different approach, like storing 	*/
/* all entities in a linked list or arraylist instead of array.	*/
#define N 200
#define MAX_LEN 64
#define WIN_HEIGHT 365
#define WIN_WIDTH 710

/*static int** getMapFromBG (std::string bg_name);	*/

/* NOTE: these external contant values were abandoned
 * because, defined this way, they cannot define the size of an array;
 * thus, the map_matrix could not be defined using them	*/
/*extern const int HEIGHT, WIDTH;			*/

int main (int argc, char** argv) {
/*	printf ("%lu bytes for an entity; %lu bytes for node\n", sizeof (Entity), sizeof (node));*/
	if (SDL_Init(SDL_INIT_VIDEO)) {
		logSDLError (std::cout, "SDL_Init");
		return 1;
	}
	if (TTF_Init()) {
		logSDLError (std::cout, "TTF_Init");
		SDL_Quit();
		return 1;
	}

	SDL_Window *win = SDL_CreateWindow ("Old Crosscavern Map", 50, 50, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
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

	int ind, indc/*, num_entities = 3*/;
	int map_matrix[WIN_HEIGHT][WIN_WIDTH];
	for (ind = 0; ind < WIN_HEIGHT; ind ++) {
		for (indc = 0; indc < WIN_WIDTH; indc ++) {
			map_matrix[ind][indc] = 0;
		}
	}

	/* New idea: if the number of entities in the master entity list */
	/* is changeable (and will change really frequently), then --	*/
	/* why not make it a linked list of Entity structures, rather 	*/
	/* than an arrray ? 						*/
/*	Entity* master_entity_list[N] = {NULL};*/

	/* Note: As a tentative beginning plan,
	 * this master entity list will always begin pointing to the palyer Entity. */

/*	node* head_entity = NULL;*/
/*	node** memory_location_of_head = &head_entity;*/

/* We will need a game state and background number, for general
 *   background loading */
	unsigned int /*game_state = 11,*/ bg_num = 99;

	const std::string path = "../../res/";
	const char *data = "bg_data.txt";
	const char *bg = getBGStringFromNum (bg_num);

/*	SDL_Texture *playerLoc = loadTextureFromFont ();*/
	SDL_Texture *bgTex = loadTextureFromPNG (path + "" + bg, ren);
	SDL_Texture *playerTex = loadTextureFromPNG (path + "player_transparent.png", ren);


	ind = 0;
	int found = 0;

	/* Set the basic, default data for the player here. */
	/* Player will always be the first entity. */
	Entity *player_data  = (Entity *) malloc (sizeof (Entity));
	player_data->visual = playerTex;
	player_data->x_comp = 100;
	player_data->y_comp = 300;
	node* master_entity_list = (node *) malloc (sizeof (node));
	master_entity_list->info = player_data;
	master_entity_list->next = NULL;

/* Invocation of the general solution here:
   map_matrix = getMapFromBG (bg);
 * or perhaps:
   adjustMapFromBG (bg, map_matrix);
   * or something of the sort.
 * */
	int indr/*, indc*/;
	for (indr = 0; indr <= 271; indr ++) {
		for (indc = 0; indc < WIN_WIDTH; indc ++) {
			map_matrix[indr][indc] = 1;
		}
	}
	FILE *database = fopen (data, "r");
	char whole_line[4*MAX_LEN] = {'\0'};
	while ((!found) && fgets (whole_line, 4*MAX_LEN + 1, database)) {
		int initX, initY, subtraction;
		indc = 0;
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
		printf ("Value of bg (background file name): %s\n", bg);
		printf ("Value of pngName (background file name as read from database: %s\n", pngName);
		printf ("About to compare these to each other\n");*/

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
			if (ind+3 >= MAX_LEN) {
				printf ("ERR: File name is too long.\nWe've identified the file for background %s\n", pngName);
				return -1;
			}
			pngFileName[ind] = '.';
			pngFileName[ind+1] = 't';
			pngFileName[ind+2] = 'x';
			pngFileName[ind+3] = 't';
/* This texture is redundant, since we already have a background texture initialized above. However, in a general engine, this may need to be uncommented. */
/*			SDL_Texture *bg_texture = loadTextureFromPNG (path + "" + pngName, ren);	*/
			FILE* specific_entities_list = fopen (pngFileName, "r");
			ind = 1;
/*			printf ("Debug msg: About to start inner while loop, to read individual entity data\n");*/
			while (fgets (whole_line, 4*MAX_LEN + 1, specific_entities_list)) {
				char postLine[MAX_LEN] = {'\0'};
				char nextPngName[MAX_LEN] = {'\0'};
				int ent_initX, ent_initY;
				indc = 0;
/*				printf ("Debug msg: %s", whole_line);*/
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
/*				*(master_entity_list + ind) = (entity *) malloc (sizeof (entity));
				if (master_entity_list[ind] == NULL) {
					std::cout << "Nooo! Failure. COULD NOT ALLOCATE MEMORY" << std::endl;
					return -1;
				}*/
/*				printf ("Debug: Successfully allocated memory\n");*/
				SDL_Texture *next_texture = loadTextureFromPNG (path + "" + nextPngName, ren);
				addEntity (&master_entity_list, next_texture, nextPngName, ent_initX, ent_initY);
/*				printf ("Debug: made SDL Texture for this entity.\n");*/
/*			       	master_entity_list[ind]->x_comp = ent_initX;
				master_entity_list[ind]->y_comp = ent_initY;
				printf ("Debug: SET THE X AND Y\n");
				master_entity_list[ind]->visual = next_texture;*/
				ind ++;
			}
			found = 1;
		}
	}
	fclose (database);
/* SO -- the array master_entity_list should now look like:
	master_entity_list[0] = { playerTex, 100, 300};
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

	if ((bgTex == NULL) || (playerTex == NULL)) {
		SDL_DestroyRenderer (ren);
		SDL_DestroyWindow (win);
		SDL_Quit();
		return 1;
	}

	/*NOTE: Now may be a good time to work on setting the map_matrix
	 * in a more accurate manner, based on the Component Entities.
	 * This is the next step: running through each Entity currently in existence,
	 * i.e. iterating through the linked list master_entity_list,
	 * and updating values of map_matrix[][] based upon the location of
	 * each entity.
	 *
	 * I plan to add this  soon, probably next week.
	 * -VC
	 * */

	/* These are variables to store the player's initial position */
	/* Obviously they will be changed later on and will change where
	 * the player object entity is renderred. */
	int playerX = 100, playerY = 300/*, winstate = 0*/;

	/* Warning: please don't make this speed variable too large, 
		else the player might go off screen in the left direction... */
	int speed = 2;
	SDL_Event ev;
	int open = 1/*, iteration = 0*/;
	float scale = 2.0f;

	while (open) {
		while (SDL_PollEvent (&ev)) {
			playerX = master_entity_list->info->x_comp;
			playerY = master_entity_list->info->y_comp;
/* Polled an event for this iteration. */
			if (ev.type == SDL_QUIT) {
/* Quit - by exiting the loop / closing the window. */
				open = 0;
			}
			else if (ev.type == SDL_KEYDOWN) {
/* Process the WASD key presses: */
				if (ev.key.keysym.sym == SDLK_d) {
					if (map_matrix[playerY][playerX + speed] == 0) {
						master_entity_list->info->x_comp += speed;
/* In the future...? Would it be:
 * 						master_entity_list[0]->x_commp += speed;
 * 						*/
					}
				} else if (ev.key.keysym.sym == SDLK_a) {
					if (map_matrix[playerY][playerX -  speed] == 0) {
						master_entity_list->info->x_comp -= speed;
					}
				} else if (ev.key.keysym.sym == SDLK_w) {
					if (map_matrix[playerY - speed][playerX] == 0) {
						master_entity_list->info->y_comp -= speed;
					}
				} else if (ev.key.keysym.sym == SDLK_s) {
					if (map_matrix[playerY + speed][playerX] == 0) {
						master_entity_list->info->y_comp += speed;
					}
				} else if ((ev.key.keysym.sym == SDLK_x) || (ev.key.keysym.sym == SDLK_q)) {
/* Allow exit on X key: */
					open = 0;
				}
/* Allow player to loop around in the x axis direction*/
				if (master_entity_list->info->x_comp < 0) {
					master_entity_list->info->x_comp += WIN_WIDTH;
				} 
				master_entity_list->info->x_comp %= WIN_WIDTH;
			}
		}
		SDL_RenderClear (ren);

		/* A couple of hard-coded textures being renderred */
		renderTexture (bgTex, ren, 0, 0);

		node *runner = master_entity_list->next;
		while (runner != NULL) {
			int x_pos, y_pos;
			SDL_Texture *nextText = runner->info->visual;
			x_pos = runner->info->x_comp;
			y_pos = runner->info->y_comp;
			renderTexture (nextText, ren, x_pos, y_pos);
			runner = runner->next;
		}
/* NOTE: Render the Player object last. */
		renderTextureFactor (master_entity_list->info->visual, ren, 
				master_entity_list->info->x_comp,
				master_entity_list->info->y_comp,
				scale);
		SDL_RenderPresent (ren);
	}

	deleteRecursive (master_entity_list);

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

const char* getBGStringFromNum (unsigned int ref_num) {
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

SDL_Texture *renderText (const std::string &msg, const std::string &fontFile, SDL_Color color, int size, SDL_Renderer *ren) {
	TTF_Font *font = TTF_OpenFont (fontFile.c_str(), size);
	if (font == NULL) {
		logSDLError (std::cout, "TTF_OpenFont() FontNotFound");
		return NULL;
	}

	SDL_Surface *fontSurface = TTF_RenderText_Blended (font, msg.c_str(), color);
	if (fontSurface == NULL) {
		logSDLError (std::cout, "TTF_RenderText");
		TTF_CloseFont (font);
		return NULL;
	}

	SDL_Texture *textureFont = SDL_CreateTextureFromSurface(ren, fontSurface);
	if (textureFont == NULL) {
		logSDLError (std::cout, "CreateTextureFromSurface");
	}
	SDL_FreeSurface (fontSurface);
	TTF_CloseFont (font);
	return textureFont;
}

void deleteEntity (node *head, Entity *addr) {
	node *aheadNode = head;
	node *tmp = NULL;

	while ((aheadNode != NULL) &&
	       (strcmp(aheadNode->info->id, addr->id))) {
		tmp = aheadNode;
		aheadNode = aheadNode->next;
	}

	if ((aheadNode == NULL) || (tmp == NULL)) {
		return;
	}
	tmp->next = aheadNode->next;
	SDL_DestroyTexture (aheadNode->info->visual);
	free (aheadNode->info);
	free (aheadNode);
}

void addEntity (node** eList, SDL_Texture *texture, char *identifier, int x_pos, int y_pos) {
	node *aheadNode = *eList;
	node *tmp = NULL;

	while (aheadNode != NULL) {
		tmp = aheadNode;
		aheadNode = aheadNode->next;
	}

	if (tmp == NULL) { /* In the case that *eList (the head node location) is NULL */
		Entity *head = (Entity *) malloc (sizeof (Entity));
		head->visual = texture;
		strcpy (head->id, identifier);
/*		head->id = identifier;*/
		head->x_comp = x_pos;
		head->y_comp = y_pos;
		
		node *newNode = (node *) malloc (sizeof (node));
		newNode->info = head;
		newNode->next = NULL;
		*eList = newNode;
		return;
	}

	Entity *newEnt = (Entity *) malloc (sizeof (Entity));
	newEnt->visual = texture;
	newEnt->id = identifier;
	newEnt->x_comp = x_pos;
	newEnt->y_comp = y_pos;

	node *newNode = (node *) malloc (sizeof(node));
	newNode->info = newEnt;
	newNode->next = NULL;

	tmp->next = newNode;
}

void deleteRecursive (node *top) {
	if (top == NULL) {
		return;
	} else {
		deleteRecursive (top->next);
		deleteEntity (top, top->info);
	}
}
