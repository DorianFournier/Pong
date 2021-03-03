
/*
PONG PROJECT
6 May 2020
Modified by Dorian FOURNIER
*/
 

#include <SDL2/SDL.h>	
#include <stdlib.h>
#include <stdio.h>
#include "STM8.h"
#include <SDL2/SDL_mixer.h>

#define Applause 0									//definition de symbole pour l'applause 
#define Boo    1									//definition de symbole pour le boooo
#define musique_de_fond 2							//definition de symbole pour la musique de fond

#define Attente 5000

void fctmusic(unsigned char choix_musique);        
void fctsong(unsigned char choix_musique);			//prototype des 3 fonctions musique / son
void fctsong2(unsigned char choix_musique);

int init(int w, int h, int argc, char* args[]); 
Uint32 next_game_tick = 0;

#define SCREEN_WIDTH 640	//window height
#define SCREEN_HEIGHT 480	//window width

// ADD NNC SERIAL
#define SERIAL_PORT_BUFFER_LENGTH 20
//#define SERIAL_PORT "COM5"
#define SERIAL_PORT "CNCB0"							//definition du port CNCB0 pour notre SERIAL_PORT

//int init(int w, int h, int argc, char* args[]);
//Uint32 next_game_tick = 0;

//initialisation de plusieurs variables 

int etatmusic;
char player_goal = 0; 
char z = 1; 
char reinitialisation = 10;
char p = 0;


void fctmusic(unsigned char choix_musique) {

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
	{
		printf("%s", Mix_GetError());
	}

	Mix_VolumeMusic(MIX_MAX_VOLUME / 2);	 //Mettre le volume a la moitie
	Mix_Music* musique;						 //Création d'un pointeur de type Mix_Music
	musique = Mix_LoadMUS("Ehr.wav");		 //Chargement de la musique
	Mix_PlayMusic(musique, -1);				 //Jouer infiniment la musique
}


void fctsong(unsigned char choix_musique) {

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
	{
		printf("%s", Mix_GetError());
	}

	Mix_VolumeMusic(MIX_MAX_VOLUME / 2);	//Mettre le volume a la moitie
	Mix_Music* musique2;					//Création d'un pointeur de type Mix_Music
	musique2 = Mix_LoadMUS("App.wav");		//Chargement de la musique
	Mix_PlayMusic(musique2, -1);			//Jouer infiniment la musique
	SDL_Delay(5000);
	Mix_HaltMusic();
}


void fctsong2(unsigned char choix_musique) {

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
	{
		printf("%s", Mix_GetError());
	}

	Mix_VolumeMusic(MIX_MAX_VOLUME / 2);	//Mettre le volume a la moitie
	Mix_Music* musique3;					//Création d'un pointeur de type Mix_Music
	musique3 = Mix_LoadMUS("Boo.wav");		//Chargement de la musique
	Mix_PlayMusic(musique3, -1);		    //Jouer infiniment la musique
	SDL_Delay(5000);
	Mix_HaltMusic();
}

typedef struct ball_s {

	int x, y;			//position on the screen 
	int w, h;		    //ball width and height
	int dx, dy;	        //movement vector 

} ball_t;

typedef struct paddle {

	int x, y;
	int w, h;

} paddle_t;

// Program globals
static ball_t ball;
static paddle_t paddle[2];
int score[2];
int width, height;		     //used if fullscreen

SDL_Window* window = NULL;	 //The window we'll be rendering to
SDL_Renderer* renderer;		 //The renderer SDL will use to draw to the screen

//surfaces
static SDL_Surface* screen;
static SDL_Surface* title;
static SDL_Surface* numbermap;
static SDL_Surface* end;

//textures
SDL_Texture* screen_texture;

//inisilise starting position and sizes of game elemements
static void init_game() {						//fonction initialisation de la partie 
	
	z = 1;

	ball.x = screen->w / 2;						//placer la balle à la moitié de la largeur de l'ecran 
	ball.y = screen->h / 2;						//placer la balle à la moitié de la hauteur de l'ecran 
	ball.w = 10;								//largeur de la balle
	ball.h = 10;								//hauteur de la balle
	ball.dy = 1;
	ball.dx = 1;

	paddle[0].x = 20;							//initialisation du paddle IA
	paddle[0].y = screen->h / 2 - 50;
	paddle[0].w = 10;
	paddle[0].h = 50;

	paddle[1].x = screen->w - 20 - 10;			//initialisation du paddle J1
	paddle[1].y = screen->h / 2 - 50;
	paddle[1].w = 10;
	paddle[1].h = 50;
}

