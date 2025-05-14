//--------------------------------------------------------
// Application demonstrator: SNAKE game
//--------------------------------------------------------


#include "EDK_CM0.h" 
#include "core_cm0.h"
#include "edk_driver.h"
#include "edk_api.h"
#include "modified_cactus.h"
#include "modified_dino.h"

#include <stdio.h>			

//Game region
#define left_boundary 5
#define right_boundary 96
#define top_boundary 5
#define bottom_boundary 116
#define boundary_thick 1

#define DINO_COLOR 			 (RED)

#define DINO_X           (left_boundary + 10)
			     
#define GROUND_Y         (bottom_boundary - 40)

#define GROUND_Y_OBS     (bottom_boundary - 32)


//Global variables
static int i;
static char key;
static int score;
static int pause;

int switches;

int jump_pos[23] = {0,4,8,12,16,20,24,28,32,36,40,44,40,36,32,28,24,20,16,12,8,4,0};
int dino_moved=1;
int jump_counter = 0;
int jumping = 0;

static int dinospeed;
static int obsspeed;
static int speed_table[14]={20,30,40,50,60,70,80,90,100,110,120,130,140,150};
static int spawn_counter = 0;
static int rand_spawn;

struct Dino{
	int x[5]; //leave room for multiple rectangles
	int y[5];
	int speed;
	}dino;

struct Obstacle {
		int x[5];
		int y[5];
		int width;
		int height;
		int active;
		int speed;
		int pos_counter;
	} obs;	

//---------------------------------------------
// Game
//---------------------------------------------


void Game_Init(void)
{	
	//Draw a game region
	clear_screen();
	rectangle(left_boundary,top_boundary,right_boundary,top_boundary+boundary_thick,BLUE);
	rectangle(left_boundary,top_boundary,left_boundary+boundary_thick,bottom_boundary,BLUE);
	rectangle(left_boundary,bottom_boundary,right_boundary,bottom_boundary+boundary_thick,BLUE);
	rectangle(right_boundary,top_boundary,right_boundary+boundary_thick,bottom_boundary+boundary_thick,BLUE);	

	rectangle(left_boundary+boundary_thick, bottom_boundary - 19, right_boundary, bottom_boundary - 14, WHITE); //floor

	//Initialise data
	

	pause=0;
	
	//Print instructions
	printf("\n------- DINO Game --------");
  	printf("\nKeyboard w ........ JUMP");
  	printf("\nKeyboard space ...... pause");
	  printf("\nKeyboard r ...... reset");
	  printf("\nKeyboard q ...... quit");
  	printf("\n---------------------------");	

	printf("\nJump to Avoid the Objects");
	printf("\nPress Space to Pause");
	printf("\nChoose difficulty with");
	printf("\nNo Switch  EASY");
	printf("\nSwitch 1 NORMAL");
	printf("\nSwitch 2 HARD");
	printf("\nSwitch 3 EXTREME");
	printf("\nGame speeds up as you play");
  	printf("\n---------------------------");
	printf("\nPress any key to start\n");	
	while(KBHIT()==0);

	dino.x[0] = DINO_X;
	dino.y[0] = GROUND_Y;

	obs.y[0] = GROUND_Y_OBS; // Make sure GROUND_Y matches your floor
	obs.width =5;
	obs.height=12;
	obs.active=0;
	obs.pos_counter =0;
		
	dino.speed = 1;
	
	score=0;

	switches = GPIO_read();
	if(switches == 1){
		obs.speed = 4;
		printf("\n-------  NORMAL  -------");
	} else if (switches == 3){
		obs.speed = 5;
		printf("\n-------  HARD  -------");
	} else if (switches == 7){
		obs.speed = 6;
		printf("\n----- EXTREME -----");
	}else{
		obs.speed = 3;
		printf("\n-------  EASY  -------");
	}

	dinospeed=speed_table[dino.speed];		
	obsspeed=speed_table[obs.speed];		
	
	//Initialise timer (load value, prescaler value, mode value)
	timer_init((Timer_Load_Value_For_One_Sec/dinospeed),Timer_Prescaler,1);	
	timer_enable();

	timer2_init((Timer_Load_Value_For_One_Sec/obsspeed),Timer_Prescaler,1);	
	timer2_enable();
	draw_dino();

	NVIC_EnableIRQ(Timer_IRQn);			//start timing
	NVIC_EnableIRQ(UART_IRQn);	
	NVIC_EnableIRQ(Timer2_IRQn);
}


