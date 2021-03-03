
/*

										PROJET PONG

										6 May 2020
						 Modified by Thomas BRUNET / Dorian FOURNIER
						  B1 -  Aéronautique et systèmes embarqués

*/

//BEGIN HEAD
//BEGIN DESCRIPTION

/* A Slider, follow up from mousegrab
 * click and hold the left mouse button
 * and move it
 */

 /* DEFINED PROGRESS GOALS
  *
  * TODO Make it nice
  * TODO Render Values
  */
  //END   DESCRIPTION

  //BEGIN INCLUDES

//system headers
#include <math.h>
//local headers
#include "helper.h"
#include "STM8.h"
#include "stdio.h"
#include "errno.h""
#include <SDL2/SDL.h>	

//END   INCLUDES

//BEGIN CPP DEFINITIONS
#define WHITE 255,255,255,255
#define BLACK 0,0,0,00
#define RED   255,0,0,255

// ADD NNC SERIAL
#define SERIAL_PORT_BUFFER_LENGTH 20
//#define SERIAL_PORT "COM5"
#define SERIAL_PORT "CNCA0"
//END   CPP DEFINITIONS

//BEGIN DATASTRUCTURES
//END	DATASTRUCTURES

//BEGIN GLOBALS
int wh;
int ww;

char etatbarre = 104;				//initialisation de différentes variables 
char player_goal = 0;
unsigned long time = 0;
char etatmusic = 124;

SDL_Texture* btn_play = NULL;
SDL_Rect 	btn_play_dst;

SDL_Texture* btn_pause = NULL;
SDL_Rect 	btn_pause_dst;

//==================== LEDs =======================

SDL_Texture* blanc = NULL;
SDL_Rect 	blanc_dst;

SDL_Texture* blanc1 = NULL;
SDL_Rect 	blanc1_dst;

SDL_Texture* blanc2 = NULL;
SDL_Rect 	blanc2_dst;

SDL_Texture* rouge = NULL;
SDL_Rect 	rouge_dst;

SDL_Texture* rouge1 = NULL;
SDL_Rect 	rouge1_dst;

SDL_Texture* rouge2 = NULL;
SDL_Rect 	rouge2_dst;

//================ RGB ==============================

SDL_Texture* RGB_rouge = NULL;
SDL_Rect 	RGB_rouge_dst;

SDL_Texture* RGB_vert = NULL;
SDL_Rect 	RGB_vert_dst;

SDL_Texture* RGB_blanc = NULL;
SDL_Rect 	RGB_blanc_dst;

SDL_Texture* dash = NULL;
SDL_Rect 	dash_dst;


//================== 7 segments J1 ====================//

SDL_Texture* SGB_A = NULL;
SDL_Rect 	SGB_A_dst;

SDL_Texture* SGB_B = NULL;
SDL_Rect 	SGB_B_dst;

SDL_Texture* SGB_C = NULL;
SDL_Rect 	SGB_C_dst;

SDL_Texture* SGB_D = NULL;
SDL_Rect 	SGB_D_dst;

SDL_Texture* SGB_E = NULL;
SDL_Rect 	SGB_E_dst;

SDL_Texture* SGB_F = NULL;
SDL_Rect 	SGB_F_dst;

SDL_Texture* SGB_G = NULL;
SDL_Rect 	SGB_G_dst;

SDL_Texture* SGR_A = NULL;
SDL_Rect 	SGR_A_dst;

SDL_Texture* SGR_B = NULL;
SDL_Rect 	SGR_B_dst;

SDL_Texture* SGR_C = NULL;
SDL_Rect 	SGR_C_dst;

SDL_Texture* SGR_D = NULL;
SDL_Rect 	SGR_D_dst;

SDL_Texture* SGR_E = NULL;
SDL_Rect 	SGR_E_dst;

SDL_Texture* SGR_F = NULL;
SDL_Rect 	SGR_F_dst;

SDL_Texture* SGR_G = NULL;
SDL_Rect 	SGR_G_dst;

//================== 7 segments IA ====================//

SDL_Texture* SGB_A2 = NULL;
SDL_Rect 	SGB_A2_dst;

SDL_Texture* SGB_B2 = NULL;
SDL_Rect 	SGB_B2_dst;

SDL_Texture* SGB_C2 = NULL;
SDL_Rect 	SGB_C2_dst;

SDL_Texture* SGB_D2 = NULL;
SDL_Rect 	SGB_D2_dst;

