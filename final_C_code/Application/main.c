//--------------------------------------------------------
// Application demonstrator: SNAKE game
//--------------------------------------------------------

#include "EDK_CM0.h"
#include "core_cm0.h"
#include "edk_api.h"
#include "edk_driver.h"
<<<<<<< HEAD
#include <stdio.h>
#include "modified_dino.h" 
#include "modified_cactus.h" 

=======

#include <stdio.h>

>>>>>>> 7ec0088b5a2306d8679269bbeaa73978b1064fe3
// Game region
#define left_boundary 5
#define right_boundary 96
#define top_boundary 5
#define bottom_boundary 116
#define boundary_thick 1

#define DINO_X (left_boundary + 10)
<<<<<<< HEAD
#define GROUND_Y (bottom_boundary - 36)

#define GROUND_Y_OBS (bottom_boundary - 25)
#define BACKGROUND_COLOR BLACK
#define DINO_COLOR GREEN
=======
#define GROUND_Y (bottom_boundary - 40)

#define GROUND_Y_OBS (bottom_boundary - 25)

>>>>>>> 7ec0088b5a2306d8679269bbeaa73978b1064fe3
// Global variables
static int i;
static char key;
static int score;
static int pause;

int jump_pos[17] = {0,  4,  8,  12, 16, 20, 24, 28, 30,
                    28, 24, 20, 16, 12, 8,  4,  0};
int dino_moved = 1;
int jump_counter = 0;
int jumping = 0;

static int gamespeed;
static int speed_table[10] = {6, 9, 12, 15, 20, 25, 30, 35, 40, 100};

struct Dino {
  int x[5]; // leave room for multiple rectangles
  int y[5];
} dino;

struct Obstacle {
  int x[5];
  int y[5];
  int width;
  int height;
  int active;
} obs;

//---------------------------------------------
// Game
//---------------------------------------------

void Game_Init(void) {
  // Draw a game region
  clear_screen();
  rectangle(left_boundary, top_boundary, right_boundary,
            top_boundary + boundary_thick, BLUE);
  rectangle(left_boundary, top_boundary, left_boundary + boundary_thick,
            bottom_boundary, BLUE);
  rectangle(left_boundary, bottom_boundary, right_boundary,
            bottom_boundary + boundary_thick, BLUE);
  rectangle(right_boundary, top_boundary, right_boundary + boundary_thick,
            bottom_boundary + boundary_thick, BLUE);

  rectangle(left_boundary + boundary_thick, bottom_boundary - 19,
            right_boundary, bottom_boundary - 14, WHITE); // floor

  // Initialise data

  score = 5;

  gamespeed = speed_table[score];

  // Initialise timer (load value, prescaler value, mode value)
  timer_init((Timer_Load_Value_For_One_Sec / gamespeed), Timer_Prescaler, 1);
  timer_enable();
  pause = 0;

  // Print instructions
  printf("\n------- DINO Game --------");
  printf("\nKeyboard w ........ JUMP");
  printf("\nKeyboard space ...... pause");
  printf("\n---------------------------");

  printf("\nJump to Avoid the Objects");
  printf("\nPress Space to Pause");
  printf("\n---------------------------");
  printf("\nPress any key to start\n");
  while (KBHIT() == 0)
    ;

  dino.x[0] = DINO_X;
  dino.y[0] = GROUND_Y;

  obs.y[0] = GROUND_Y_OBS; // Make sure GROUND_Y matches your floor
  obs.width = 5;
  obs.height = 5;
  obs.active = 0;

  draw_dino();

  NVIC_EnableIRQ(Timer_IRQn); // start timing
  NVIC_EnableIRQ(UART_IRQn);
}

void Game_Close(void) {
  clear_screen();
  score = 0;
  printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"); // flush screen
  printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
  NVIC_DisableIRQ(Timer_IRQn);
  NVIC_DisableIRQ(UART_IRQn);
}

