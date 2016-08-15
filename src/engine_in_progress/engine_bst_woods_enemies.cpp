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
#include "engine_bst.h"

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

	int ind, indc/*, num_entities = 3*/;
	int map_matrix[WIN_HEIGHT][WIN_WIDTH];
	for (ind = 0; ind < WIN_HEIGHT; ind ++) {
		for (indc = 0; indc < WIN_WIDTH; indc ++) {
			map_matrix[ind][indc] = 0;
		}
	}

/* We will need a game state and background number, for general
 *   background loading */
	unsigned int /*game_state = 11,*/ bg_num = 100;

	char path[MAX_LEN] = "../../res/";
	char data[MAX_LEN] = "bg_data.txt";
	const char *bg = getBGStringFromNum (bg_num);
	int path_len = strlen (path);
/*	int data_len = strlen (data);*/
	int bg_len = strlen (bg);
	char full_bg_path[MAX_LEN] = {'\0'};
	int str_copy_ind = 0;
	for (; str_copy_ind < path_len; str_copy_ind ++) {
		full_bg_path[str_copy_ind] = path[str_copy_ind];
	}
	for (; str_copy_ind < (bg_len + path_len); str_copy_ind ++) {
		full_bg_path[str_copy_ind] = bg[str_copy_ind - path_len];
	}
	full_bg_path[str_copy_ind] = '\0';

	SDL_Texture *bgTex = loadTextureFromPNG (full_bg_path, ren);
	if (bgTex == NULL) {
		logSDLError (std::cout, "Background Texture");
		SDL_DestroyRenderer (ren);
		SDL_DestroyWindow (win);
		SDL_Quit();
		return 1;
	}
	SDL_Texture *playerTex = loadTextureFromPNG ("../../res/player_transparent.png", ren);
	if (playerTex == NULL) {
		logSDLError (std::cout, "Player Texture");
		SDL_DestroyTexture (bgTex);
		SDL_DestroyRenderer (ren);
		SDL_DestroyWindow (win);
		SDL_Quit();
		return 1;
	}
	int playerWidth, playerHeight;
	SDL_QueryTexture (playerTex, NULL, NULL, &playerWidth, &playerHeight);
	ind = 0;
	int found = 0;

	char user_name[MAX_LEN] = {'\0'};
	user_name[0] = 'm';
	user_name[1] = '_';
	user_name[2] = 'p';
	user_name[3] = 'l';
	user_name[4] = 'a';
	user_name[5] = 'y';
	user_name[6] = 'e';
	user_name[7] = 'r';
	user_name[8] = '\0';

	/* Set the basic, default data for the player here. */
	/* Player will always be the first entity. */
	Entity *player_data = (Entity *) malloc (sizeof(Entity));
	player_data->visual = playerTex;
	strcpy (player_data->id, user_name);
	for (ind = 0; ind < MAX_LEN; ind++) {
		player_data->id[ind] = '\0';
	}
	player_data->id[strlen(user_name)] = '\0';
/*	player_data->id = user_name;	*/
	player_data->x_comp = 100;
	player_data->y_comp = 300;
	node* master_entity_list = (node *) malloc (sizeof(node));
	master_entity_list->info = player_data;
	master_entity_list->left = NULL;
	master_entity_list->right = NULL;

/* Invocation of the general solution here:
   map_matrix = getMapFromBG (bg);
 * or perhaps:
   adjustMapFromBG (bg, map_matrix);
   * or something of the sort.
 * */
	FILE *database = fopen (data, "r");
/*	printf ("Got the database file that contains info about each bg\n");*/
	char whole_line[4*MAX_LEN] = {'\0'};