SDL_Texture* SGB_E2 = NULL;
SDL_Rect 	SGB_E2_dst;

SDL_Texture* SGB_F2 = NULL;
SDL_Rect 	SGB_F2_dst;

SDL_Texture* SGB_G2 = NULL;
SDL_Rect 	SGB_G2_dst;

SDL_Texture* SGR_A2 = NULL;
SDL_Rect 	SGR_A2_dst;

SDL_Texture* SGR_B2 = NULL;
SDL_Rect 	SGR_B2_dst;

SDL_Texture* SGR_C2 = NULL;
SDL_Rect 	SGR_C2_dst;

SDL_Texture* SGR_D2 = NULL;
SDL_Rect 	SGR_D2_dst;

SDL_Texture* SGR_E2 = NULL;
SDL_Rect 	SGR_E2_dst;

SDL_Texture* SGR_F2 = NULL;
SDL_Rect 	SGR_F2_dst;

SDL_Texture* SGR_G2 = NULL;
SDL_Rect 	SGR_G2_dst;

//==================== FIN LEDs ==========================

//BEGIN VISIBLES
SDL_Surface* temp_surface = NULL;
SDL_Surface* temp_surface2 = NULL;

SDL_Texture* bg = NULL;
SDL_Rect 	bg_dst;

SDL_Texture* button = NULL;
SDL_Rect 	button_dst;
SDL_Rect 	button_bounds_dst;
//END 	VISIBLES

//BEGIN MOUSE
SDL_Point	mouse;
SDL_bool 	mouse_follow = SDL_FALSE;
SDL_Point 	mouse_offset;
//END 	MOUSE

//BEGIN FUNCTION PROTOTYPES
void assets_in(void);
void assets_out(void);
void inverse_LED(void);
void etatLED(void);
//END	FUNCTION PROTOTYPES