int check_score() {

	int i;
	
	//loop through player scores
	for (i = 0; i <= 1; i++) 
	{
									
		if (score[i] == 3)			//regarder si le score max est atteint par un joueur
		{							//score max = 3

			if (reinitialisation == 10) 
			{
				score[0] = 0;			//si le score max est atteint par J1 ou IA alors 
				score[1] = 0;           //on reinitialise les deux scores à 0 
			}
									
			if (i == 0) 
			{			
				return 1;			//return 1 si IA a atteint la limite
			}
			else 
			{					
				return 2;			//si non return 2 si J1 a atteint la limite
			}
			
		}
	}

	return 0;						//return 0 si personne n'a atteint le score max
}

//if return value is 1 collision occured. if return is 0, no collision.
int check_collision(ball_t a, paddle_t b) {

	int left_a, left_b;
	int right_a, right_b;
	int top_a, top_b;
	int bottom_a, bottom_b;

	left_a = a.x;
	right_a = a.x + a.w;
	top_a = a.y;
	bottom_a = a.y + a.h;

	left_b = b.x;
	right_b = b.x + b.w;
	top_b = b.y;
	bottom_b = b.y + b.h;


	if (left_a > right_b) 
	{
		return 0;
	}

	if (right_a < left_b) 
	{
		return 0;
	}

	if (top_a > bottom_b) 
	{
		return 0;
	}

	if (bottom_a < top_b) 
	{
		return 0;
	}

	return 1;
}

/* This routine moves each ball by its motion vector. */
static void move_ball() {

	if (player_goal == 0 || player_goal == 't' || player_goal == 's') 
	{
		player_goal = 0;
	}

	/* Move the ball by its motion vector. */
	ball.x += ball.dx;
	ball.y += ball.dy;

	/* Turn the ball around if it hits the edge of the screen. */
	if (ball.x < 0)													//si la balle passe deriere le slider IA : (but J1) 
	{												

		score[1] += 1;												//incrementer de 1 le score J1
				
								
		if (score[1] == 1)
		{ 
			player_goal = 'u';
		}							
		if (score[1] == 2) 
		{ 
			player_goal = 'd';
		}							
		if (score[1] == 3) 
		{ 
			player_goal = 't'; 
			reinitialisation = 20; 
			
		}						 
			
		init_game();												
	}

	if (ball.x > screen->w - 10)									
	{					

		score[0] += 1;												
																		
		if (score[0] == 1)
		{ 
			player_goal = 'q';
		}							
		
		if (score[0] == 2) 
		{
			player_goal = 'c';
		}							
		if (score[0] == 3) 
		{
			player_goal = 's';
			reinitialisation = 20; 
		}							
		
		init_game();
	}

	STM8_write(&player_goal, 1);

	if (ball.y < 0 || ball.y > screen->h - 10) 
	{

		ball.dy = -ball.dy;
	}

	//check for collision with the paddle
	int i;

	for (i = 0; i < 2; i++) {

		int c = check_collision(ball, paddle[i]);

		//collision detected	
		if (c == 1)
		{

			//ball moving left
			if (ball.dx < 0)
			{
				ball.dx -= 1;

				//ball moving right
			}
			else
			{
				ball.dx += 1;
			}

			//change ball direction
			ball.dx = -ball.dx;

			//change ball angle based on where on the paddle it hit
			int hit_pos = (paddle[i].y + paddle[i].h) - ball.y;

			if (hit_pos >= 0 && hit_pos < 7) 
			{
				ball.dy = 4;
			}

			if (hit_pos >= 7 && hit_pos < 14)
			{
				ball.dy = 3;
			}

			if (hit_pos >= 14 && hit_pos < 21)
			{
				ball.dy = 2;
			}

			if (hit_pos >= 21 && hit_pos < 28)
			{
				ball.dy = 1;
			}

			if (hit_pos >= 28 && hit_pos < 32)
			{
				ball.dy = 0;
			}

			if (hit_pos >= 32 && hit_pos < 39)
			{
				ball.dy = -1;
			}

			if (hit_pos >= 39 && hit_pos < 46)
			{
				ball.dy = -2;
			}

			if (hit_pos >= 46 && hit_pos < 53) 
			{
				ball.dy = -3;
			}

			if (hit_pos >= 53 && hit_pos <= 60) 
			{
				ball.dy = -4;
			}

			//ball moving right
			if (ball.dx > 0)
			{

				//teleport ball to avoid mutli collision glitch
				if (ball.x < 30) 
				{
					ball.x = 30;
				}

				//ball moving left
			}
			else 
			{
				//teleport ball to avoid mutli collision glitch
				if (ball.x > 600)
				{
					ball.x = 600;
				}
			}
		}
	}
}

