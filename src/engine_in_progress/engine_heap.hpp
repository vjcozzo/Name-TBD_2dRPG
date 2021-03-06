/*#ifndef "general_engine.h"*/
/*#define "general_engine.h"*/
#define MAX_LEN 64
#define MAX_INV 32
#define N 200
#define WIN_HEIGHT 365
#define WIN_WIDTH 710
#define DEFAULT_MAX_ENT 16
#define MAX_PRI_VAL 999

/* Note: this would have been called "Player"; however,
 * I would not  be surprised if we wanted to use
 * a very very similar structure to model enemy health / data.
 * Correct me if I'm wrong, but the basic principles are the same
 * whether the object is the player, or an enemy. 
 * So I name this class 'actor' to represent a general object type.*/
class Actor {
    private:
    unsigned int health, maxHealth;
    unsigned int exp, level, toNext;
    bool alive, invincible;

    public:
    /* Constructor */
    Actor (unsigned int hp, unsigned int maxHP, unsigned int exper,
           unsigned int nowLevel, unsigned int toNextLev) {
        health = hp;
        maxHealth = maxHP;
        exp = exper;
        level = nowLevel;
        toNext = toNextLev;
        alive = true;
        invincible = false;
    }
    
    void gainHP (unsigned int amnt) {
        health += amnt;
	if (health > maxHealth) {
            health = maxHealth;
	}
    }
    void loseHP (unsigned int amnt) {
        if (! invincible) {
            health -= amnt;
            if (health <= 0) {
                health = 0;
                alive = false;
            }
	}
    }
    void initInvincible () {
        invincible  = true;
    }

    /* Public retrieval methods */
    unsigned int getHP () {
        return health;
    }
    unsigned int getMaxHP () {
        return maxHealth;
    }
/*
    kill () {
        alive = false;
    }
*/
};

struct animation {
    SDL_Texture *visual;
    animation *next;
};

struct item {
    /* General structure for any item that can be accessed 
     * from the dense search tree (the inventory) */
    animation *anim;
    std::string id;
/*    item *left, *right;*/
    unsigned int weight/*, occurrence*/;
};

struct bag {
    /* Actual contents depend on how we store the inventory
     * Possibilities:
       *Dense_search_tree (Sorted, can search efficiently)
       *Heap (ordered based on each object's weight?)
       *Array (if we're limiting contents based on number)
     */
	/* This implementation: An Array. */
    item *inv[MAX_INV];
    unsigned int size;
};

typedef struct entity {
    animation *head_anim;
    char id[MAX_LEN];
    int x_comp, y_comp, num_frames, priority;
    /* any additional data can be stored below... */
} Entity;

struct heap {
    Entity **list;
    int size;
    int max_size;
};

void logSDLError (std::ostream &out, const std::string &msg);
SDL_Texture *loadTextureFromPNG (const std::string &fileName, SDL_Renderer *ren);
void renderTexture (SDL_Texture *tex, SDL_Renderer *ren, int x, int y);
void renderTextureFactor (SDL_Texture *tex, SDL_Renderer *ren, int x, int y, float fact);
const char* getBGStringFromNum (unsigned int ref_num);
SDL_Texture *renderText (const std::string &msg, const std::string &fontFile, SDL_Color color, int fontSize, SDL_Renderer *ren);
void addAnimation (Entity *subject, SDL_Texture *texture);
void deleteAnimationFrames (animation *frame, int iterations);
void deleteEntity (heap *head, Entity *addr);
void initHeap (heap **h);
void addToHeap (heap **h, Entity *tba);
heap *increment_size (heap *h);
void deleteHeap (heap *h);
void renderHeap (heap *h, unsigned int count, SDL_Renderer *rend,
		 heap **used, unsigned int hp, unsigned int max,
                 SDL_Texture *num, SDL_Texture *den);