int main(int argc, char* argv[])
{

	(void)argc;
	(void)argv;

	// ADD NNC POUR SERIAL
	char serialPortBuffer[SERIAL_PORT_BUFFER_LENGTH] = { 0 };
	int serialPortReadDataLength = 0;

	if (!STM8_connect(SERIAL_PORT, 115200))
		exit(EXIT_FAILURE);

	//FIN ADD NNC POUR SERIAL
	
	//BEGIN INIT
	init();
	assets_in();

	//BEGIN WINDOW
	SDL_SetWindowPosition(Window, 100, 400 );
	SDL_SetWindowSize(Window, ww, wh);
	SDL_SetWindowTitle(Window, "STM8");
	SDL_ShowWindow(Window);

	SDL_Event event;
	int running = 1;
	
	const Uint8* keystate = SDL_GetKeyboardState(NULL);
	
	//BEGIN MAIN LOOP
	while (running) 
	{

		STM8_read(&player_goal, sizeof(SERIAL_PORT_BUFFER_LENGTH));			//lecture de la variable 'player_goal' reçu de pong

		etatLED();															//appel de la fonction etatLED 

		if (player_goal == 0 || player_goal == 't' || player_goal == 's')	
		{
				
			if (keystate[SDL_SCANCODE_SPACE])										//si la barre espace est appuyée 
			{

				etatbarre = 105;	

				STM8_write(&etatbarre, 1);											//envoi de la variable etatbarre au pong

				temp_surface = IMG_Load("./assets/gfx/RGB_blanc.png");				// reinitialisation des LEDs et du 7 segments en début de partie
				RGB_blanc = SDL_CreateTextureFromSurface(Renderer, temp_surface);

				temp_surface = IMG_Load("./assets/gfx/SGB_A.png");
				SGB_A = SDL_CreateTextureFromSurface(Renderer, temp_surface);
				temp_surface = IMG_Load("./assets/gfx/SGB_B.png");
				SGB_B = SDL_CreateTextureFromSurface(Renderer, temp_surface);
				temp_surface = IMG_Load("./assets/gfx/SGB_C.png");
				SGB_C = SDL_CreateTextureFromSurface(Renderer, temp_surface);
				temp_surface = IMG_Load("./assets/gfx/SGB_D.png");
				SGB_D = SDL_CreateTextureFromSurface(Renderer, temp_surface);
				temp_surface = IMG_Load("./assets/gfx/SGB_E.png");
				SGB_E = SDL_CreateTextureFromSurface(Renderer, temp_surface);
				temp_surface = IMG_Load("./assets/gfx/SGB_F.png");
				SGB_F = SDL_CreateTextureFromSurface(Renderer, temp_surface);
				temp_surface = IMG_Load("./assets/gfx/SGB_G.png");
				SGB_G = SDL_CreateTextureFromSurface(Renderer, temp_surface);

				temp_surface = IMG_Load("./assets/gfx/blanc.png");
				blanc = SDL_CreateTextureFromSurface(Renderer, temp_surface);
				temp_surface = IMG_Load("./assets/gfx/blanc1.png");
				blanc1 = SDL_CreateTextureFromSurface(Renderer, temp_surface);
				temp_surface = IMG_Load("./assets/gfx/blanc2.png");
				blanc2 = SDL_CreateTextureFromSurface(Renderer, temp_surface);

				temp_surface = IMG_Load("./assets/gfx/SGB_A2.png");
				SGB_A2 = SDL_CreateTextureFromSurface(Renderer, temp_surface);
				temp_surface = IMG_Load("./assets/gfx/SGB_B2.png");
				SGB_B2 = SDL_CreateTextureFromSurface(Renderer, temp_surface);
				temp_surface = IMG_Load("./assets/gfx/SGB_C2.png");
				SGB_C2 = SDL_CreateTextureFromSurface(Renderer, temp_surface);
				temp_surface = IMG_Load("./assets/gfx/SGB_D2.png");
				SGB_D2 = SDL_CreateTextureFromSurface(Renderer, temp_surface);
				temp_surface = IMG_Load("./assets/gfx/SGB_E2.png");
				SGB_E2 = SDL_CreateTextureFromSurface(Renderer, temp_surface);
				temp_surface = IMG_Load("./assets/gfx/SGB_F2.png");
				SGB_F2 = SDL_CreateTextureFromSurface(Renderer, temp_surface);
				temp_surface = IMG_Load("./assets/gfx/SGB_G2.png");
				SGB_G2 = SDL_CreateTextureFromSurface(Renderer, temp_surface);

				etatmusic = 107;													
				STM8_write(&etatmusic, 1);												//envoi de la variable 'etatmusic' à pong

				temp_surface = IMG_Load("./assets/gfx/btn_Play.png");
				btn_play = SDL_CreateTextureFromSurface(Renderer, temp_surface);
			}

		}

		else
		{
			etatbarre = 104;
		}
		
		//BEGIN EVENT LOOP
		SDL_GetMouseState(&mouse.x, &mouse.y);

		while (SDL_PollEvent(&event)) 
		{

			if (event.type == SDL_QUIT) 
			{
				running = 0;
			}
			if (event.type == SDL_MOUSEMOTION) 
			{
				; 
			}
			if (event.type == SDL_MOUSEBUTTONDOWN) 
			{
				if (event.button.button == SDL_BUTTON_RIGHT) 
				{
					;
				}
				if (event.button.button == SDL_BUTTON_MIDDLE)
				{
					;
				}
				if (event.button.button == SDL_BUTTON_LEFT) 
				{
					if (event.button.button == SDL_BUTTON_LEFT && SDL_PointInRect(&mouse, &button_dst) && !mouse_follow) 
					{
						mouse_offset.y = mouse.y - button_dst.y;
						mouse_follow = SDL_TRUE;
					}
					if (event.button.button == SDL_BUTTON_LEFT && SDL_PointInRect(&mouse, &btn_play_dst))		//si il y a eu un clic gauche souris sur le bouton
					{
						if (etatmusic == 107)		
						{
							temp_surface = IMG_Load("./assets/gfx/btn_Pause.png");								//chargement du bouton pause
							btn_play = SDL_CreateTextureFromSurface(Renderer, temp_surface);					//le bouton pause se met aux même coordonnées que le bouton play
							etatmusic = 106;																	//on passe 'etatmusic'à 106 et on l'envoie au pong
							STM8_write(&etatmusic, 1);															
						}

						else if (etatmusic == 106)																//si 'etatmusic == 106'
						{
						    temp_surface = IMG_Load("./assets/gfx/btn_Play.png");								//chargement du bouton play
							btn_play = SDL_CreateTextureFromSurface(Renderer, temp_surface);					//le bouton play se met aux même coordonnées que le bouton pause
							etatmusic = 107;																	//on passe 'etatmusic'à 107 et on l'envoie au pong
							STM8_write(&etatmusic, 1);															
						}
						
					}
					
				}
			}

			if (mouse_follow && event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
			{
				mouse_follow = SDL_FALSE;
			}

			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:						//si apppui echap alors on quite le simu  
					running = 0;
					break;

				case SDLK_r:
				case SDLK_BACKSPACE:
					break;

				case SDLK_p:
				case SDLK_COMMA:
					break;

				default:
					break;
				}
			}
		}
		//END   EVENT LOOP

		if (mouse_follow) 
		{

			button_dst.y = mouse.y - mouse_offset.y ;
			//BOUNDS CHECK
			//TOP
			if (button_dst.y <= button_bounds_dst.y)
				button_dst.y = button_bounds_dst.y;
			//BOT
			if (button_dst.y + button_dst.h >= button_bounds_dst.y + button_bounds_dst.h)
				button_dst.y = (button_bounds_dst.y + button_bounds_dst.h) - button_dst.h;

		}
		
		//AJOUT NNC

		char i = ((button_dst.y *126) / button_bounds_dst.h -28);  
		STM8_write(&i, 1);

		//FIN AJOUT NNC

		//BEGIN RENDERING
		SDL_SetRenderDrawColor(Renderer, BLACK);
		SDL_RenderClear(Renderer);
		SDL_RenderCopy(Renderer, bg, NULL, &bg_dst);
		SDL_RenderCopy(Renderer, btn_play, NULL, &btn_play_dst);
		SDL_RenderCopy(Renderer, btn_pause, NULL, &btn_pause_dst);

		SDL_RenderCopy(Renderer, blanc, NULL, &blanc_dst);
		SDL_RenderCopy(Renderer, blanc1, NULL, &blanc1_dst);
		SDL_RenderCopy(Renderer, blanc2, NULL, &blanc2_dst);

		SDL_RenderCopy(Renderer, rouge, NULL, &rouge_dst);
		SDL_RenderCopy(Renderer, rouge1, NULL, &rouge1_dst);
		SDL_RenderCopy(Renderer, rouge2, NULL, &rouge2_dst);

		SDL_RenderCopy(Renderer, RGB_blanc, NULL, &RGB_blanc_dst);
		SDL_RenderCopy(Renderer, RGB_vert, NULL, &RGB_vert_dst);
		SDL_RenderCopy(Renderer, RGB_rouge, NULL, &RGB_rouge_dst);

		SDL_RenderCopy(Renderer, SGB_A, NULL, &SGB_A_dst);
		SDL_RenderCopy(Renderer, SGB_B, NULL, &SGB_B_dst);
		SDL_RenderCopy(Renderer, SGB_C, NULL, &SGB_C_dst);
		SDL_RenderCopy(Renderer, SGB_D, NULL, &SGB_D_dst);									
		SDL_RenderCopy(Renderer, SGB_E, NULL, &SGB_E_dst);
		SDL_RenderCopy(Renderer, SGB_F, NULL, &SGB_F_dst);
		SDL_RenderCopy(Renderer, SGB_G, NULL, &SGB_G_dst);
		
		SDL_RenderCopy(Renderer, SGR_A, NULL, &SGR_A_dst);
		SDL_RenderCopy(Renderer, SGR_B, NULL, &SGR_B_dst);
		SDL_RenderCopy(Renderer, SGR_C, NULL, &SGR_C_dst);
		SDL_RenderCopy(Renderer, SGR_D, NULL, &SGR_D_dst);
		SDL_RenderCopy(Renderer, SGR_E, NULL, &SGR_E_dst);
		SDL_RenderCopy(Renderer, SGR_F, NULL, &SGR_F_dst);
		SDL_RenderCopy(Renderer, SGR_G, NULL, &SGR_G_dst);

		SDL_RenderCopy(Renderer, SGB_A2, NULL, &SGB_A2_dst);
		SDL_RenderCopy(Renderer, SGB_B2, NULL, &SGB_B2_dst);
		SDL_RenderCopy(Renderer, SGB_C2, NULL, &SGB_C2_dst);
		SDL_RenderCopy(Renderer, SGB_D2, NULL, &SGB_D2_dst);
		SDL_RenderCopy(Renderer, SGB_E2, NULL, &SGB_E2_dst);
		SDL_RenderCopy(Renderer, SGB_F2, NULL, &SGB_F2_dst);
		SDL_RenderCopy(Renderer, SGB_G2, NULL, &SGB_G2_dst);

		SDL_RenderCopy(Renderer, SGR_A2, NULL, &SGR_A2_dst);
		SDL_RenderCopy(Renderer, SGR_B2, NULL, &SGR_B2_dst);
		SDL_RenderCopy(Renderer, SGR_C2, NULL, &SGR_C2_dst);
		SDL_RenderCopy(Renderer, SGR_D2, NULL, &SGR_D2_dst);
		SDL_RenderCopy(Renderer, SGR_E2, NULL, &SGR_E2_dst);
		SDL_RenderCopy(Renderer, SGR_F2, NULL, &SGR_F2_dst);
		SDL_RenderCopy(Renderer, SGR_G2, NULL, &SGR_G2_dst);
		
		SDL_RenderCopy(Renderer, dash, NULL, &dash_dst);

		SDL_RenderCopy(Renderer, button, NULL, &button_dst);
		SDL_SetRenderDrawColor(Renderer, WHITE);
		SDL_RenderDrawRect(Renderer, &button_bounds_dst);
		SDL_RenderPresent(Renderer);

		//END   RENDERING
	}	//END   MAIN LOOP

	assets_out();
	exit_();
	return EXIT_SUCCESS;

}		//END   MAIN FUNCTION