/*	printf ("Declared the whole_line variable, a lengthy string.\n");*/
	while ((!found) && fgets (whole_line, 4*MAX_LEN + 1, database)) {
		int initX, initY, subtraction, horizon, indr=0;
		indc = 0;
		char pngName[MAX_LEN] = {'\0'};
		char excludeFirst[MAX_LEN] = {'\0'};
/*		sscanf (whole_line, "%s,%d,%d\n", pngName, &initX, &initY);*/
		while ((indc < MAX_LEN) && (whole_line[indc] != ',')) {
			pngName[indc] = whole_line[indc];
			indc ++;
		}
		indc ++;
		pngName[indc] = '\0';
		subtraction = indc;
		while ((whole_line[indc] != '\n') && (indc < MAX_LEN)) {
			excludeFirst[indc - subtraction] = whole_line[indc];
			indc ++;
		}
		sscanf (excludeFirst, "%d,%d,%d", &initX, &initY, &horizon);
		player_data->x_comp = initX;
		player_data->y_comp = initY;
		for (indr = 0; indr <= horizon; indr ++) {
			for (indc = 0; indc < WIN_WIDTH; indc ++) {
				map_matrix[indr][indc] = 1;
			}
		}
/*		printf ("Debug: just read from the line in the database\n");*/
/*
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
				printf ("ERR: File name is too long.\nIdentified a long name for the file for background picture: %s\n", pngName);
				return -1;
			}
			pngFileName[ind] = '.';
			pngFileName[ind+1] = 't';
			pngFileName[ind+2] = 'x';
			pngFileName[ind+3] = 't';
			pngFileName[ind+4] = '\0';
/* This texture is redundant, since we already have a background texture initialized above. However, in a general engine, this may need to be uncommented. */
/*			SDL_Texture *bg_texture = loadTextureFromPNG (path + "" + pngName, ren);	*/
			FILE* specific_entities_list = fopen (pngFileName, "r");
			ind = 1;
/*			printf ("Debug msg: About to start inner while loop, to read individual entity data\n");*/
			while (fgets (whole_line, 4*MAX_LEN + 1, specific_entities_list)) {
				int width, height;
				char postLine[MAX_LEN] = {'\0'};
				char nextPngName[MAX_LEN] = {'\0'};
				char nextPngNameWithPath[MAX_LEN] = {'\0'};
				int ent_initX, ent_initY,
				    floorX, floorY;
				for (indc = 0; indc < path_len; indc ++) {
					nextPngNameWithPath[indc] = path[indc];
				}
/*				printf ("Debug msg: %s", whole_line);*/
				while (whole_line[indc - path_len] != ',') {
					nextPngNameWithPath[indc] = whole_line[indc - path_len];
					nextPngName[indc - path_len] = whole_line[indc - path_len];
					indc ++;
				}
/*				printf ("Debug: Extracted the next png name.\n");*/
/*				printf ("\tIt's %s!\n", nextPngName);*/
				indc -= path_len;
				indc ++;
				subtraction = indc;
/*				printf ("Now the index right after the comma is %d\n", (indc-path_len));*/
				while ((whole_line[indc] != '\n')
					&& (indc < MAX_LEN)) {
					postLine[indc-subtraction] = whole_line[indc];
					indc ++;
				}
/*				printf ("Stopped at %d becuase there is a newline character there.\n", indc);*/
				indc = 0;
				sscanf (postLine, "%d,%d,%d,%d", &ent_initX, &ent_initY, &floorX, &floorY);
/*				printf ("reading from postline=\"%s\"\n", postLine);*/
				SDL_Texture *next_texture = loadTextureFromPNG (nextPngNameWithPath, ren);
				addEntity (&master_entity_list, next_texture, nextPngName, ent_initX, ent_initY);
				SDL_QueryTexture (next_texture, NULL, NULL, &width, &height);
/*				printf ("Debug: next texture, %s, has: \n"\
			"\tbaseX %d, baseY %d;\n\twidth %d and height %d\n",
					nextPngName,
					floorX,
					floorY,
					width,
					height);*/
/*				updateMap (&map_matrix[0], floorX, floorY, width, height);*/
				for (indr = floorY;
				    (indr < floorY+height) && 
				    (indr < WIN_HEIGHT); indr ++) {
					for (indc = floorX; 
					    (indc < floorX+width) && 
					    (indc < WIN_WIDTH);
				  	    indc ++) {
						map_matrix[indr][indc] = 1;
					}
				}
				ind ++;
			}
			fclose (specific_entities_list);
			found = 1;
		}
	}
	fclose (database);
 
/* On second thought, this is happening outside the main loop, so...
 * We may just want to initialize two or three main entities,
 * Namely the ones that exist by default on the TITLE screen.
 * Then update the entities inside the while loop ? - Vince
*/
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
/*	int indr = 196;
	indc = 0;
	for (; indr < WIN_HEIGHT; indr ++) {
		printf ("%d: ", indr);
		for (indc = 0; indc < WIN_WIDTH; indc ++) {*/
/*			printf ("%d", map_matrix[indr][indc]);*/
/*			if (1 == map_matrix[indr][indc]) {
				printf ("\n\nCONFIRMED: 1 identified below horizon\n\n");
			}*/
/*		}*/
/*		printf ("\n");*/
/*	}*/

	/* These are variables to store the player's initial position */
	/* Obviously they will be changed later on and will change where
	 * the player object entity is renderred. */
	int x_coord, y_coord/*, winstate = 0*/;

	/* Warning: please don't make this speed variable too large, 
		else the player might go off screen in the left direction... */
	int speed = 2;
	SDL_Event ev;
	int open = 1/*, iteration = 0*/;
	float scale = 2.0f;
