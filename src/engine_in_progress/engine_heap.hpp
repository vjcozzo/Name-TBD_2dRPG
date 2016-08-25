/*#ifndef "general_engine.h"*/
/*#define "general_engine.h"*/
#define MAX_LEN 64
#define MAX_INV 32

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
		 heap **used, unsigned int hp, unsigned int max);
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
