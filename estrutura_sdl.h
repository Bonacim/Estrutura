#include "estrutura.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


//Screen dimension constants
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1036;

const int SDL_OFFSET = 20;
const double SDL_ESCALA = 100;

//Starts up SDL and creates window
bool init();

//Frees media and shuts down SDL
void close();

void create_window();

void render_window(bool *quit, Estrutura* obj);

int arredonda (double x);