//BEGIN FUNCTIONS
void assets_in(void)
{
	//BEGIN BG
	temp_surface = IMG_Load("./assets/gfx/bg2.bmp");
	bg = SDL_CreateTextureFromSurface(Renderer, temp_surface);
	SDL_QueryTexture(bg, NULL, NULL, &bg_dst.w, &bg_dst.h);
	ww = bg_dst.w;
	wh = bg_dst.h;
	bg_dst.x = 0;
	bg_dst.y = 0;
	//END 	BG

	//BEGIN BOUTON_PLAY
	temp_surface = IMG_Load("./assets/gfx/btn_Play.png");
	btn_play = SDL_CreateTextureFromSurface(Renderer, temp_surface);
	SDL_QueryTexture(btn_play, NULL, NULL, &btn_play_dst.w, &btn_play_dst.h);
	btn_play_dst.x = 400;
	btn_play_dst.y = 250;
	//END BOUTON_PLAY
	
	//BEGIN BLANC RGB
	temp_surface = IMG_Load("./assets/gfx/RGB_blanc.png");
	RGB_blanc = SDL_CreateTextureFromSurface(Renderer, temp_surface);
	SDL_QueryTexture(RGB_blanc, NULL, NULL, &RGB_blanc_dst.w, &RGB_blanc_dst.h);
	RGB_blanc_dst.x = 400;
	RGB_blanc_dst.y = 50;
	//END BLANC RGB

	//BEGIN BLANC
	temp_surface = IMG_Load("./assets/gfx/blanc.png");
	blanc = SDL_CreateTextureFromSurface(Renderer, temp_surface);
	SDL_QueryTexture(blanc, NULL, NULL, &blanc_dst.w, &blanc_dst.h);
	blanc_dst.x = 500;
	blanc_dst.y = 50;
	//END BLANC

	//BEGIN BLANC1
	temp_surface = IMG_Load("./assets/gfx/blanc1.png");
	blanc1 = SDL_CreateTextureFromSurface(Renderer, temp_surface);
	SDL_QueryTexture(blanc1, NULL, NULL, &blanc1_dst.w, &blanc1_dst.h);
	blanc1_dst.x = 500;
	blanc1_dst.y = 105;
	//END BLANC1

	//BEGIN BLANC2
	temp_surface = IMG_Load("./assets/gfx/blanc2.png");
	blanc2 = SDL_CreateTextureFromSurface(Renderer, temp_surface);
	SDL_QueryTexture(blanc2, NULL, NULL, &blanc2_dst.w, &blanc2_dst.h);
	blanc2_dst.x = 500;
	blanc2_dst.y = 160;
	//END BLANC2

	//SGB_A
	temp_surface = IMG_Load("./assets/gfx/SGB_A.png");
	SGB_A = SDL_CreateTextureFromSurface(Renderer, temp_surface);
	SDL_QueryTexture(SGB_A, NULL, NULL, &SGB_A_dst.w, &SGB_A_dst.h);
	SGB_A_dst.x = 270;
	SGB_A_dst.y = 196;
	//END SGB_A

	//SGB
	temp_surface = IMG_Load("./assets/gfx/SGB_B.png");
	SGB_B = SDL_CreateTextureFromSurface(Renderer, temp_surface);
	SDL_QueryTexture(SGB_B, NULL, NULL, &SGB_B_dst.w, &SGB_B_dst.h);
	SGB_B_dst.x = 320;
	SGB_B_dst.y = 198;
	//END SGB_B

	//SGB_C
	temp_surface = IMG_Load("./assets/gfx/SGB_C.png");
	SGB_C = SDL_CreateTextureFromSurface(Renderer, temp_surface);
	SDL_QueryTexture(SGB_C, NULL, NULL, &SGB_C_dst.w, &SGB_C_dst.h);
	SGB_C_dst.x = 320;
	SGB_C_dst.y = 249;
	//END SGB_C

	//SGB_D
	temp_surface = IMG_Load("./assets/gfx/SGB_D.png");
	SGB_D = SDL_CreateTextureFromSurface(Renderer, temp_surface);
	SDL_QueryTexture(SGB_D, NULL, NULL, &SGB_D_dst.w, &SGB_D_dst.h);
	SGB_D_dst.x = 270;
	SGB_D_dst.y = 300;
	//END SBG_D

	//SGB_E
	temp_surface = IMG_Load("./assets/gfx/SGB_E.png");
	SGB_E = SDL_CreateTextureFromSurface(Renderer, temp_surface);
	SDL_QueryTexture(SGB_E, NULL, NULL, &SGB_E_dst.w, &SGB_E_dst.h);
	SGB_E_dst.x = 267;
	SGB_E_dst.y = 249;
	//END SGB_E

	//SGB_F
	temp_surface = IMG_Load("./assets/gfx/SGB_F.png");
	SGB_F = SDL_CreateTextureFromSurface(Renderer, temp_surface);
	SDL_QueryTexture(SGB_F, NULL, NULL, &SGB_F_dst.w, &SGB_F_dst.h);
	SGB_F_dst.x = 267;
	SGB_F_dst.y = 198;
	//END SBG_F

	//SGB_G
	temp_surface = IMG_Load("./assets/gfx/SGB_G.png");
	SGB_G = SDL_CreateTextureFromSurface(Renderer, temp_surface);
	SDL_QueryTexture(SGB_G, NULL, NULL, &SGB_G_dst.w, &SGB_G_dst.h);
	SGB_G_dst.x = 270;
	SGB_G_dst.y = 248;
	//END SGB_G
	
	//DASH
	temp_surface = IMG_Load("./assets/gfx/dash.png");
	dash = SDL_CreateTextureFromSurface(Renderer, temp_surface);
	SDL_QueryTexture(dash, NULL, NULL, &dash_dst.w, &dash_dst.h);
	dash_dst.x = 228;
	dash_dst.y = 248;
	//END DASH

	//SGB_A2
	temp_surface = IMG_Load("./assets/gfx/SGB_A2.png");
	SGB_A2 = SDL_CreateTextureFromSurface(Renderer, temp_surface);
	SDL_QueryTexture(SGB_A2, NULL, NULL, &SGB_A2_dst.w, &SGB_A2_dst.h);
	SGB_A2_dst.x = 150;
	SGB_A2_dst.y = 196;
	//END SGB_A2

	//SGB2
	temp_surface = IMG_Load("./assets/gfx/SGB_B2.png");
	SGB_B2 = SDL_CreateTextureFromSurface(Renderer, temp_surface);
	SDL_QueryTexture(SGB_B2, NULL, NULL, &SGB_B2_dst.w, &SGB_B2_dst.h);
	SGB_B2_dst.x = 200;
	SGB_B2_dst.y = 198;
	//END SGB_B2

	//SGB_C2
	temp_surface = IMG_Load("./assets/gfx/SGB_C2.png");
	SGB_C2 = SDL_CreateTextureFromSurface(Renderer, temp_surface);
	SDL_QueryTexture(SGB_C2, NULL, NULL, &SGB_C2_dst.w, &SGB_C2_dst.h);
	SGB_C2_dst.x = 200;
	SGB_C2_dst.y = 249;
	//END SGB_C2

	//SGB_D2
	temp_surface = IMG_Load("./assets/gfx/SGB_D2.png");
	SGB_D2 = SDL_CreateTextureFromSurface(Renderer, temp_surface);
	SDL_QueryTexture(SGB_D2, NULL, NULL, &SGB_D2_dst.w, &SGB_D2_dst.h);
	SGB_D2_dst.x = 150;
	SGB_D2_dst.y = 300;
	//END SBG_D2

	//SGB_E2
	temp_surface = IMG_Load("./assets/gfx/SGB_E2.png");
	SGB_E2 = SDL_CreateTextureFromSurface(Renderer, temp_surface);
	SDL_QueryTexture(SGB_E2, NULL, NULL, &SGB_E2_dst.w, &SGB_E2_dst.h);
	SGB_E2_dst.x = 147;
	SGB_E2_dst.y = 249;
	//END SGB_E2

	//SGB_F2
	temp_surface = IMG_Load("./assets/gfx/SGB_F2.png");
	SGB_F2 = SDL_CreateTextureFromSurface(Renderer, temp_surface);
	SDL_QueryTexture(SGB_F2, NULL, NULL, &SGB_F2_dst.w, &SGB_F2_dst.h);
	SGB_F2_dst.x = 147;
	SGB_F2_dst.y = 198;
	//END SBG_F2

	//SGB_G2
	temp_surface = IMG_Load("./assets/gfx/SGB_G2.png");
	SGB_G2 = SDL_CreateTextureFromSurface(Renderer, temp_surface);
	SDL_QueryTexture(SGB_G2, NULL, NULL, &SGB_G2_dst.w, &SGB_G2_dst.h);
	SGB_G2_dst.x = 150;
	SGB_G2_dst.y = 248;
	//END SGB_G2
	
	//BEGIN BUTTON
	temp_surface = IMG_Load("./assets/gfx/button.png");
	button = SDL_CreateTextureFromSurface(Renderer, temp_surface);
	SDL_QueryTexture(button, NULL, NULL, &button_dst.w, &button_dst.h);
	button_dst.x = 50;
	button_dst.y = (wh / 2);
	//END 	BUTTON

	//BEGIN BOUNDS
	button_bounds_dst.w = button_dst.w;
	button_bounds_dst.h = button_dst.h * 5;
	button_bounds_dst.x = button_dst.x;
	button_bounds_dst.y = button_dst.y - (2.5 * button_dst.h);
	//END 	BOUNDS
}