int GameOver(void) {
  char key;
<<<<<<< HEAD
=======

  NVIC_DisableIRQ(UART_IRQn);
  NVIC_DisableIRQ(Timer_IRQn);
  printf("\nGame over\n");
  printf("\nPress 'q' to quit");
  printf("\nPress 'r' to replay");
  while (1) {
    while (KBHIT() == 0)
      ;
    key = UartGetc();
    if (key == RESET) {
      return 1;
    } else if (key == QUIT) {
      return 0;
    } else
      printf("\nInvalid input");
  }
}
>>>>>>> 7ec0088b5a2306d8679269bbeaa73978b1064fe3

  NVIC_DisableIRQ(UART_IRQn);
  NVIC_DisableIRQ(Timer_IRQn);
  printf("\nGame over\n");
  printf("\nPress 'q' to quit");
  printf("\nPress 'r' to replay");
  while (1) {
    while (KBHIT() == 0)
      ;
    key = UartGetc();
    if (key == RESET) {
      return 1;
    } else if (key == QUIT) {
      return 0;
    } else
      printf("\nInvalid input");
  }
}
//int draw_dino(void) {
//  rectangle(dino.x[0], dino.y[0], dino.x[0] + 10, dino.y[0] + 20, WHITE);
//  return 0;
//}
int draw_dino(void) {
<<<<<<< HEAD
	int i, j;
	int rows = sizeof(modified_dino_rom) / sizeof(modified_dino_rom[0]);          // height
	int cols = sizeof(modified_dino_rom[0]) / sizeof(modified_dino_rom[0][0]);    // width

	for (j = 0; j < rows; j++) {
		for (i = 0; i < cols; i++) {
			if (modified_dino_rom[j][i] != 0) {
				VGA_plot_pixel(dino.x[0] + i, dino.y[0] + j, GREEN);
			}
		}
	}
	return 0;
=======
  rectangle(dino.x[0], dino.y[0], dino.x[0] + 10, dino.y[0] + 20, WHITE);
  return 0;
>>>>>>> 7ec0088b5a2306d8679269bbeaa73978b1064fe3
}

int clear_dino(void) {
  rectangle(dino.x[0], dino.y[0], dino.x[0] + 10, dino.y[0] + 20, BLACK);
  return 0;
}

int draw_obstacle(void) {
<<<<<<< HEAD
int i, j;
	int rows = sizeof(modified_cactus_rom) / sizeof(modified_cactus_rom[0]);          // height
	int cols = sizeof(modified_cactus_rom[0]) / sizeof(modified_cactus_rom[0][0]);    // width

	for (j = 0; j < rows; j++) {
		for (i = 0; i < cols; i++) {
			if (modified_cactus_rom[j][i] != 0) {
				VGA_plot_pixel(obs.x[0] + i, obs.y[0] + j, GREEN);
			}
		}
	}
=======
  rectangle(obs.x[0], obs.y[0], obs.x[0] + obs.width, obs.y[0] + obs.height,
            GREEN);
>>>>>>> 7ec0088b5a2306d8679269bbeaa73978b1064fe3
  return 0;
}

int clear_obstacle(void) {
  rectangle(obs.x[0], obs.y[0], obs.x[0] + obs.width, obs.y[0] + obs.height,
            BLACK);
  return 0;
}

//---------------------------------------------
// UART ISR -- used to input commands
//---------------------------------------------

void UART_ISR(void) {

  key = UartGetc();

  // Only update the direction if the previous movement is finished
  if (dino_moved == 1) {
    if (key == UP) {
      jumping = 1;
      jump_counter = 0;
      dino_moved = 0;
    }
  }
  if (key == PAUSE) {
    if (pause == 0) {
      pause = 1;
      NVIC_DisableIRQ(Timer_IRQn);
    } else {
      pause = 0;
      NVIC_EnableIRQ(Timer_IRQn);
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
      if (jump_counter < 17) {
        dino.y[0] = GROUND_Y - jump_pos[jump_counter];
      } else {
        dino.y[0] = GROUND_Y;
        jumping = 0;
        jump_counter = 0;
        dino_moved = 1;
      }
    }
    // Draw updated Dino
    draw_dino();

    if (obs.active) {
      clear_obstacle();
      obs.x[0] -= 2; // Move left

      if (obs.x[0] + obs.width < 10) {
        obs.active = 0; // Off screen
      } else {
        draw_obstacle();
      }
    } else {
      // Spawn a new one every N ticks
      static int spawn_counter = 0;
      spawn_counter++;
      if (spawn_counter >= 50) { // adjust this value to control spawn rate
        obs.x[0] = 90;
        obs.y[0] = GROUND_Y_OBS;
        obs.active = 1;
        spawn_counter = 0;
      }
    }
  }

  Display_Int_Times();
  timer_irq_clear();
}

//---------------------------------------------
// Main Function
//---------------------------------------------

int main(void) {

  // Initialise the system
  SoC_init();
  // Initialise the game
  Game_Init();

  // Go to sleep mode and wait for interrupts
  while (1)
    __WFI();
<<<<<<< HEAD
}
=======
}
>>>>>>> 7ec0088b5a2306d8679269bbeaa73978b1064fe3
