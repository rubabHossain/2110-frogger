#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "myLib.h"
#include "text.h"

#include "frogger.h"

/*---------------*/
#include "startScreen.h"
#include "frog.h"
#include "losingScreen.h"
/*---------------*/


// author: Rubab Hossain
// rhossain6
// 903226977


int main() {
	REG_DISPCNT = MODE3 | BG2_ENABLE;

	gameState state = START_SCREEN;


	//displays start screen for the first time
	drawImage3(0, 0, 240, 160, startScreen);
	char buffer[51];
	sprintf(buffer, "Press Start");
	drawString(150, 5, buffer, RED);


	// some booleaans that I probably shouldve put into a struct
	// but Im too lazy and my code works and its sunday
	// and I have a headache
	bool start_down = false;
	bool select_down = false;
	bool left_down = false;
	bool right_down = false;
	bool up_down = false;
	bool down_down = false;
	
	//this variable is used to control animation intervals
	volatile unsigned int vBlanks = 0;	

	// arrays used to maintain river state
	int riverLevels[] = {0, 0, 0};
	int oldRiverLevels[] = {0, 0, 0};

	// variable showing time left to finish
	int timeLeft = 30;

/*-------------------GAME LOOP-------------------------------*/

	while(1) {
		waitForVblank();
		//update animation interval counter
		vBlanks++;
		vBlanks = vBlanks % 60;
		
/*----------------------------------------------*/
		switch(state) {

//________________________________________________1
			case START_SCREEN:
				//this is to prevent cycling through states
				select_down = false;

				// initializes frog position to starting position
				// whenever we come back to starting state
				// Yes I couldve done this in one line, 
				// but doing it like this helps explain what the 
				// numbers in the struct mean
				int myFrogRow = 120;
				int myFrogCol = 100;
				int myFrogL = myFrogCol;
				int myFrogR = myFrogCol + FROG_WIDTH;
				int myFrogT = myFrogRow;
				int myFrogB = myFrogRow + FROG_HEIGHT;
				int myFrogRiverLevel = -1;
				int myFrogQuadrant = 5;
				POSITION myFrog = {myFrogRow, myFrogCol, myFrogL, myFrogR,
					myFrogT, myFrogB, myFrogRiverLevel, myFrogQuadrant};
				timeLeft = 30;

				// reinitializes river state 
				for (int i = 0; i < 3; i++) {
					riverLevels[i] = 0;
					oldRiverLevels[i] = 0;
				}

				/*--------TRANSITION TO NEXT STATE---------------*/
				if (KEY_DOWN_NOW(BUTTON_START) && !start_down) {
					start_down = true;
					state = GAME_PLAY;
					drawWater();
					drawImage3(myFrog.row, myFrog.col, FROG_WIDTH, FROG_HEIGHT, frog);
				} else if (!KEY_DOWN_NOW(BUTTON_START) && start_down) {
					start_down = false;
				}

				break;
//________________________________________________2
			case GAME_PLAY:

				
/*-LEFT BUTTON---------------------------------------------*/
				if (KEY_DOWN_NOW(BUTTON_LEFT) && !left_down) {

					left_down = true;

					// update position data in frog struct
					myFrog.col = myFrog.col - 20;
					myFrog.leftBound = myFrog.leftBound - 20;
					myFrog.rightBound = myFrog.rightBound - 20;
					myFrog.quadrant = myFrog.quadrant - 1;
					
					/* UPDATING THE SCREEN APPROPRIAYELY IF BUTTON PRESSED */
					drawWater();
					drawLogs(oldRiverLevels, riverLevels);
					drawImage3(myFrog.row, myFrog.col, FROG_WIDTH, FROG_HEIGHT, frog);
					char buffer[51];
					sprintf(buffer, "%d", timeLeft);
					drawString(150, 5, buffer, YELLOW);

				} else if (!KEY_DOWN_NOW(BUTTON_LEFT) && left_down) {
					left_down = false;
				}
/*-RIGHT BUTTON---------------------------------------------*/
				if (KEY_DOWN_NOW(BUTTON_RIGHT) && !right_down) {

					right_down = true;

					// update position data in frog struct
					myFrog.col = myFrog.col + 20;
					myFrog.leftBound = myFrog.leftBound + 20;
					myFrog.rightBound = myFrog.rightBound + 20;
					myFrog.quadrant = myFrog.quadrant + 1;
					


					/* UPDATING THE SCREEN APPROPRIAYELY IF BUTTON PRESSED */
					drawWater();
					drawLogs(oldRiverLevels, riverLevels);
					drawImage3(myFrog.row, myFrog.col, FROG_WIDTH, FROG_HEIGHT, frog);
					char buffer[51];
					sprintf(buffer, "%d", timeLeft);
					drawString(150, 5, buffer, YELLOW);

				} else if (!KEY_DOWN_NOW(BUTTON_RIGHT) && right_down) {
					right_down = false;
				}
/*-DOWN BUTTON---------------------------------------------*/
				if (KEY_DOWN_NOW(BUTTON_DOWN) && !down_down) {

					down_down = true;

					// update position data in frog struct
					myFrog.row = myFrog.row + 20;
					myFrog.topBound = myFrog.topBound + 20;
					myFrog.bottomBound = myFrog.bottomBound + 20;
					myFrog.riverLevel = myFrog.riverLevel - 1;
					

					/* UPDATING THE SCREEN APPROPRIAYELY IF BUTTON PRESSED */
					drawWater();
					drawLogs(oldRiverLevels, riverLevels);
					drawImage3(myFrog.row, myFrog.col, FROG_WIDTH, FROG_HEIGHT, frog);
					char buffer[51];
					sprintf(buffer, "%d", timeLeft);
					drawString(150, 5, buffer, YELLOW);

				} else if (!KEY_DOWN_NOW(BUTTON_DOWN) && down_down) {
					down_down = false;
				}
/*-UP BUTTON---------------------------------------------*/

				if (KEY_DOWN_NOW(BUTTON_UP) && !up_down) {

					up_down = true;
					
					// update position data in frog struct
					myFrog.row = myFrog.row - 20;
					myFrog.topBound = myFrog.topBound - 20;
					myFrog.bottomBound = myFrog.bottomBound -20;
					myFrog.riverLevel = myFrog.riverLevel + 1;
					

					/* UPDATING THE SCREEN APPROPRIAYELY IF BUTTON PRESSED */
					drawWater();
					drawLogs(oldRiverLevels, riverLevels);
					drawImage3(myFrog.row, myFrog.col, FROG_WIDTH, FROG_HEIGHT, frog);
					char buffer[51];
					sprintf(buffer, "%d", timeLeft);
					drawString(150, 5, buffer, YELLOW);

				} else if (!KEY_DOWN_NOW(BUTTON_UP) && up_down) {
					up_down = false;
				}
/*-select---------------------------------------------*/

				// making sure we can always return to start screen
				if (KEY_DOWN_NOW(BUTTON_SELECT) && !select_down) {
					select_down = true;
					state = START_SCREEN;
					drawStartScreen();
				} else if (!KEY_DOWN_NOW(BUTTON_START) && select_down) {
					start_down = false;
				}
/*-check bounds---------------------------------------------*/

				// checking if we won or lost by checking position of frog
				if (checkOutOfBounds(myFrog) || timeLeft == 0) {
					state = END;
					drawLosingScreen();
				}
				if (myFrog.topBound <= 40) {
					state = END;
					drawWinningScreen();
				}
				if (((riverLevels[myFrog.riverLevel] & (0x800 >> myFrog.quadrant)) == 0)
					&& myFrog.riverLevel >=0 && myFrog.riverLevel < 3) {
					state = END;
					drawLosingScreen();
				}
/*-updating river---------------------------------------------*/

				/* ANIMATES THE RIVER*/	
				if (vBlanks == 0) {
					timeLeft--;
					// makes froggy move if in the river
					if (myFrog.riverLevel >=0 && myFrog.riverLevel < 3) {

						//if tide is to the right, move right
						if (myFrog.riverLevel % 2 == 1) {
							myFrog.quadrant++;
							myFrog.col += 20;
							myFrog.rightBound += 20;
							myFrog.leftBound += 20;
						} else {
							// else move left
							myFrog.quadrant--;
							myFrog.col -= 20;
							myFrog.rightBound -= 20;
							myFrog.leftBound -= 20;
						}
					}
					// old river state <- current river state
					for (int i = 0; i < 3; i++) {
						oldRiverLevels[i] = riverLevels[i];
					}
					// new river state <-  updated current river state
					updateRiver(riverLevels);

					// UPDATE THE RIVER ON SCREEN
					drawWater();
					drawLogs(oldRiverLevels, riverLevels);	
					drawImage3(myFrog.row, myFrog.col, FROG_WIDTH, FROG_HEIGHT, frog);	
					char buffer[51];
					sprintf(buffer, "%d", timeLeft);
					drawString(150, 5, buffer, YELLOW);
				
				}

				break;
//_State: END_______________________________________________3
			case END:

				//BASIC ENDING STUFF

				if (KEY_DOWN_NOW(BUTTON_START) && !start_down) {
					start_down = true;
					state = START_SCREEN;
					drawStartScreen();
				} else if (!KEY_DOWN_NOW(BUTTON_START) && start_down) {
					start_down = false;
				}
				if (KEY_DOWN_NOW(BUTTON_SELECT) && !select_down) {
					select_down = true;
					state = START_SCREEN;
					drawStartScreen();
				} else if (!KEY_DOWN_NOW(BUTTON_START) && select_down) {
					start_down = false;
				}

				break;


		}
/*----------------------------------------------*/
	}

	return 0;

}