static void move_paddle_ai() {

	int center = paddle[0].y + 25;
	int screen_center = screen->h / 2 - 25;
	int ball_speed = ball.dy;

	if (ball_speed < 0) 
	{
		ball_speed = -ball_speed;
	}

	//ball moving right
	if (ball.dx > 0) 
	{
		//return to center position
		if (center < screen_center)
		{
			paddle[0].y += ball_speed;
		}
		else 
		{
			paddle[0].y -= ball_speed;
		}
		//ball moving left
	}
	else
	{
		//ball moving down
		if (ball.dy > 0)
		{
			if (ball.y > center)
			{
				paddle[0].y += ball_speed;
			}
			else 
			{
				paddle[0].y -= ball_speed;
			}
		}
		//ball moving up
		if (ball.dy < 0)
		{
			if (ball.y < center)
			{
				paddle[0].y -= ball_speed;
			}
			else 
			{
				paddle[0].y += ball_speed;
			}
		}

		//ball moving stright across
		if (ball.dy == 0) 
		{
			if (ball.y < center)
			{
				paddle[0].y -= 5;
			}
			else 
			{
				paddle[0].y += 5;
			}
		}
	}
}

static void move_paddle(int d) {

	// if the down arrow is pressed move paddle down
	if (d == 0) 
	{

		if (paddle[1].y >= screen->h - paddle[1].h) 
		{
			paddle[1].y = screen->h - paddle[1].h;
		}
		else 
		{
			paddle[1].y += 5;
		}
	}

	// if the up arrow is pressed move paddle up
	if (d == 1) 
	{
		if (paddle[1].y <= 0)
		{
			paddle[1].y = 0;
		}
		else 
		{
			paddle[1].y -= 5;
		}
	}
}

//ADD NNC pour posisionner le PADDLE
static void place_paddle(int d) {

	paddle[1].y = (screen->h - paddle[1].h) * d/100;

}

static void draw_game_over(int p) {

	SDL_Rect p1;
	SDL_Rect p2;
	SDL_Rect cpu;
	SDL_Rect dest;

	p1.x = 0;
	p1.y = 0;
	p1.w = end->w;
	p1.h = 75;

	p2.x = 0;
	p2.y = 75;
	p2.w = end->w;
	p2.h = 75;

	cpu.x = 0;
	cpu.y = 150;
	cpu.w = end->w;
	cpu.h = 75;

	dest.x = (screen->w / 2) - (end->w / 2);
	dest.y = (screen->h / 2) - (75 / 2);
	dest.w = end->w;
	dest.h = 75;

	switch (p) {

	case 1:
		SDL_BlitSurface(end, &p1, screen, &dest);
		break;
	case 2:
		SDL_BlitSurface(end, &p2, screen, &dest);
		break;
	default:
		SDL_BlitSurface(end, &cpu, screen, &dest);
	}

}

static void draw_menu() {

	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = title->w;
	src.h = title->h;

	dest.x = (screen->w / 2) - (src.w / 2);
	dest.y = (screen->h / 2) - (src.h / 2);
	dest.w = title->w;
	dest.h = title->h;

	SDL_BlitSurface(title, &src, screen, &dest);
}


static void draw_background() {

	SDL_Rect src;

	//draw bg with net
	src.x = 0;
	src.y = 0;
	src.w = screen->w;
	src.h = screen->h;

	//draw the backgorund
	int r = SDL_FillRect(screen,&src,0);

	if (r !=0)
	{
		printf("fill rectangle faliled in func draw_background()");
	}
}

