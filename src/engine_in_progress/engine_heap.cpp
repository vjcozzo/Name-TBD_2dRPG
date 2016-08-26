/* vincent cozzo                         */
/* A test for generalizing the movement capabilities,                */
/* With the eventual goal of rendering any general background        */
/*  engine_heap.cpp : the heap implementation                        */

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "engine_heap.hpp"

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
    int flag = IMG_INIT_PNG;
    if (IMG_Init(IMG_INIT_PNG)!=flag) {
        logSDLError(std::cout, "SDL_image IMG_Init");
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
    unsigned int game_state = 20, bg_num = 100;

    const std::string path = "../../res/";
    int ind=0;

    /* A texture for font-printing may end up useful.
     * The bgTex, howeverm is now obsolete. Hence, 
     * it will be commented or removed later.*/
/*    SDL_Texture *playerHP = loadTextureFromFont();*/
    SDL_Texture *bgTex = NULL;
    SDL_Texture *playerTex = loadTextureFromPNG
                                 (path + "player_doubleSize.png", ren);

    const char user_name[MAX_LEN] = "player_username";
    /* Set the basic, default data for the player here. */
    /* Player will always be the first entity. */
    animation *player_frames = (animation *) malloc (sizeof(animation));
    player_frames->visual = playerTex;
    player_frames->next = NULL;

    Entity *player_graphical = (Entity *) malloc (sizeof(Entity));
    player_graphical->head_anim = player_frames;
    strcpy(player_graphical->id, user_name);
    player_graphical->x_comp = 100;
    player_graphical->y_comp = 300;
    player_graphical->num_frames = 1;
    player_graphical->priority = 100;

    Actor player_data = Actor(31, 90, 1, 1, 499);
    bag *inventory = (bag *) malloc (sizeof(bag));
    inventory->size = 0;

    int set_ind;
    Entity **entity_list = (Entity **) malloc (16*sizeof(Entity *));
    if (entity_list == NULL) {
        printf("Evident failure in allocating memory for the entity list.\n");
        free (player_graphical);
        free (player_frames);
        SDL_DestroyTexture (bgTex);
        SDL_DestroyTexture (playerTex);
        SDL_DestroyWindow (win);
        SDL_Quit();
        return 1;
    }
/*    printf("Allocated memory for entity_list, the array of pointers\n");*/
    entity_list[0] = player_graphical;
    for (set_ind = 1; set_ind < DEFAULT_MAX_ENT;  set_ind ++) {
        entity_list[set_ind] = NULL;
    }
    heap *master_heap = (heap *) malloc (sizeof(heap));
    if (master_heap == NULL) {
        printf("Evident failure in allocating memory for the master heap.\n");
        free (entity_list);
        free (player_graphical);
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
    for (indr = 0; indr <= 271; indr ++) {
        for (indc = 0; indc < WIN_WIDTH; indc ++) {
            map_matrix[indr][indc] = 1;
        }
    }

    int *stored_ptr = map_matrix[0];
    showNewBackground(bg_num, master_heap, ren, &stored_ptr, &bgTex);
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
    SDL_QueryTexture(player_graphical->head_anim->visual,
                     NULL, NULL, &playerWidth, &playerHeight);
    int speed = 2;
    bool show_inventory = false;
    SDL_Event ev;
    bool open = true/*, iteration = 0*/;
/*    float scale = 2.0f;*/
    unsigned int ctr = 1;

    std::string fontName = "fonts/FreeSerifBold.ttf";
    SDL_Color textColor = {246, 142, 86, 255};
    std::stringstream numer, denom;
    numer << player_data.getHP();
    denom << player_data.getMaxHP();
    SDL_Texture *hp_num = renderText(numer.str(), path + fontName, textColor, 18, ren);
    SDL_Texture *hp_denom = renderText(denom.str(), path + fontName, textColor, 18, ren);

/* Now, to prepare for the main game loop, we need to set structures
 * to keep track of time. */
    struct timespec elapsed = {0, 0};
    int res = clock_getres(CLOCK_REALTIME, &elapsed);
    if (res == 0) {
        /* no-op*/
    } else {
        printf("Problem getting the wall time. ERR.\n");
        return -1;
    }

/* ******   MAIN GAME LOOP BELOW   ****** */

    while (open) {
        heap *h_prime = NULL;
        /* Update coordinates: */
        x_coord = (player_graphical->x_comp 
                    + ((/*scale* */playerWidth)/2));
        y_coord = player_graphical->y_comp
                    + ((/*scale* */playerHeight));
        while(SDL_PollEvent(&ev)) {
            /* Polled an event for this iteration. */
            if (ev.type == SDL_QUIT) {
            /* Quit - by exiting the loop / closing the window. */
                open = false;
            }
            else if (ev.type == SDL_KEYDOWN) {
                /* Process the WASD key presses: */
                if (ev.key.keysym.sym == SDLK_d) {
                    if (map_matrix[y_coord][x_coord + speed] == 0) {
                        player_graphical->x_comp += speed;
                    }
                } else if (ev.key.keysym.sym == SDLK_a) {
                    if (map_matrix[y_coord][x_coord - speed] == 0) {
                        player_graphical->x_comp -= speed;
                    } else {
/*                        printf("map_matrix[y_coord][x_coord-speed] = map_m"\
                               "atrix[%d][%d]=1\n", y_coord, x_coord-speed);*/
                    }
                } else if (ev.key.keysym.sym == SDLK_w) {
                    if (map_matrix[y_coord - speed][x_coord] == 0) {
                        player_graphical->y_comp -= speed;
                    }
                } else if (ev.key.keysym.sym == SDLK_s) {
                    if ((y_coord + speed < WIN_HEIGHT) && 
                              (map_matrix[y_coord + speed][x_coord] == 0)) {
                        player_graphical->y_comp += speed;
                    } else if (y_coord + speed >= WIN_HEIGHT) {
/*                        printf("%d + %d >= WIN_HEIGHT (365)\n",
                                                      y_coord, speed);*/
                    } else {
/*                        printf("map_matrix[y_coord+speed][x_coord] = map_matrix[%d][%d]=1\n", y_coord+speed, x_coord);*/
                    }
                } else if ((ev.key.keysym.sym == SDLK_x) || (ev.key.keysym.sym == SDLK_q)) {
                /* Allow exit on X key or Q key: */
                    open = false;
/* The following case in development - where the player presses enter,
 * to interact with another entity.*/
                } else if (ev.key.keysym.sym == SDLK_RETURN) {
/*                    checkAction(player_data, map_matrix);*/
                      printf ("You've pressed the ENTER button. Case is in development.\n");
                } else if (ev.key.keysym.sym == SDLK_e) {
                    /* Find out if the inventory needs to be 
                     * opened or closed... adjust game_state. */
                    if ((game_state % 4) == 0)  {
                        game_state ++;
                        show_inventory = true;
                    } else if ((game_state % 4) == 1) {
                        game_state --;
                        show_inventory = false;
                    }
                }

                /* Allow player to loop around in the x axis direction*/
                /* Note: in this current implementation, this only works
                 * going in the rightward direction...
                 * Obviously, this will not be the final implementation.
                 * After all, we want the player to be able to go to 
                 * different screens by moving around! - VC */
                if (x_coord < 0) {
                    int numEntities=(master_heap->size), deleteInd=numEntities-1;
                    int *mapNotReadOnly = map_matrix[0];
                    bg_num --;
                    printf("msg: NOW CHANGING THE BACKGROUND (id now %d)\n", bg_num);
                    /* First, delete all entities other than the player entity*/
                    for (; deleteInd >= 0; deleteInd --) {
                        if (master_heap->list[deleteInd] != player_graphical) {
                            deleteEntity(master_heap, master_heap->list[deleteInd]);
                        }
                    }
                    /* Finally, load the new background. */
                    showNewBackground(bg_num, master_heap, ren, &mapNotReadOnly, &bgTex);
                    player_graphical->x_comp = WIN_WIDTH-playerWidth-1;
                    x_coord = (player_graphical->x_comp 
                                           + ((/*scale* */playerWidth)/2));
                } 
                if (x_coord > WIN_WIDTH) {
                    int numEntities=(master_heap->size), deleteInd=numEntities-1;
                    int *mapNotReadOnly = map_matrix[0];
                    bg_num ++;
                    printf("NOW CHANGING THE BACKGROUND (id now %d)\n", bg_num);
                    /* First, delete all entities other than the player entity*/
                    for (; deleteInd >= 0; deleteInd --) {
                        if (master_heap->list[deleteInd] != player_graphical) {
                            deleteEntity(master_heap, master_heap->list[deleteInd]);
                        }
                    }
                    /* Finally, load the new background. */
                    showNewBackground(bg_num, master_heap, ren, &mapNotReadOnly, &bgTex);
                    player_graphical->x_comp = 1;
                    x_coord = (player_graphical->x_comp 
                                           + ((/*scale* */playerWidth)/2));
                }
                player_graphical->x_comp %= WIN_WIDTH;
            }
        }
/* So after polling for events, we can use a time-step numerical method to 
 * calculate any changes in the position of entities on the screen.
 * This is a function of how much time has passed...*/
        long lastTime = elapsed.tv_nsec;
        res = clock_gettime(CLOCK_REALTIME, &elapsed);
        long dur = ((elapsed.tv_nsec - lastTime) / 1000);
        printf("Time since last frame (microseconds): %lu\n", dur);
        SDL_RenderClear(ren);

        /* Now, to render all the proper textures of each entity.*/
        unsigned int ctr_adjusted = ctr % 5;
        if ((ctr % 10000) == 0) {
            ctr_adjusted ++;
            printf("Just healed another 10 HP!\n");
            player_data.gainHP (10);

        /* Now update the pointer to the HP_NUMERATOR SDL_Texture.*/
        /* To do this, first delete the texture: */
            SDL_DestroyTexture(hp_num);
            numer.str(std::string());
            numer << player_data.getHP();
            hp_num = renderText(numer.str(), path + fontName, textColor, 18, ren);

            int nextInd = (ctr / 10000) - 1;
            if (nextInd < 8) {
                std::stringstream temporaryStorage;
                temporaryStorage << nextInd;
                std::string nextIndStr = temporaryStorage.str();
                printf ("%s is the nextIndStr this time.\n", nextIndStr.c_str());
                animation *comp = (animation *) malloc(sizeof(animation));
                comp->visual = loadTextureFromPNG(path + "test" + nextIndStr + ".png", ren);
                comp->next = NULL;
                item *nextItem = (item *) malloc(sizeof(item));
                nextItem->anim = comp;
/*            nextItem->occurrence = 1;*/
                nextItem->weight = 5;
                addItem(inventory, nextItem);
            }
        } else {
            if (((ctr % 30) == 0)) {
/*                printf("%u frames passed\n", ctr);*/
            }
        }

        renderHeap (master_heap, ctr_adjusted, ren, &h_prime,
                    player_data.getHP(), player_data.getMaxHP(), hp_num,
                    hp_denom);
        if (show_inventory) {
            displayInventory(ren, inventory);
        }
        SDL_RenderPresent(ren);
        master_heap = h_prime;
        ctr ++;
    }
    ind = (master_heap->size - 1);

    for (; ind >= 0; ind --) {
        SDL_DestroyTexture(master_heap->list[ind]->head_anim->visual);
        free(master_heap->list[ind]->head_anim);
        free(master_heap->list[ind]);
    }

    ind = (inventory->size - 1);
    for (; ind >= 0; ind --) {
        SDL_DestroyTexture(inventory->inv[ind]->anim->visual);
        free(inventory->inv[ind]->anim);
        free(inventory->inv[ind]);
    }

    free(inventory);
    free(master_heap->list);
    free(master_heap);
    IMG_Quit();
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

void logSDLError(std::ostream &os, const std::string &message) {
    os << message << " ERR: " << SDL_GetError() << std::endl;
}