/*	unsigned long ctr = 0;*/

	while (open) {
		while (SDL_PollEvent (&ev)) {
			x_coord = (master_entity_list->info->x_comp 
					+ ((scale*playerWidth)/2));
			y_coord = master_entity_list->info->y_comp
					+ ((scale*playerHeight));
/* Polled an event for this iteration. */
			if (ev.type == SDL_QUIT) {
/* Quit - by exiting the loop / closing the window. */
				open = 0;
			}
			else if (ev.type == SDL_KEYDOWN) {
/* Process the WASD key presses: */
				if (ev.key.keysym.sym == SDLK_d) {
					if (map_matrix[y_coord][x_coord + speed] == 0) {
						master_entity_list->info->x_comp += speed;
						x_coord += speed;
					}
				} else if (ev.key.keysym.sym == SDLK_a) {
					if (map_matrix[y_coord][x_coord -  speed] == 0) {
						master_entity_list->info->x_comp -= speed;
						x_coord -= speed;
					}
				} else if (ev.key.keysym.sym == SDLK_w) {
					if (map_matrix[y_coord - speed][x_coord] == 0) {
						master_entity_list->info->y_comp -= speed;
						y_coord -= speed;
					}
				} else if (ev.key.keysym.sym == SDLK_s) {
					if ((y_coord + speed < WIN_HEIGHT)
						&& map_matrix[y_coord + speed][x_coord] == 0) {
						master_entity_list->info->y_comp += speed;
						y_coord += speed;
					}
				} else if ((ev.key.keysym.sym == SDLK_x) 
					|| (ev.key.keysym.sym == SDLK_q)) {
/* Allow exit on X key or Q Key: */
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

		renderAllTextures (master_entity_list->left, ren);
		renderAllTextures (master_entity_list->right, ren);
		/* NOTE: Render the Player object last. */
		renderTextureFactor (master_entity_list->info->visual,
			       	ren, 
				master_entity_list->info->x_comp,
				master_entity_list->info->y_comp,
				scale);
		SDL_RenderPresent (ren);
/*		ctr += 1;*/
	}

	deleteRecursive (master_entity_list);
	SDL_DestroyTexture (bgTex);
/*	SDL_DestroyTexture (playerTex);*/
/*	SDL_DestroyTexture (master_entity_list->info->visual);*/
/*	master_entity_list->info->visual = NULL;
	free (master_entity_list->info);
	master_entity_list->info = NULL;
	free (master_entity_list);*/
	master_entity_list = NULL;

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
	switch (ref_num) {
		case 99: return "plains_fort.png\0"; break;
		case 100: return "woods_bg.png\0"; break;
		default: return "plains_fort.png\0"; break;
	}
	logSDLError (std::cout, "BACKGROUND REFNUM SELECTION");
	return "";
}

SDL_Texture *renderText (const std::string &msg, const std::string &fontFile, SDL_Color color, int size, SDL_Renderer *ren) {
	TTF_Font *font = TTF_OpenFont (fontFile.c_str(), size);
	if (font == NULL) {
		logSDLError (std::cout, "TTF_OpenFont() FontNotFound");
		return NULL;
	}

	SDL_Surface *fontSurface = TTF_RenderText_Blended(font, msg.c_str(), color);
	if (fontSurface == NULL) {
		logSDLError(std::cout, "TTF_RenderText");
		TTF_CloseFont (font);
		return NULL;
	}

	SDL_Texture *textureFont = SDL_CreateTextureFromSurface(ren, fontSurface);
	if (textureFont == NULL) {
		logSDLError(std::cout, "CreateTextureFromSurface");
	}
	SDL_FreeSurface (fontSurface);
	TTF_CloseFont (font);
	return textureFont;
}

void deleteEntity (node *head, Entity *addr) {
	if (addr == NULL) {
		return;
	}
	int lastStep = 0;
	node *aheadNode = head;
	node *tmp = NULL;

	while ((aheadNode != NULL) &&
		(aheadNode->info != addr)
	       /*(strcmp(aheadNode->info->id, addr->id))*/) {
		int compare_result = strcmp (aheadNode->info->id,
						addr->id);
		if (compare_result >= 0) {
			tmp = aheadNode;
			aheadNode = aheadNode->left;
			lastStep = 1;
		} else /*if (compare_result < 0)*/ {
			tmp = aheadNode;
			aheadNode = aheadNode->right;
			lastStep = -1;
		}
	}
	if (tmp != NULL) {
		if (lastStep >= 0) {
			tmp->left = NULL;
		} else {/*if (lastStep < 0) {*/
			tmp->right = NULL;
		}
	}
	/* For each of THIS deleted Node's children,*/
	/* add them to the rest of the tree. */
	if (aheadNode->left != NULL) {
		addAllNodes(&head, aheadNode->left);
	}
	if (aheadNode->right != NULL) {
		addAllNodes(&head, aheadNode->right);
	}
	if (aheadNode != NULL) {
		SDL_DestroyTexture (aheadNode->info->visual);
		aheadNode->info->visual = NULL;
		free (aheadNode->info);
		aheadNode->info = NULL;
		free (aheadNode);
		aheadNode = NULL;
	}
}

void addEntity (node** eList, SDL_Texture *texture, char *identifier, int x_pos, int y_pos) {
	int lastStep = 0;
	node *aheadNode = *eList;
	node *tmp = NULL;

	if ((texture == NULL) || (identifier == NULL)) {
		printf("Error: have tried to add an entity to the BST, where:\n");
		if ((texture == NULL)) {
			printf("\tThe texture given is NULL. This is a weird error.\n");
		}
		else {
			printf("\tThe identifier is NULL. Check where you call this addEntity() fct,\n\t and then verify the Entity name.\n");
		}
		return;
	}
	while (aheadNode != NULL) {
		if (strcmp(aheadNode->info->id, identifier) < 0) {
			tmp = aheadNode;
			aheadNode = aheadNode->right;
			lastStep = -1;
		} else {
			tmp = aheadNode;
			aheadNode = aheadNode->left;
			lastStep = 1;
		}
	}

	if (tmp == NULL) {
	    /* In the case that *eList (the head node location) is NULL */
		Entity *head = (Entity *) malloc (sizeof(Entity));
		head->visual = texture;
		strcpy (head->id, identifier);
		head->id[strlen(identifier)] = '\0';
		head->x_comp = x_pos;
		head->y_comp = y_pos;
		
		node *newNode = (node *) malloc (sizeof(node));
		newNode->info = head;
		newNode->left = NULL;
		newNode->right = NULL;
		*eList = newNode;
		return;
	}

	Entity *newEnt = (Entity *) malloc (sizeof(Entity));
	newEnt->visual = texture;
	strcpy (newEnt->id, identifier);
	newEnt->id[strlen(identifier)] = '\0';
	newEnt->x_comp = x_pos;
	newEnt->y_comp = y_pos;

	node *newNode = (node *) malloc (sizeof(node));
	newNode->info = newEnt;
	newNode->left = NULL;
	newNode->right = NULL;

	if (lastStep >= 0) {
		tmp->left = newNode;
	} else {
		tmp->right = newNode;
	}
}

/* Note: this addNode() function does no  additional
 * dynamic memory allocation.
 * This is a main distinction between it and the createEntity() funct.*/
void addNode (node** eList, node *toBeAdded) {
	int lastStep = 0;
	node *aheadNode = *eList;
	node *tmp = NULL;

	while (aheadNode != NULL) {
		int compare_result = strcmp (aheadNode->info->id, toBeAdded->info->id);
		if (compare_result < 0) {
			tmp = aheadNode;
			aheadNode = aheadNode->right;
			lastStep = -1;
		} else /*if (compare_result > 0) */ {
			tmp = aheadNode;
			aheadNode = aheadNode->left;
			lastStep = 1;
		}
	}

	if (tmp == NULL) {
	/* In the case that *eList (the head node location) is NULL */
		*eList = toBeAdded;
		return;
	}

	if (lastStep > 0) {
		tmp->left = toBeAdded;

	} else /*if (lastStep <= 0)*/ {
		tmp->right = toBeAdded;

	}
}


void addAllNodes (node **destination, node *source) {
	if (destination == NULL) {
		printf ("The source pointer is NULL. Error in addAllNodes()\n");
		return;
	}
	if ((*destination) == NULL) {
		*destination = source;
		return;
	} else {
		addAllNodes (destination, source->left);
		addAllNodes (destination, source->right);
		addNode (destination, source);
	}
}

void deleteRecursive (node *top) {
	if (top == NULL) {
		return;
	} else {
		if (top->left != NULL) {
			deleteRecursive (top->left);
			deleteEntity (top, top->left->info);
		}
		if (top->right != NULL) {
			deleteRecursive (top->right);
			deleteEntity (top, top->right->info);
		}
	}
/*	if ((top->left == NULL) && (top->right == NULL)) {
		return;
	} else {
		deleteRecursive (top->left);
		deleteRecursive (top->right);
		deleteEntity (top->left, top->info);
		deleteEntity (top->right, top->info);
		deleteEntity (top, top->info);
	}*/
}

void renderAllTextures (node* tree_head, SDL_Renderer *ren) {
	if (tree_head == NULL) {
		return;
	} else {
		int x_pos, y_pos;
		
		renderAllTextures (tree_head->left, ren);
		renderAllTextures (tree_head->right, ren);

		SDL_Texture *nextText = tree_head->info->visual;
		x_pos = tree_head->info->x_comp;
		y_pos = tree_head->info->y_comp;
		renderTexture (nextText, ren, x_pos, y_pos);
	}
}