static void draw_net() {

	SDL_Rect net;

	net.x = screen->w / 2;
	net.y = 20;
	net.w = 5;
	net.h = 15;

	//draw the net
	int i, r;

	for (i = 0; i < 15; i++) {

		r = SDL_FillRect(screen, &net, 0xffffffff);

		if (r != 0) 
		{
			printf("fill rectangle faliled in func draw_net()");
		}

		net.y = net.y + 30;
	}
}

static void draw_ball() {

	SDL_Rect src;
	
	src.x = ball.x;
	src.y = ball.y;
	src.w = ball.w;
	src.h = ball.h;

	int r = SDL_FillRect(screen, &src, 0xffffffff);

	if (r != 0) 
	{
		printf("fill rectangle faliled in func drawball()");
	}
}

static void draw_paddle() {

	SDL_Rect src;
	int i;

	for (i = 0; i < 2; i++) {

		src.x = paddle[i].x;
		src.y = paddle[i].y;
		src.w = paddle[i].w;
		src.h = paddle[i].h;

		int r = SDL_FillRect(screen, &src, 0xffffffff);
	}
}

static void draw_player_0_score() {

	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = 64;
	src.h = 64;

	dest.x = (screen->w / 2) - src.w - 12; //12 is just padding spacing
	dest.y = 0;
	dest.w = 64;
	dest.h = 64;

	if (score[0] > 0 && score[0] < 10) 
	{
		src.x += src.w * score[0];
	}

	SDL_BlitSurface(numbermap, &src, screen, &dest);
}

static void draw_player_1_score() {

	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = 64;
	src.h = 64;

	dest.x = (screen->w / 2) + 12;
	dest.y = 0;
	dest.w = 64;
	dest.h = 64;

	if (score[1] > 0 && score[1] < 10)
	{
		src.x += src.w * score[1];
	}

	SDL_BlitSurface(numbermap, &src, screen, &dest);
}

int main(int argc, char* args[]) {
	
	//SDL Window setup
	if (init(SCREEN_WIDTH, SCREEN_HEIGHT, argc, args) == 1)
	{
		return 0;
	}

	// ADD NNC POUR SERIAL

	char serialPortBuffer[SERIAL_PORT_BUFFER_LENGTH] = { 0 };
	int serialPortReadDataLength = 0;

	if (!STM8_connect(SERIAL_PORT, 115200))
		exit(EXIT_FAILURE);
	
	//FIN ADD NNC POUR SERIAL

	next_game_tick = SDL_GetTicks();
	SDL_GetWindowSize(window, &width, &height);

	int sleep = 0;
	int quit = 0;
	char state = 0;
	int r = 0;
	
	init_game();				// Initialize the ball position data. 
	
	//render loop
	while (quit == 0) {
		p = 0;

		//check for new events every frame
		SDL_PumpEvents();

		const Uint8* keystate = SDL_GetKeyboardState(NULL);

		if (keystate[SDL_SCANCODE_ESCAPE])
		{
			quit = 1;
		}

		//AJOUT NNC ET DFR

		serialPortReadDataLength = STM8_read(serialPortBuffer, SERIAL_PORT_BUFFER_LENGTH);
		if (serialPortReadDataLength == SERIAL_PORT_BUFFER_LENGTH)  serialPortReadDataLength = SERIAL_PORT_BUFFER_LENGTH - 1;
		serialPortBuffer[serialPortReadDataLength] = 0;

		if (serialPortReadDataLength > 0)
		{
			if (serialPortBuffer[0] > 102)					//si la valeur recu de STM8 est superieur a 100
			{
				switch (serialPortBuffer[0])
				{

				case 104:									//'104' recu donc state = 0
					state = 0;
					break;

				case 105 : 
					state = 1;								//'105' recu donc c'est que la barr espace à ete appuyee cote STM8 donc pongon lance la partie car state = 1
					break;

				case 106 :
					fctmusic(musique_de_fond);				//'106' recu donc c'est que le bouton musique à ete appuyee cote STM8 donc pong lance la musique
					break;

				case 107 :									//'107' recu donc c'est que le bouton musique à ete une nouvelle fois appuyee cote STM8 donc pong eteint la musique
					Mix_HaltMusic();
					break;

				default:	break; 
				}
				
			}
			else
			{
				place_paddle(serialPortBuffer[0]);			//les autres valeurs sont pour le deplacement du paddle
			}
		}
		//FIN AJOUT NNC ET DFR
		
		if (keystate[SDL_SCANCODE_DOWN]) 
		{
			move_paddle(0);
		}

		if (keystate[SDL_SCANCODE_UP]) 
		{
			move_paddle(1);
		}

		//draw background
		SDL_RenderClear(renderer);
		SDL_FillRect(screen, NULL, 0x000000ff);

		//display main menu
  		if (state == 0)
		{
			draw_menu();
		}
		else if (state == 2) 
		{
			if (r == 1) 
			{

				draw_game_over(3);
				z += z;
				if (z == 2)
				{
					fctsong2(Boo);
					for (p = 0; p < 2; p++)
					{
						reinitialisation = 10;
					}
				}
			}

			else 
			{

				draw_game_over(r);
				z += z;
				if (z == 2)
				{
					fctsong(Applause);
					for (p = 0; p < 2; p++) 
					{
						reinitialisation = 10;
					}
				}
			}
		}

		else if (state == 1) 
		{
			//check score
			r = check_score();

			//if either player wins, change to game over state
			if (r == 1) 
			{
				state = 2;
			}

        	else if (r == 2)
			{
				state = 2;
			}

			//paddle ai movement
			move_paddle_ai();
			
			//Move the balls for the next frame. 
			move_ball();

			//draw net
			draw_net();

			//draw paddles
			draw_paddle();
			
			//* Put the ball on the screen.
			draw_ball();

			//draw the score
			draw_player_0_score();

			//draw the score
			draw_player_1_score();
		}

		SDL_UpdateTexture(screen_texture, NULL, screen->pixels, screen->w * sizeof(Uint32));
		SDL_RenderCopy(renderer, screen_texture, NULL, NULL);

		//draw to the display
		SDL_RenderPresent(renderer);

		//time it takes to render  frame in milliseconds
		next_game_tick += 1000 / 60;
		sleep = next_game_tick - SDL_GetTicks();

		if (sleep >= 0) 
		{
			SDL_Delay(sleep);
		}
		p = 1; 
	}

	//free loaded images
	SDL_FreeSurface(screen);
	SDL_FreeSurface(title);
	SDL_FreeSurface(numbermap);
	SDL_FreeSurface(end);

	//free renderer and all textures used with it
	SDL_DestroyRenderer(renderer);

	//Destroy window 
	SDL_DestroyWindow(window);

	//Quit SDL subsystems 
	SDL_Quit();

	return 0;

}