void Game_Close(void){
	clear_screen();
	score=0;
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");		//flush screen
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	NVIC_DisableIRQ(Timer_IRQn);			
	NVIC_DisableIRQ(UART_IRQn);	
	NVIC_DisableIRQ(Timer2_IRQn);	
}

	
int GameOver(void){
	char key;
	
	NVIC_DisableIRQ(UART_IRQn);
	NVIC_DisableIRQ(Timer_IRQn);
	NVIC_DisableIRQ(Timer2_IRQn);	
	printf("\nGame over\n");
	printf("\nYou Jumped Over %d\n",score);
	printf("\nPress 'q' to quit");
	printf("\nPress 'r' to replay");
	printf("\nChoose Your Difficulty\n");
	printf("\nDifficulty will update");
	printf("\nwhen you RESET");
	while(1){
		while(KBHIT()==0);
		key = UartGetc();
		if (key == RESET){
			return 1;
		}
		else if (key == QUIT){	
			return 0;
		}
		else
			printf("\nInvalid input");
	}
		
}

int draw_dino(void) {
	static int width = sizeof(modified_dino_rom[0]) / sizeof(modified_dino_rom[0][0]);  // columns
	static int height = sizeof(modified_dino_rom) / sizeof(modified_dino_rom[0]);       // rows

	int x_offset = dino.x[0];
	int y_offset = dino.y[0];
	int y, x;
	for (y = 0; y < height; ++y) {
		for (x = 0; x < width; ++x) {
			int color = (modified_dino_rom[y][x] == 0) ? BLACK : DINO_COLOR;
			VGA_plot_pixel(x + x_offset, y + y_offset, color);
		}
	}

	return 0;
}


int clear_dino(void) {
	static int width = sizeof(modified_dino_rom[0]) / sizeof(modified_dino_rom[0][0]);  // columns
	static int height = sizeof(modified_dino_rom) / sizeof(modified_dino_rom[0]);       // rows

	int x_offset = dino.x[0];
	int y_offset = dino.y[0];
	int y, x;
	for (y = 0; y < height; ++y) {
		for (x = 0; x < width; ++x) {
			VGA_plot_pixel(x + x_offset, y + y_offset, BLACK);
		}
	}

	return 0;
}

int draw_obstacle(void) {
	static int width = sizeof(modified_cactus_rom[0]) / sizeof(modified_cactus_rom[0][0]);  // columns
	static int height = sizeof(modified_cactus_rom) / sizeof(modified_cactus_rom[0]);       // rows

	int x_offset = obs.x[0];
	int y_offset = obs.y[0];
	int y, x;
	for (y = 0; y < height; ++y) {
		for (x = 0; x < width; ++x) {
			int color = (modified_cactus_rom[y][x] == 0) ? BLACK : GREEN;
			VGA_plot_pixel(x + x_offset, y + y_offset, color);
		}
	}

	return 0;
}

int clear_obstacle(void) {
	static int width = sizeof(modified_cactus_rom[0]) / sizeof(modified_cactus_rom[0][0]);  // columns
	static int height = sizeof(modified_cactus_rom) / sizeof(modified_cactus_rom[0]);       // rows

	int x_offset = obs.x[0];
	int y_offset = obs.y[0];
	int y, x;
	for (y = 0; y < height; ++y) {
		for (x = 0; x < width; ++x) {
			VGA_plot_pixel(x + x_offset, y + y_offset, BLACK);
		}
	}

	return 0;
}

