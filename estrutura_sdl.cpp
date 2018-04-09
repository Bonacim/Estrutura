#include "estrutura_sdl.h"


//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "Estrutura", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

			}
		}
	}

	return success;
}

void close()
{
	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

void create_window()
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
}

void render_window(bool *quit, Estrutura* obj)
{
	//Event handler
	SDL_Event e;

	int x,y;
	int draw_x, draw_y;

	//While application is running
	if( !(*quit) )
	{
		//Handle events on queue
		while( SDL_PollEvent( &e ) != 0 )
		{
			//User requests quit
			if( e.type == SDL_QUIT )
			{
				(*quit) = true;
			}
		}

		//Clear screen
		SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
		SDL_RenderClear( gRenderer );

		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
		for (y = 0; y < obj->GetH(); y++) 
		{
			for (x = 0; x < obj->GetLargura(y); x++) 
			{
				draw_x= arredonda (SDL_OFFSET + SDL_ESCALA * (obj->GetPosX(y,x))/ ESCALA);
				draw_y = arredonda (SDL_OFFSET + SDL_ESCALA * (obj->GetPosY(y,x)) / ESCALA);
				SDL_RenderDrawPoint( gRenderer,  draw_x, draw_y);
			}
		}

		//Update screen
		SDL_RenderPresent( gRenderer );
	}
	else
	{
		close();
	}
}

int arredonda (double x)
{
    if (x >= 0)
        return ((int) (x + 0.5));
    else
        return ((int) (x - 0.5));
}