int init(int width, int height, int argc, char* args[]) {

	//Initialize SDL
	if (SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());

		return 1;
	}

	int i;

	for (i = 0; i < argc; i++) {

		//Create window	
		if (strcmp(args[i], "-f"))
		{
			SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer);
		}
		else 
		{
			SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &renderer);
		}
	}

	if (window == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());

		return 1;
	}

	//create the screen sruface where all the elemnts will be drawn onto (ball, paddles, net etc)
	screen = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);

	if (screen == NULL)
	{
		printf("Could not create the screen surfce! SDL_Error: %s\n", SDL_GetError());

		return 1;
	}

	//create the screen texture to render the screen surface to the actual display
	screen_texture = SDL_CreateTextureFromSurface(renderer, screen);

	if (screen_texture == NULL)
	{
		printf("Could not create the screen_texture! SDL_Error: %s\n", SDL_GetError());

		return 1;
	}

	//Load the title image
	title = SDL_LoadBMP("title.bmp");

	if (title == NULL) 
	{
		printf("Could not Load title image! SDL_Error: %s\n", SDL_GetError());

		return 1;
	}

	//Load the numbermap image
	numbermap = SDL_LoadBMP("numbermap.bmp");

	if (numbermap == NULL) 
	{
		printf("Could not Load numbermap image! SDL_Error: %s\n", SDL_GetError());

		return 1;
	}

	//Load the gameover image
	end = SDL_LoadBMP("gameover.bmp");

	if (end == NULL) 
	{
		printf("Could not Load title image! SDL_Error: %s\n", SDL_GetError());

		return 1;
	}

	// Set the title colourkey. 
	Uint32 colorkey = SDL_MapRGB(title->format, 255, 0, 255);
	SDL_SetColorKey(title, SDL_TRUE, colorkey);
	SDL_SetColorKey(numbermap, SDL_TRUE, colorkey);

	return 0;
}