//---------------------------------------------
// UART ISR -- used to input commands
//---------------------------------------------

void UART_ISR(void)
{	

  key=UartGetc();	
	
	//Only update the direction if the previous movement is finished
	if(dino_moved==1){			
		if(key==UP){
			jumping = 1;
            jump_counter = 0;
			dino_moved=0; 
		}
	}
		if(key==PAUSE){
				if(pause==0){
						pause=1;
						NVIC_DisableIRQ(Timer_IRQn);	
						NVIC_DisableIRQ(Timer2_IRQn);
						if(key==RESET){
							if (GameOver()==0){
								Game_Close();
								return;
							}
							else{
								Game_Init();
								return;
							}
						}
				}
				else{
						pause =0;
						NVIC_EnableIRQ(Timer_IRQn);
						NVIC_EnableIRQ(Timer2_IRQn);
				}
		}	
		
}
 

//---------------------------------------------
// TIMER ISR -- used to move the snake
//---------------------------------------------


void Timer_ISR(void) {
    if (pause == 0) {

        // Erase current Dino
        clear_dino();

        if (jumping) {
            jump_counter++;
            if (jump_counter < 23) {
                dino.y[0] = GROUND_Y - jump_pos[jump_counter];
            } else {
				dino.y[0] = GROUND_Y;
                jumping = 0;
				jump_counter=0;
				dino_moved=1;
            }
        }
        // Draw updated Dino
        draw_dino();

		if (jump_counter <= 3 || jump_counter >= 19) {

			if(obs.pos_counter > 64 && obs.pos_counter < 81){
				if (GameOver()==0){
					Game_Close();
					return;
				}
				else{
					Game_Init();
					return;
				}

			}
		}


    }else{

	}
    Display_Int_Times();
    timer_irq_clear();
}

void Timer2_ISR(void) {
	if (pause == 0) {
		if (obs.active) {
			clear_obstacle();
			obs.pos_counter++;
			obs.x[0] -= 1; // Move left
			
			if (obs.x[0] + obs.width <11) {
				obs.active = 0; // Off screen
				obs.pos_counter = 0;
				score++;
				if(switches ==7){
						obs.speed = random(8,13);
						obsspeed = speed_table[obs.speed];
						timer2_init((Timer_Load_Value_For_One_Sec / obsspeed), Timer_Prescaler, 1);
						timer2_enable();
				}else{	
					if (score % 3 == 0 && obs.speed < 12) {
						obs.speed++;
						if(obs.speed >=13){
							obs.speed= 13;
						}
						obsspeed = speed_table[obs.speed];
						timer2_init((Timer_Load_Value_For_One_Sec / obsspeed), Timer_Prescaler, 1);
						timer2_enable();
					}
				}
			} else {
				
				draw_obstacle();
			}
		} else {
			// Spawn a new one every N ticks
			if(spawn_counter == 0){
				
				
				if(switches == 1){
					rand_spawn = random(50,100);
				} else if (switches == 3){
					rand_spawn = random(25,50);
				} else if (switches == 7){
					rand_spawn = random(5,15);
				}else{
					rand_spawn = random(50,100);
				}
			
			}
			spawn_counter++;
			if (spawn_counter >= rand_spawn) { // adjust this value to control spawn rate
				obs.x[0] = 90;
				obs.y[0] = GROUND_Y_OBS;
				obs.active = 1;
				spawn_counter = 0;
				obs.pos_counter = 0;
			}
		}
	}
    timer2_irq2_clear();
}

//---------------------------------------------
// Main Function
//---------------------------------------------


int main(void){

	//Initialise the system
	SoC_init();
	//Initialise the game
	Game_Init();

	//Go to sleep mode and wait for interrupts
	while(1){
		__WFI();	
	}

	

}


