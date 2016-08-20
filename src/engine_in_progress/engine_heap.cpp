/* Vincent Cozzo                         */
/* A test for generalizing the movement capabilities,		*/
/* With the eventual goal of rendering any general background	*/
/*  engine_heap.cpp : the heap implementation			*/

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "engine_heap.h"

#define N 200
#define MAX_LEN 64
#define WIN_HEIGHT 365
#define WIN_WIDTH 710
#define DEFAULT_MAX_ENT 16
#define MAX_PRI_VAL 999

int main (int argc, char** argv) {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        logSDLError(std::cout, "SDL_Init");
        return 1;
    }
    if (TTF_Init()) {
        logSDLError (std::cout, "TTF_Init");
        SDL_Quit();
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow ("Old Crosscavern Map", 50, 50, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
    if(win == NULL) {
        logSDLError (std::cout, "SDL_CreateWindow");
        SDL_Quit();
        return 1;
    }
    
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == NULL) {
        logSDLError(std::cout, "SDL_CreateRenderer");
        SDL_DestroyWindow (win);
        SDL_Quit();
        return 1;
    }

/* We will need the following arrays to determine player
 * movement restrictions */
    int indr, indc;
    int map_matrix[WIN_HEIGHT][WIN_WIDTH];
    for(indr = 0; indr < WIN_HEIGHT; indr ++) {
        for (indc = 0; indc < WIN_WIDTH; indc ++) {
            map_matrix[indr][indc] = 0;
        }
    }

/* We will need a game state and background number, for general
 *   background loading */
    unsigned int /*game_state = 11,*/ bg_num = 100;

    const std::string path = "../../res/";
    int ind=0, path_len=path.size();
    const char *data = "bg_data.txt";
    const char *bg = getBGStringFromNum(bg_num);

/*    SDL_Texture *playerLoc = loadTextureFromFont();*/
    SDL_Texture *bgTex = loadTextureFromPNG(path + "" + bg, ren);
    SDL_Texture *playerTex = loadTextureFromPNG
                                 (path + "player_transparent.png", ren);

    int found = 0;

    const char user_name[MAX_LEN] = "player_username";
/*    user_name[0] = 'm';
    user_name[1] = '_';
    user_name[2] = 'p';
    user_name[3] = 'l';
    user_name[4] = 'a';
    user_name[5] = 'y';
    user_name[6] = 'e';
    user_name[7] = 'r';
    user_name[8] = '\0';*/
    /* Set the basic, default data for the player here. */
    /* Player will always be the first entity. */
    animation *player_frames = (animation *) malloc (sizeof(animation));
    player_frames->visual = playerTex;
    player_frames->next = NULL;

    Entity *player_data = (Entity *) malloc (sizeof(Entity));
    player_data->head_anim = player_frames;
    strcpy(player_data->id, user_name);
    player_data->x_comp = 100;
    player_data->y_comp = 300;
    player_data->num_frames = 1;
    player_data->priority = 100;

    int set_ind;
    Entity **entity_list = (Entity **) malloc (16*sizeof(Entity *));
    if (entity_list == NULL) {
        printf("Evident failure in allocating memory for the entity list.\n");
	free (player_data);
	free (player_frames);
	SDL_DestroyTexture (bgTex);
	SDL_DestroyTexture (playerTex);
        SDL_DestroyWindow (win);
        SDL_Quit();
        return 1;
    }
/*    printf("Allocated memory for entity_list, the array of pointers\n");*/
    entity_list[0] = player_data;
    for (set_ind = 1; set_ind < DEFAULT_MAX_ENT;  set_ind ++) {
        entity_list[set_ind] = NULL;
    }
    heap *master_heap = (heap *) malloc (sizeof(heap));
    if (master_heap == NULL) {
        printf("Evident failure in allocating memory for the master heap.\n");
        free (entity_list);
        free (player_data);
        free (player_frames);
        SDL_DestroyTexture (bgTex);
        SDL_DestroyTexture (playerTex);
        SDL_DestroyWindow (win);
        SDL_Quit();
        return 1;
    }
    master_heap->list = entity_list;
    master_heap->size = 1;
    master_heap->max_size = 16;
/*    printf("DEBUG msg (can ignore now): Allocated the master heap\n");*/

/* Invocation of the general solution here:
   map_matrix = getMapFromBG(bg);
 * or perhaps:
   adjustMapFromBG(bg, map_matrix);
   * or something of the sort.
 * */
    for (indr = 0; indr <= 271; indr ++) {
        for (indc = 0; indc < WIN_WIDTH; indc ++) {
            map_matrix[indr][indc] = 1;
        }
    }
    FILE *database = fopen(data, "r");
    char whole_line[4*MAX_LEN] = {'\0'};
    while ((!found) && fgets(whole_line, 4*MAX_LEN + 1, database)) {
        int initX, initY, subtraction;
        char pngName[MAX_LEN] = {'\0'};
        char excludeFirst[MAX_LEN] = {'\0'};
        indc = 0;
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
        sscanf(excludeFirst, "%d, %d", &initX, &initY);
/*        printf("Debug: just read from the line in the database\n");
        printf("%d\n", initX);
        printf("%d\n", initY);
        printf("Value of bg (background file name): %s\n", bg);
        printf("Value of pngName (background file name as read from database: %s\n", pngName);
        printf("About to compare these to each other\n");*/

        if (strcmp(bg, pngName)) {
            int t;
            for (t = 0; t < indc; t ++) {
                pngName[t] = '\0';
            }
        } else {
            char pngFileName[MAX_LEN] = {'\0'};
            ind = 0;
            int cap = strlen(whole_line);
/*            printf("Found our png file in the master database!\n");*/
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
                printf("ERR: File name is too long.\nWe've identified the file for background %s\n", pngName);
                return -1;
            }
            pngFileName[ind] = '.';
            pngFileName[ind+1] = 't';
            pngFileName[ind+2] = 'x';
            pngFileName[ind+3] = 't';
/* This texture is redundant, since we already have a background texture initialized above. However, in a general engine, this may need to be uncommented. */
/*            SDL_Texture *bg_texture = loadTextureFromPNG (path + "" + pngName, ren);    */
            FILE* specific_entities_list = fopen(pngFileName, "r");
            ind = 1;
/*            printf("Debug msg: About to start inner while loop, to read individual entity data\n");*/
            while (fgets(whole_line, 4*MAX_LEN + 1, specific_entities_list)) {
                int width, height,
                    frame_count=1, indf;
                char postLine[MAX_LEN] = {'\0'};
                char nextPngName[MAX_LEN] = {'\0'};
                char nextPngNameWithPath[MAX_LEN] = {'\0'};
                int ent_initX, ent_initY, entPri,
                    floorX, floorY;
                for (indc = 0; indc < path_len; indc ++) {
                    nextPngNameWithPath[indc] = path[indc];
                }
/*                printf("Debug msg: %s", whole_line);*/
                while ((whole_line[indc - path_len] != ',') &&
                       (whole_line[indc - path_len] != '\n')) {
                    nextPngNameWithPath[indc] = whole_line[indc - path_len];
                    nextPngName[indc - path_len] = whole_line[indc - path_len];
                    indc ++;
                }
/*                printf("Debug: Extracted the next png name.\n");*/
/*                printf("\tIt's %s!\n", nextPngName);*/
                indc -= path_len;
                indc ++;
                subtraction = indc;
/*                printf("Now the index right after the comma is %d\n", (indc-path_len));*/
                while ((whole_line[indc] != '\n')
                    && (indc < MAX_LEN)) {
                    postLine[indc-subtraction] = whole_line[indc];
                    indc ++;
                }
/*                printf("Stopped at %d becuase there is a newline character there.\n", indc);*/
                indc = 0;
                sscanf(postLine, "%d,%d,%d,%d,%d,%d",
                          &ent_initX, &ent_initY,
                          &floorX, &floorY, &frame_count, &entPri);
                SDL_Texture *next_texture 
                             = loadTextureFromPNG(nextPngNameWithPath, ren);
                Entity *nextEnt = makeEntity (next_texture, nextPngName,
			       	ent_initX, ent_initY, frame_count, entPri);
                indf = 1;
/*                printf("reading from postline=\"%s\"\n", postLine);*/
                if (frame_count > 1) {
                    while (((indf < frame_count)) &&
                        (fgets(whole_line, 4*MAX_LEN+1,
                        specific_entities_list) != NULL)) {
/*                        printf("Now processing animation frame %d < %d.\n",
 *                        indf, frame_count);*/
                        sscanf(whole_line, "%s\n", nextPngName);
                        if (nextPngName[strlen(nextPngName) - 1] == '\n') {
                            nextPngName[strlen(nextPngName) - 1] = '\0';
                        }
/*                        printf("\tpng name is %s!\n\tLine identified is %s!\n", nextPngName, whole_line);*/
                    /* NOTE: here we need to prepend
                     * the path string
                     * to the nextPngName
                     * in order for the PNG to be 
                     * identified:*/
                        for (indc = 0;
                        (indc < MAX_LEN) && (nextPngName[indc] != '\n');
                                       indc ++) {
                            nextPngNameWithPath[indc + path_len] = nextPngName[indc];
                        }
                        SDL_Texture *next_anim =
                            loadTextureFromPNG(nextPngNameWithPath, ren);
                        addAnimation(nextEnt, next_anim);

                        for (indc = 0; indc < 4*MAX_LEN; indc ++) {
                            whole_line[indc] = '\0';
                        }
                        for (indc = 0; indc < MAX_LEN; indc ++) {
                            nextPngName[indc] = '\0';
                        }
                        for (indc = path_len; indc < MAX_LEN; indc ++) {
                            nextPngNameWithPath[indc] = '\0';
                        }
                        indc ++;
                        indf ++;
                    }
                }
                SDL_QueryTexture(next_texture, NULL, NULL, &width, &height);
/*                printf ("Debug: next texture, %s, has: \n"\
            "\tbaseX %d, baseY %d;\n\twidth %d and height %d\n",
                    nextPngName,
                    floorX,
                    floorY,
                    width,
                    height);*/
                addToHeap(&master_heap, nextEnt);
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
            fclose(specific_entities_list);
            found = 1;
        }
    }
    fclose(database);

    if ((bgTex == NULL) || (playerTex == NULL)) {
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    /* Following are variables to store the player's initial position */
    /* Obviously they will be changed later on and will change where
     * the player object entity is renderred. */
    int x_coord, y_coord, playerWidth, playerHeight;
    SDL_QueryTexture(master_heap->list[0]->head_anim->visual,
		    NULL, NULL, &playerWidth, &playerHeight);
    int speed = 2;
    SDL_Event ev;
    int open = 1/*, iteration = 0*/;
/*    float scale = 2.0f;*/
    unsigned int ctr = 1;

/* ******   MAIN GAME LOOP BELOW   ****** */
    printf("About to START GAME LOOP ---------\n");
    while (open) {
        heap *h_prime = NULL;
        /* Update coordinates: */
        x_coord = (player_data->x_comp 
                    + ((/*scale* */playerWidth)/2));
        y_coord = player_data->y_comp
                    + ((/*scale* */playerHeight));
        while(SDL_PollEvent(&ev)) {
            /* Polled an event for this iteration. */
            if (ev.type == SDL_QUIT) {
            /* Quit - by exiting the loop / closing the window. */
                open = 0;
            }
            else if (ev.type == SDL_KEYDOWN) {
                /* Process the WASD key presses: */
                if (ev.key.keysym.sym == SDLK_d) {
                    if (map_matrix[y_coord][x_coord + speed] == 0) {
                        player_data->x_comp += speed;
                    }
                } else if (ev.key.keysym.sym == SDLK_a) {
                    if (map_matrix[y_coord][x_coord - speed] == 0) {
                        player_data->x_comp -= speed;
		    } else {
                        printf("map_matrix[y_coord][x_coord-speed] = map_matrix[%d][%d]=1\n", y_coord, x_coord-speed);
		    }
                } else if (ev.key.keysym.sym == SDLK_w) {
                    if (map_matrix[y_coord - speed][x_coord] == 0) {
                        player_data->y_comp -= speed;
                    }
                } else if (ev.key.keysym.sym == SDLK_s) {
                    if ((y_coord + speed < WIN_HEIGHT) && 
	                (map_matrix[y_coord + speed][x_coord] == 0)) {
                        player_data->y_comp += speed;
                    } else if (y_coord + speed >= WIN_HEIGHT) {
                        printf("%d + %d >= WIN_HEIGHT (365)\n", y_coord, speed);
		    } else {
                        printf("map_matrix[y_coord+speed][x_coord] = map_matrix[%d][%d]=1\n", y_coord+speed, x_coord);
		    }
                } else if ((ev.key.keysym.sym == SDLK_x) || (ev.key.keysym.sym == SDLK_q)) {
                /* Allow exit on X key or Q key: */
                    open = 0;
                }
                /* Allow player to loop around in the x axis direction*/
                if (player_data->x_comp < 0) {
                    player_data->x_comp += WIN_WIDTH;
                } 
                player_data->x_comp %= WIN_WIDTH;
            }
        }
        SDL_RenderClear(ren);

        /* A single hard-coded texture being renderred */
        /* This is of  course the background texture.	*/
        renderTexture(bgTex, ren, 0, 0);

        /* Now, to render all the proper textures of each entity.*/
        unsigned int ctr_adjusted = ctr % 5;
        if (ctr % 4500 == 0) {
            ctr_adjusted ++;
            ctr_adjusted %= 5;
        }
        renderHeap (master_heap, ctr_adjusted, ren, &h_prime);
/* On a rather happy note, much of the following code
 * block can be replaced - in the Heap implementation - 
 * by the renderHeap () function. */
/*        for (render_ind=0; render_ind < master_heap->size; render_ind ++) {
            int x_pos, y_pos;
            unsigned int anim_ind = 0;
            anim_ind = master_heap->list[render_ind]->num_frames;
            animation *next_frame = master_heap->list[render_ind]->head_anim;
            if (master_heap->list[render_ind]->num_frames > 1) {
                for (; anim_ind < ctr_adjusted; anim_ind ++) {
                    next_frame = next_frame->next;
                }
            }
            SDL_Texture *frame_texture = next_frame->visual;
            x_pos = master_heap->list[render_ind]->x_comp;
            y_pos = master_heap->list[render_ind]->y_comp;
            renderTexture(frame_texture, ren, x_pos, y_pos);
        }*/
        /* NOTE: Render the Player object last. */
	/*
        renderTextureFactor(master_heap->list[0]->head_anim->visual, ren, 
                master_heap->list[0]->x_comp,
                master_heap->list[0]->y_comp,
                scale);
	*/
        SDL_RenderPresent(ren);
	master_heap = h_prime;
        ctr ++;
    }
    ind = (master_heap->size - 1);

    /* Now recursively delete all nodes in the entity list */
    for (; ind > 0; ind --) {
        SDL_DestroyTexture(master_heap->list[ind]->head_anim->visual);
        free(master_heap->list[ind]->head_anim);
        free(master_heap->list[ind]);
    }

    free(master_heap->list);
    free(master_heap);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

void logSDLError(std::ostream &os, const std::string &message) {
    os << message << " ERR: " << SDL_GetError() << std::endl;
}

SDL_Texture *loadTextureFromPNG(const std::string &file, SDL_Renderer *ren) {
    const char *mod = "rb";
    SDL_Texture *texture = NULL;
    SDL_RWops *rwop = SDL_RWFromFile(file.c_str(), mod);
    SDL_Surface *loadedImage = IMG_LoadPNG_RW(rwop);
    if (loadedImage != NULL) {
        texture = SDL_CreateTextureFromSurface (ren, loadedImage);
        SDL_FreeSurface (loadedImage);
        if (texture == NULL) {
            logSDLError(std::cout, "CreateTextureFromSurface");
            return NULL;
        }
    } else {
        logSDLError(std::cout, "LoadPNG");
        return NULL;
    }
    return texture;
}

void renderTexture (SDL_Texture *texture, SDL_Renderer *ren, int x, int y) {
    SDL_Rect destination;
    destination.x = x;
    destination.y = y;

    SDL_QueryTexture(texture, NULL, NULL, &destination.w, &destination.h);
    SDL_RenderCopy(ren, texture, NULL, &destination);
}

void renderTextureFactor (SDL_Texture *texture, SDL_Renderer *ren, int x, int y, float fact) {
    SDL_Rect destination;
    destination.x = x;
    destination.y = y;

    SDL_QueryTexture(texture, NULL, NULL, &destination.w, &destination.h);
    destination.w *= fact;
    destination.h *= fact;
    SDL_RenderCopy(ren, texture, NULL, &destination);
}

const char* getBGStringFromNum (unsigned int ref_num) {
    switch (ref_num) {
        case 99: return "plains_fort.png\0"; break;
        case 100: return "woods_bg.png\0"; break;
        default: return "plains_fort.png\0"; break;
    }
    logSDLError(std::cout, "BACKGROUND REFNUM SELECTION");
    return "";
}

SDL_Texture *renderText (const std::string &msg,
    const std::string &fontFile, SDL_Color color, int size, SDL_Renderer *ren) {
    TTF_Font *font = TTF_OpenFont(fontFile.c_str(), size);
    if (font == NULL) {
        logSDLError(std::cout, "TTF_OpenFont() FontNotFound");
        return NULL;
    }

    SDL_Surface *fontSurface = TTF_RenderText_Blended(font, msg.c_str(), color);
    if (fontSurface == NULL) {
        logSDLError(std::cout, "TTF_RenderText");
        TTF_CloseFont(font);
        return NULL;
    }

    SDL_Texture *textureFont = SDL_CreateTextureFromSurface(ren, fontSurface);
    if (textureFont == NULL) {
        logSDLError(std::cout, "CreateTextureFromSurface");
    }
    SDL_FreeSurface(fontSurface);
    TTF_CloseFont(font);
    return textureFont;
}

void deleteEntity (heap *head, Entity *addr) {
    int ind;
    for (ind = (head->size - 1); ind >= 0; ind --) {
        if (head->list[ind] == addr) {
            int update_ind = ind;
            head->list[update_ind] = head->list[head->size - 1];
	    head->list[head->size - 1] = NULL;
	    head->size --;
/*            deleteAnimationFrames(head->list[ind]->head_anim,
                                  head->list[ind]->num_frames);*/
/*            SDL_DestroyTexture(head->list[ind]->head_anim->visual);*/
/*            free(head->list[ind]->head_anim);*/
/*            free(head->list[ind]);*/
            
            /* IMPORTANT: Maintain the invariant of the min heap! */
            while (update_ind < (head->size)) {
                /* Now consider the next child's children...*/
                int left_pri, right_pri, left_ind, right_ind;

                left_ind = (2*update_ind + 1);
                right_ind = (2*update_ind + 2);
                if ((left_ind) < (head->size)) {
                    left_pri = head->list[left_ind]->priority;
                } else {
                    left_pri = MAX_PRI_VAL;
                }
                if ((right_ind) < (head->size)) {
                    right_pri = head->list[right_ind]->priority;
                } else {
                    right_pri = MAX_PRI_VAL;
                }
		/* Now cover all possible cases of the values of 
		 * these priorities. ... */
		if ((left_pri <= right_pri) && 
                    (left_pri < head->list[update_ind]->priority)) {
                    /* Swap left with this current node */
                    Entity *tmp = head->list[update_ind];
                    head->list[update_ind] = head->list[left_ind];
                    head->list[left_ind] = tmp;
                    update_ind = left_ind;
                } else if ((right_pri <= left_pri) && 
                           (right_pri < head->list[update_ind]->priority)) {
                    /* Swap right with this current node */
                    Entity *tmp = head->list[update_ind];
                    head->list[update_ind] = head->list[right_ind];
                    head->list[right_ind] = tmp;
                    update_ind = right_ind;
                } else {
                    /* no-op, we're at a step where,
		     * in a mathematically provable sense,
		     * we must be at a satisfactory state,
		     * one in which the invariant has been preserved. */
                    update_ind = head->size;
		}
            }
        }
    }
}

void deleteAnimationFrames (animation *node, int iterations) {
    if (NULL == node) {
        return;
    } else if (iterations == 1) {
        node->next = NULL;
        SDL_DestroyTexture(node->visual);
        free (node);
        return;
    } else {
        deleteAnimationFrames(node->next, iterations-1);
        node->next = NULL;
        SDL_DestroyTexture(node->visual);
        free (node);
        return;
    }
}

void addAnimation (Entity *subject, SDL_Texture *tba) {
    animation *perma_head = subject->head_anim;
    animation *prev = NULL;
    animation *forward = subject->head_anim/*->next*/;

    animation *new_text = (animation *) malloc (sizeof(animation));
    new_text->visual = tba;
    new_text->next = NULL;
    while ((forward != NULL) && (forward != perma_head)) {
        prev = forward;
        forward = forward->next;
    }
    if ((forward == NULL)) {
        prev->next = new_text;
        new_text->next = prev;
    } else if (forward == perma_head) {
        if (prev == NULL) { /*  */
            perma_head->next = new_text;
            new_text->next = perma_head;
            return;
        }
        prev->next = new_text;
        new_text->next = perma_head;
    }
}

/* Returns a pointer to a new Entity object, with all
 * the necessary data fields.*/
/* This is where entities are actually allocated.*/
Entity *makeEntity (SDL_Texture *base_pic, char *label,
               	    int x, int y, int frames, int pri) {
    /* First, initialize the animation structure */
    animation *base = (animation *) malloc (sizeof(animation));
    base->visual = base_pic;
    base->next = NULL;

    /* Then initialize / allocate memory for the Entity struct itself */
    Entity *result = (Entity *) malloc (sizeof(Entity));
    result->head_anim = base;
    strcpy (result->id, label);
    result->x_comp = x;
    result->y_comp = y;
    result->num_frames = frames;
    result->priority = pri;
    return result;
}

void initHeap (heap **tbr/*, Entity *tba*/) {
    int initInd = 0;
    Entity **eList = (Entity **) malloc (16*sizeof(Entity *));
    for (; initInd < 16; initInd ++)  {
        eList[initInd] = NULL;
    }

    heap *result = (heap *) malloc (sizeof (heap));
    result->list = eList;
    result->size = 0;
    result->max_size = 16;
    *tbr = result;
}

void addToHeap (heap **h, Entity *tba) {
    if (*h ==  NULL) {
        initHeap(h/*, tba*/);
    }
    int index, parent_ind, size;
    Entity *tmp = NULL;

    size = (*h)->size;
    increment_size (*h);
    (*h)->list[size] = tba;
    index = size;
    parent_ind = ((index - 1)/ 2);
    while ((parent_ind >= 0) && (index > 0) &&
          ((*h)->list[parent_ind]->priority > (*h)->list[index]->priority)) {
        parent_ind = ((index - 1) / 2);

        /* Now swap with this entity's parent */
        tmp = (*h)->list[index];
        (*h)->list[index] = (*h)->list[parent_ind];
        (*h)->list[parent_ind] = tmp;
        index = parent_ind;
    }
    /* Invaraint is now preserved. */
    /* an return now. */
}

heap *increment_size (heap *h) {
    h->size = (h->size + 1);
    if ((h->size) == (h->max_size)) {
        Entity **rev_list = (Entity **) malloc ((2*h->max_size)*sizeof(Entity *));
	memcpy(rev_list, h->list, (h->max_size)*sizeof(Entity *));
        heap *result = (heap *) malloc (sizeof(heap));
	result->list = rev_list;
	deleteHeap (h);
	return result;
    } else {
        return h;
    }
}

void deleteHeap (heap *tbd) {
    if (tbd == NULL) {
        return;
    } else {
        int ind = 0;
        for (; ind < (tbd->size); ind ++) {
            free (tbd->list[ind]);
            tbd->list[ind] = NULL;
	}
        free(tbd->list);
	tbd->list = NULL;
	free(tbd);
    }
}

void renderHeap (heap *h, unsigned int count, SDL_Renderer *rend, heap **used) {
    /* Remember: we want to preserve the heap invariant */
    /* Also: after removing nodes from the first heap "h",
     * add them into the "used" heap, which will be used 
     * in future iterations */
    while ((h->size) > 0) {
        SDL_Texture *frame_texture;
        int x_pos, y_pos;
        unsigned int anim_ind = 1;
        Entity *nextRoot = h->list[0];

        animation *next_frame = nextRoot->head_anim;
        if (nextRoot->num_frames > 1) {
            for (; anim_ind < count; anim_ind ++) {
                next_frame = next_frame->next;
            }
        }

        frame_texture = next_frame->visual;
        x_pos = nextRoot->x_comp;
        y_pos = nextRoot->y_comp;
        renderTexture(frame_texture, rend, x_pos, y_pos);
        deleteEntity (h, nextRoot);
        addToHeap (used, nextRoot);
    }
}
