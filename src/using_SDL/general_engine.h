/*#ifndef "general_engine.h"*/
/*#define "general_engine.h"*/
const int height = 365;
const int width = 710;

void logSDLError (std::ostream &out, const std::string &msg);
SDL_Texture *loadTextureFromPNG (const std::string &fileName, SDL_Renderer *ren);
void renderTexture (SDL_Texture *tex, SDL_Renderer *ren, int x, int y);
void renderTextureFactor (SDL_Texture *tex, SDL_Renderer *ren, int x, int y, float fact);
const char* getBGStringFromNum (unsigned int ref_num);
SDL_Texture *renderText (const std::string &msg, const std::string &fontFile, SDL_Color color, int fontSize, SDL_Renderer *ren);

typedef struct entity {
	SDL_Texture *visual;
	int x_comp, y_comp;
	/* any additional data can be stored below... */
} Entity;

struct node {
	Entity info;
	struct node *next;
};

/*#endif	*/