void etatLED(void)					//fonction etatLED qui va gerer les LEDs en fonction de la valeur 'player_goal' recu par pong
{

	if (player_goal == 'u')														//si la valeur reçu par pong est le caractere 'u' alors 
	{

		temp_surface = IMG_Load("./assets/gfx/rouge.png");						//j'allume la premiere LED
		blanc = SDL_CreateTextureFromSurface(Renderer, temp_surface);

		temp_surface = IMG_Load("./assets/gfx/RGB_vert.png");					//j'allume la RGB en vert
		RGB_blanc = SDL_CreateTextureFromSurface(Renderer, temp_surface);

		temp_surface = IMG_Load("./assets/gfx/SGR_B.png");						//j'affiche 1 avec mon 7 segments  
		SGB_B = SDL_CreateTextureFromSurface(Renderer, temp_surface);
		temp_surface = IMG_Load("./assets/gfx/SGR_C.png");
		SGB_C = SDL_CreateTextureFromSurface(Renderer, temp_surface);

	}


	if (player_goal == 'd')
	{

		temp_surface = IMG_Load("./assets/gfx/rouge1.png");
		blanc1 = SDL_CreateTextureFromSurface(Renderer, temp_surface);

		temp_surface = IMG_Load("./assets/gfx/RGB_vert.png");
		RGB_blanc = SDL_CreateTextureFromSurface(Renderer, temp_surface);

		temp_surface = IMG_Load("./assets/gfx/SGB_C.png");
		SGB_C = SDL_CreateTextureFromSurface(Renderer, temp_surface);

		temp_surface = IMG_Load("./assets/gfx/SGR_A.png");
		SGB_A = SDL_CreateTextureFromSurface(Renderer, temp_surface);
		temp_surface = IMG_Load("./assets/gfx/SGR_B.png");
		SGB_B = SDL_CreateTextureFromSurface(Renderer, temp_surface);
		temp_surface = IMG_Load("./assets/gfx/SGR_G.png");
		SGB_G = SDL_CreateTextureFromSurface(Renderer, temp_surface);
		temp_surface = IMG_Load("./assets/gfx/SGR_E.png");
		SGB_E = SDL_CreateTextureFromSurface(Renderer, temp_surface);
		temp_surface = IMG_Load("./assets/gfx/SGR_D.png");
		SGB_D = SDL_CreateTextureFromSurface(Renderer, temp_surface);

	}

	
	if (player_goal == 't')
	{

		temp_surface = IMG_Load("./assets/gfx/rouge2.png");
		blanc2 = SDL_CreateTextureFromSurface(Renderer, temp_surface);

		temp_surface = IMG_Load("./assets/gfx/RGB_vert.png");
		RGB_blanc = SDL_CreateTextureFromSurface(Renderer, temp_surface);

		temp_surface = IMG_Load("./assets/gfx/SGB_E.png");
		SGB_E = SDL_CreateTextureFromSurface(Renderer, temp_surface);

		temp_surface = IMG_Load("./assets/gfx/SGR_A.png");
		SGB_A = SDL_CreateTextureFromSurface(Renderer, temp_surface);
		temp_surface = IMG_Load("./assets/gfx/SGR_B.png");
		SGB_B = SDL_CreateTextureFromSurface(Renderer, temp_surface);
		temp_surface = IMG_Load("./assets/gfx/SGR_G.png");
		SGB_G = SDL_CreateTextureFromSurface(Renderer, temp_surface);
		temp_surface = IMG_Load("./assets/gfx/SGR_C.png");
		SGB_C = SDL_CreateTextureFromSurface(Renderer, temp_surface);
		temp_surface = IMG_Load("./assets/gfx/SGR_D.png");
		SGB_D = SDL_CreateTextureFromSurface(Renderer, temp_surface);

	}


	if (player_goal == 'q')														//si la valeur reçu par pong est le caractere 'q' alors 
	{

		temp_surface = IMG_Load("./assets/gfx/RGB_rouge.png");					//allumer la RGB en rouge car but IA
		RGB_blanc = SDL_CreateTextureFromSurface(Renderer, temp_surface);

		temp_surface = IMG_Load("./assets/gfx/SGR_B2.png");						//afficher 1 sur le 7 segments IA
		SGB_B2 = SDL_CreateTextureFromSurface(Renderer, temp_surface);
		temp_surface = IMG_Load("./assets/gfx/SGR_C2.png");
		SGB_C2 = SDL_CreateTextureFromSurface(Renderer, temp_surface);

	}

	
	if (player_goal == 'c')
	{

		temp_surface = IMG_Load("./assets/gfx/RGB_rouge.png");
		RGB_blanc = SDL_CreateTextureFromSurface(Renderer, temp_surface);

		temp_surface = IMG_Load("./assets/gfx/SGB_C2.png");
		SGB_C2 = SDL_CreateTextureFromSurface(Renderer, temp_surface);

		temp_surface = IMG_Load("./assets/gfx/SGR_A2.png");
		SGB_A2 = SDL_CreateTextureFromSurface(Renderer, temp_surface);
		temp_surface = IMG_Load("./assets/gfx/SGR_B2.png");
		SGB_B2 = SDL_CreateTextureFromSurface(Renderer, temp_surface);
		temp_surface = IMG_Load("./assets/gfx/SGR_G2.png");
		SGB_G2 = SDL_CreateTextureFromSurface(Renderer, temp_surface);
		temp_surface = IMG_Load("./assets/gfx/SGR_E2.png");
		SGB_E2 = SDL_CreateTextureFromSurface(Renderer, temp_surface);
		temp_surface = IMG_Load("./assets/gfx/SGR_D2.png");
		SGB_D2 = SDL_CreateTextureFromSurface(Renderer, temp_surface);

	}

	
	if (player_goal == 's')
	{

		temp_surface = IMG_Load("./assets/gfx/RGB_rouge.png");
		RGB_blanc = SDL_CreateTextureFromSurface(Renderer, temp_surface);

		temp_surface = IMG_Load("./assets/gfx/SGB_E2.png");
		SGB_E2 = SDL_CreateTextureFromSurface(Renderer, temp_surface);

		temp_surface = IMG_Load("./assets/gfx/SGR_A.png");
		SGB_A2 = SDL_CreateTextureFromSurface(Renderer, temp_surface);
		temp_surface = IMG_Load("./assets/gfx/SGR_B2.png");
		SGB_B2 = SDL_CreateTextureFromSurface(Renderer, temp_surface);
		temp_surface = IMG_Load("./assets/gfx/SGR_G2.png");
		SGB_G2 = SDL_CreateTextureFromSurface(Renderer, temp_surface);
		temp_surface = IMG_Load("./assets/gfx/SGR_C2.png");
		SGB_C2 = SDL_CreateTextureFromSurface(Renderer, temp_surface);
		temp_surface = IMG_Load("./assets/gfx/SGR_D2.png");
		SGB_D2 = SDL_CreateTextureFromSurface(Renderer, temp_surface);

	}
	
}

void assets_out(void)
{
	SDL_DestroyTexture(bg);
	SDL_DestroyTexture(button);
}