Entity *makeEntity (SDL_Texture *base_pic, char *label, int x, int y, int frames, int pri);
void displayInventory (SDL_Renderer *ren, bag *items);
int showNewBackground (int bg_num, heap *ents, SDL_Renderer *ren, int **map, SDL_Texture **bgTex);
void clearMap (int **map_ptr, int horiz);
void addItem (bag *total, item *tba);
/*
void deleteRecursive (item *elist);
void addEntity (item** elist, SDL_Texture *texture, char* id, int x_pos, int y_pos);
void addNode (item** elist, item *toBeAdded);
void addAllNodes (item **dest, item *src);
void renderAllTextures (item* startNode, SDL_Renderer *renderer);
*/
/*#endif    */

SDL_Texture *loadTextureFromPNG(const std::string &file, SDL_Renderer *ren) {
    int flag = IMG_INIT_PNG;
    if (IMG_Init(IMG_INIT_PNG)!=flag) {
        logSDLError(std::cout, "SDL_image IMG_Init - from inner function\n");
        return NULL;
    }
    const char *mod = "r+b";
    SDL_Texture *texture = NULL;
    SDL_RWops *rwop = SDL_RWFromFile(file.c_str(), mod);
    if (rwop == NULL) {
        printf("Error in SDL_RWFromFile()...\ncould not load %s", file.c_str());
    }
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

/* Conerts a reference number (directly corresponding to 
 * a background texture) to the string associated with it.*/
/* In the future, the idea is, we could easily add new 'cases'
 * and strings to return. 
 * Perhaps this is only a short-run solution (let me know if
 * you have a better system) but this works for now.
 * Note: an added benefit could be the ease in switching 
 * background textures (see GoogleDoc). - VC */
const char* getBGStringFromNum (unsigned int ref_num) {
    switch (ref_num) {
        case 99: return "plains_fort.png\0"; break;
        case 100: return "woods_bg.png\0"; break;
        case 101: return "woods_bg_2.png\0"; break;
        default: return "woods_bg.png\0"; break;
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

/* Precondition: id is a non-null, valid name for the item
 * Postcondition: will return a pointer to a newly-allocated
 * item structure*/
item *makeItem () {
    /**/
    return NULL;
    /* Like I said, this is under major construction.
     * I'll generate a new item structure given various parameters
     * just give me some time to implement this. -VC
     *
     * Or better yet, do it yourself if you're impatient! ;)
     * */
}

/* Will take a pointer to a pointer to a heap,
 * and initialize that heap (the pointer then will point
 * to a valid heap * pointer, one that is not NULL. */
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

/* Add a specific Entity structure to this heap.
 * Precondition: tba is a non-null Entity constructed
 * through the makeEntity() function.
 * Postcondition: h  will point to a valid, non-NULL heap pointer
 * that will include tba (to be added) in its list of entities.
 * (Of course, the size will also be adjusted as necessary) */
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
    /* can return now. */
}

/* Increments the size of a fixed heap structure, given
 * by the parameter, a non-NULL heap pointer. */
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

/* Deletes (de-allocates all assocaited memory) a heap
 * given by a general heap pointer tbd (to be deleted) */
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

/* General function for rendering the total contents of the entity heap
 * at a given time (called witin the game loop. 
 * Preconditions:
 *     h must be a non-NULL heap pointer.
 *     rend must be a non-NULL Renderer pointer.
 * Postcondition: will render all entities in the heap. */
void renderHeap (heap *h, unsigned int count, SDL_Renderer *rend,
                 heap **used, unsigned int hp, unsigned int max,
                 SDL_Texture *hp_num, SDL_Texture *hp_denom) {
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
        if (strcmp(nextRoot->id, "health_base_bg_white.png") == 0) {
            /* yup - this  is the health bar.
             * We want to process it in a unique way. */
            /* Specificaally, we fill a rectangle at the appropriate
             * width to model the palyer's current HP ratio. */
            int start_bars_ind = (x_pos+125);
            int width, height, upper_bound;
            float factor = (float) ((float) (hp) / max);
            SDL_QueryTexture(frame_texture, NULL, NULL, &width, &height);
            upper_bound = ((factor * (width - 125)) + 1) + 125;
            SDL_SetRenderDrawColor(rend, 246, 142, 86, 255);
            SDL_Rect filled;
            filled.x = x_pos+125;
            filled.y = y_pos;
            filled.w = (upper_bound - start_bars_ind) + 1;
            filled.h = height;
            SDL_RenderFillRect(rend, &filled);
            SDL_RenderDrawRect(rend, &filled);
            renderTexture(hp_num, rend, 48, 4);
            renderTexture(hp_denom, rend, 86, 4);
        }
        deleteEntity (h, nextRoot);
        addToHeap (used, nextRoot);
    }
}

void addItem (bag *items, item *tba) {
    if (items == NULL) {
        printf("ERR: BAG IS EMPTY. See where you call addItem() to debug.\n");
        return;
    }
    int num_items = items->size;
    if ((num_items < 0) || (num_items > MAX_INV)) {
        /*****/
        printf("ERR: BAG SIZE INVALID. See where the item inventory is manipulated in order to ebug.\n");
    } else if (items->size == MAX_INV) {
        printf("Cannot add to bag -- bag is full. Returning.\n");
    } else {
        /*****/
        items->inv[num_items] = tba;
        items->size ++;
    }
}

/* Precondition: ren and items are non-NULL pointers.
 * Postcondition: will render a list of each item in the inventory*/
void displayInventory (SDL_Renderer *ren, bag *items) {
    unsigned int indi;
    /* First, generate a gray overlay, over everything rendered so far. */
    SDL_SetRenderDrawColor(ren, 200, 205, 220, 199);
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    SDL_Rect filled;
    filled.x = 5;
    filled.y = 20;
    filled.w = (WIN_WIDTH - (2*filled.x));
    filled.h = WIN_HEIGHT - filled.y;
    SDL_RenderFillRect(ren, &filled);
    SDL_RenderDrawRect(ren, &filled);

    /* Next: deal directly with the bag.
     * Um. We don't know the design here yet, so 
     * we'll finish this function later.*/

    /** FUNCTION UNDER CONSTRUCTION **/

    /* The following variables represent the number of items that will
     * be displayed - for each row (X) and for each column (Y) */
    int modX = 8, modY = 8;

    /* storing the width and height values for each icon displayed: */
    int indivWidth = (WIN_WIDTH / modX);
    int indivHeight = (WIN_HEIGHT / modY);

    for (indi = 0; indi < (items->size); indi ++) {
        int x_pos, y_pos;
        x_pos = ((indi % modX)*(indivWidth)) + 5;
        y_pos = ((indi / modY)*(indivHeight)) + 20;
        SDL_Texture *nextItemText = (items->inv[indi]->anim->visual);
        renderTexture(nextItemText, ren, x_pos, y_pos);
    }
}

/* THE FOLLOWING IS **REALLY** UNDER CONSTRUCTION-----
 * FUNCTIONS have been yanked from engine_bst.cpp
 * and I expect they will prove useful for the dense-search-tree
 * implementation of the inventory.*/
/*
void deleteEntity (item *head, Entity *addr) {
    int lastStep = 0;
    item *aheadNode = head;
    item *tmp = NULL;

    while ((aheadNode != NULL) &&
           (strcmp(aheadNode->info->id, addr->id))) {
        int compare_result = strcmp (aheadNode->info->id, addr->id);
        if (compare_result > 0) {
            tmp = aheadNode;
            aheadNode = aheadNode->left;
            lastStep = 1;
        } else if (compare_result < 0) {
            tmp = aheadNode;
            aheadNode = aheadNode->right;
            lastStep = -1;
        } else {*/
            /* This is the Node we need to delete!! */
/*            if (lastStep > 0) {
                tmp->left = NULL;
            } else *//*if (lastStep < 0) {*/
/*                tmp->right = NULL;*/
        /* For each of THIS deleted Node's children,*/
        /* add them to the rest of the tree. */
/*            addAllNodes (&head, aheadNode->left);
            addAllNodes (&head, aheadNode->right);
            SDL_DestroyTexture (aheadNode->info->visual);
            free (aheadNode->info);
            free (aheadNode);
        }
    }

    if ((aheadNode == NULL) || (tmp == NULL)) {
        return;
    }
}*/

/*
void addEntity (item** eList, SDL_Texture *texture, char *identifier, int x_pos, int y_pos) {
    int lastStep = 0;
    item *aheadNode = *eList;
    item *tmp = NULL;

    if ((texture == NULL) || (identifier == NULL)) {
        printf ("Error: have tried to add an entity to the BST, where:\n");
        if ((texture == NULL)) {
            printf ("\tThe texture given is NULL. This is a weird error.\n");
        }
        else {
            printf ("\tThe identifier is NULL. Check where you call this addEntity() fct,\n\t and then verify the Entity name.\n");
        }
        return;
    }
    while (aheadNode != NULL) {
        if (strcmp (aheadNode->info->id, identifier) < 0) {
            tmp = aheadNode;
            aheadNode = aheadNode->right;
            lastStep = -1;
        } else {
            tmp = aheadNode;
            aheadNode = aheadNode->left;
            lastStep = 1;
        }
    }

    if (tmp == NULL) {*/
        /* In the case that *eList (the head node location) is NULL */
/*        Entity *head = (Entity *) malloc (sizeof (Entity));
        head->visual = texture;
        strcpy (head->id, identifier);
        head->id[strlen(identifier)] = '\0';
        head->x_comp = x_pos;
        head->y_comp = y_pos;
        
        item *newNode = (item *) malloc (sizeof (item));
        newNode->info = head;
        newNode->left = NULL;
        newNode->right = NULL;
        *eList = newNode;
        return;
    }

    Entity *newEnt = (Entity *) malloc (sizeof (Entity));
    newEnt->visual = texture;
    strcpy (newEnt->id, identifier);
    newEnt->id[strlen(identifier)] = '\0';
    newEnt->x_comp = x_pos;
    newEnt->y_comp = y_pos;

    item *newNode = (item *) malloc (sizeof(item));
    newNode->info = newEnt;
    newNode->left = NULL;
    newNode->right = NULL;

    if (lastStep > 0) {
        tmp->left = newNode;
    } else {
        tmp->right = newNode;
    }
}*/

/* Note: this addNode() function does no  additional
 * dynamic memory allocation.
 * This is a main distinction between it and the createEntity() funct.*/
/*
void addNode (item** eList, item *toBeAdded) {
    int lastStep = 0;
    item *aheadNode = *eList;
    item *tmp = NULL;

    while (aheadNode != NULL) {
        int compare_result = strcmp (aheadNode->info->id, toBeAdded->info->id);
        if (compare_result < 0) {
            tmp = aheadNode;
            aheadNode = aheadNode->right;
            lastStep = -1;
        } else {
            tmp = aheadNode;
            aheadNode = aheadNode->left;
            lastStep = 1;
        }
    }

    if (tmp == NULL) {*/
    /* In the case that *eList (the head node location) is NULL */
/*        *eList = toBeAdded;
        return;
    }

    if (lastStep > 0) {
        tmp->left = toBeAdded;
*//*
    } else*/ /*if (lastStep <= 0)*//* {
        tmp->right = toBeAdded;

    }
}*/
/*
void addAllNodes (item **destination, item *source) {
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
}*/
/*
void deleteRecursive (item *top) {
    if (top == NULL) {
        return;
    } else {
        deleteRecursive (top->left);
        deleteRecursive (top->right);
        deleteEntity (top, top->info);
    }
}*/

/* A general function for doing most of the work originally done
 * BEFORE the main game loop - e.g., reading default entities from files
 * and adding them to the main entity heap.
 *
 * This is a very general function, because it should be called
 * whenever the screen changes to a new background (e.g., the player
 * moves far enough to the right to trigger a screen change).
 *
 * It is a work in progress.
 * -VC
 *  */
int showNewBackground (int bg_num, heap *entity_heap, SDL_Renderer *ren, int **map, SDL_Texture **bgText) {
    const std::string path = "../../res/";
    const char *data = "bg_data.txt";
    int ind=0, indr=0, indc=0, path_len=path.size();
    const char *bg = getBGStringFromNum(bg_num);
    char whole_line[4*MAX_LEN] = {'\0'};
    bool found = false;

    FILE *database = fopen(data, "r");
    while ((!found) && fgets(whole_line, 4*MAX_LEN + 1, database)) {
        int initX, initY, subtraction, horizon;
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
        sscanf(excludeFirst, "%d, %d, %d", &initX, &initY, &horizon);
        clearMap(map, horizon);
        if (strcmp(bg, pngName)) {
            int t;
            for (t = 0; t < indc; t ++) {
                pngName[t] = '\0';
            }
        } else {
            char pngFileName[MAX_LEN] = {'\0'};
            ind = 0;
            int cap = strlen(whole_line);
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
                printf("ERR: File name is too long.\nWe've identified "\
                       "the file for background %s\n", pngName);
                return -1;
            }
            pngFileName[ind] = '.';
            pngFileName[ind+1] = 't';
            pngFileName[ind+2] = 'x';
            pngFileName[ind+3] = 't';
            FILE* specific_entities_list = fopen(pngFileName, "r");
            ind = 0;
            while (fgets(whole_line, 4*MAX_LEN+1, specific_entities_list)) {
                int width, height, indf, frame_count=1;
                char postLine[MAX_LEN] = {'\0'};
                char nextPngName[MAX_LEN] = {'\0'};
                char nextPngNameWithPath[MAX_LEN] = {'\0'};
                int ent_initX, ent_initY, entPri,
                    floorX, floorY;
                for (indc = 0; indc < path_len; indc ++) {
                    nextPngNameWithPath[indc] = path[indc];
                }
                while ((whole_line[indc - path_len] != ',') &&
                       (whole_line[indc - path_len] != '\n')) {
                    nextPngNameWithPath[indc] = whole_line[indc - path_len];
                    nextPngName[indc - path_len] = whole_line[indc - path_len];
                    indc ++;
                }
                indc -= path_len;
                indc ++;
                subtraction = indc;
                while ((whole_line[indc] != '\n')
                    && (indc < MAX_LEN)) {
                    postLine[indc-subtraction] = whole_line[indc];
                    indc ++;
                }
                indc = 0;
                sscanf(postLine, "%d,%d,%d,%d,%d,%d",
                          &ent_initX, &ent_initY,
                          &floorX, &floorY, &frame_count, &entPri);
                SDL_Texture *next_texture 
                            = loadTextureFromPNG(nextPngNameWithPath, ren);
                Entity *nextEnt = makeEntity (next_texture, nextPngName,
                                ent_initX, ent_initY, frame_count, entPri);
                if (ind == 0) {
                    if (bgText == NULL) {
                        logSDLError (std::cout, "background texture pointer null. See showNewBackground().");
                    } else {
                        *bgText = next_texture;
                    }
                }
                indf = 1;
                if (frame_count > 1) {
                    while (((indf < frame_count)) &&
                        (fgets(whole_line, 4*MAX_LEN+1,
                        specific_entities_list) != NULL)) {
                        sscanf(whole_line, "%s\n", nextPngName);
                        if (nextPngName[strlen(nextPngName) - 1] == '\n') {
                            nextPngName[strlen(nextPngName) - 1] = '\0';
                        }
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
                addToHeap(&entity_heap, nextEnt);
                for (indr = floorY;
                    (indr < floorY+height) &&
                    (indr < WIN_HEIGHT); indr ++) {
                    for (indc = floorX;
                        (indc < floorX+width) &&
                        (indc < WIN_WIDTH);
                          indc ++) {
                        *((*map) + (indr*WIN_WIDTH) + indc) = 1;
                    }
                }
                ind ++;
            }
            fclose(specific_entities_list);
            found = true;
        }
    }
    fclose(database);
    return 0;
}

void clearMap (int **map, int horizon) {
    int indrow, indcol;
    for (indrow = 0; indrow < horizon; indrow ++) {
        for (indcol = 0; indcol < WIN_WIDTH; indcol ++) {
            *((*map) + (WIN_WIDTH*indrow) + (indcol)) = 1;
        }
    }
    for (; indrow < WIN_HEIGHT; indrow ++) {
        for (indcol = 0; indcol < WIN_WIDTH; indcol ++) {
            *((*map) + (WIN_WIDTH*indrow) + (indcol)) = 0;
        }
    }
}
