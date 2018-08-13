#include <stdio.h>
#include <stdlib.h>
#include "myLib.h"
#include "frogger.h"
#include "text.h"

#include "startScreen.h"
#include "frog.h"
#include "losingScreen.h"
#include "winningScreen.h"


// author: Rubab Hossain
// rhossain6
// 903226977

volatile unsigned short *videoBuffer = (u16*) 0x6000000;



void drawImage3(int row, int col, int width, int height, const u16* image) {
	for (int r = 0; r < height; r++) {
		DMA[3].src = image + (r * width);	// starts DMA.src at start of every row
		DMA[3].dst = videoBuffer + ((row + r) * 240) + col;
		DMA[3].cnt = width | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | DMA_ON;
	}
}

// magic sauce
void waitForVblank() {
    while(*SCANLINECOUNTER > 160);
    while(*SCANLINECOUNTER < 160);
}


// maintains lowest layer of gameplay screen
void drawWater() {

	drawRectangle(0, 0, 160, 240, BLACK);
	drawRectangle(60, 0, 60, 240, CYAN);
}

// general rectangle drawing method
void drawRectangle(int row, int col, int height, int width, unsigned int color) {

	volatile unsigned int c = color;
	for(int r=0; r<height; r++) {	
		DMA[3].src = &c;
		DMA[3].dst = videoBuffer + ((row + r)*240 + col);
		DMA[3].cnt = width | DMA_ON | DMA_SOURCE_FIXED;
	}
}

// checks if frog is out of bounds
bool checkOutOfBounds(POSITION pos) {
	if (pos.leftBound < 0) {
		return true;
	}
	if (pos.rightBound > 240) {
		return true;
	}
	if (pos.bottomBound > 170) {
		return true;
	}
	return false;
}

// intros user to best game ever
void drawStartScreen() {
	drawImage3(0, 0, 240, 160, startScreen);
	char buffer[51];
	sprintf(buffer, "Press Start");
	drawString(150, 5, buffer, RED);
}

// motivates user to get good
void drawLosingScreen() {
	drawImage3(0, 0, 240, 160, losingScreen);
	char buffer[51];
	sprintf(buffer, "dang");
	drawString(150, 5, buffer, RED);
}


// just draws winning screen
void drawWinningScreen() {
	drawImage3(0, 0, 240, 160, winningScreen);
	char buffer[51];
	sprintf(buffer, "HIGH QUALITY Content(TM) :)");
	drawString(150, 5, buffer, RED);

	char buffer2[51];
	sprintf(buffer2, "Hope you have enjoyed this");
	drawString(140, 5, buffer2, RED);
}


// draws part of log that is designated by 2 numbers
void drawBrownRect(int riverLevel, int index) {

	int row = 20 + ((4 - riverLevel) * 20) + 1;
	int col = index * 20;
	drawRectangle(row, col, 19, 20, COLOR(165,42,42));
}


// draws a part of river that is designated by 2 numbers
void drawBlueRect(int riverLevel, int index) {

	int row = 20 + ((4 - riverLevel) * 20);
	int col = index * 20;
	drawRectangle(row, col, 20, 20, CYAN);
}



// updates positions of logs and spawns logs rnaodmly onto river
void updateRiver(int riverLevels[]) {
	

	//first shift bits
	//speed = how fast the platforms are moving
	int speed = 0x1;
	riverLevels[0] = (riverLevels[0] << speed) & 0x0FFE;
	//add chance of spawning new wood from right border
	int spawnWoodChance = rand() % 100;
	// higher number for difficulty = harder to pass level
	int difficulty = 40;
	riverLevels[0] = spawnWoodChance > difficulty ? riverLevels[0] | 0x1: riverLevels[0];


	riverLevels[1] = (riverLevels[1] >> speed) & 0x7FF;
	spawnWoodChance = rand() % 100;
	difficulty = 60;
	riverLevels[1] = spawnWoodChance > difficulty ? riverLevels[1] | 0x800: riverLevels[1];


	riverLevels[2] = (riverLevels[2] << speed) & 0x0FFE;
	spawnWoodChance = rand() % 100;
	difficulty = 80;
	riverLevels[2] = spawnWoodChance > difficulty ? riverLevels[2] | 0x1: riverLevels[2];



	// // I HAD 5 RIVER LEVELS BUT SCREEN TEARS IF I TRY TO DO THAT MANY ANIMATIONS 
	// // : (
	// 
	// riverLevels[3] = (riverLevels[3] >> speed) & 0x0FFF;
	// spawnWoodChance = rand() % 100;
	// difficulty =80;
	// riverLevels[3] = spawnWoodChance > difficulty ? riverLevels[3] + 0x800: riverLevels[3];


	// riverLevels[4] = (riverLevels[4] << speed) & 0x0FFF;
	// spawnWoodChance = rand() % 100;
	// difficulty = 90;
	// riverLevels[4] = spawnWoodChance > difficulty ? riverLevels[4] + 0x1: riverLevels[4];

}


// draws the updated river onto screen
void drawLogs(int oldRiverLevels[], int newRiverLevels[]) {
	
	for (int i = 0; i < 3; i++) {


		for (int j = 0; j < 12; j++) {

			volatile int before = (oldRiverLevels[i] & (1 << j)) >> j;
			volatile int after = (newRiverLevels[i] & (1 << j)) >> j;

			if (before != 0 && after == 0) {
			//no change
				drawBlueRect(i, 11 - j);
			
			} else if ((before == 0 && after != 0) || (before != 0 && after != 0)) {
			// it was brown but make it blue
			
				drawBrownRect(i, 11 - j);
			} else {
			//it was blue, so make it brown
			}
			//
			// ------------------------------------- 
			// BELOW IS CORRECT CODE, BUT IT IS SLOW
			// 
			// // oldRiverLevels[i]++ is there so that C doesnt throw a hissy fit
			//oldRiverLevels[i]++;
			// 
			// if ((newRiverLevels[i] & (1 << j)) == 0) {
			// //no change
			// 	drawBlueRect(i, 11 - j);
			
			// } else {
			// //it was blue, so make it brown
			// 	drawBrownRect(i, 11 - j);
			// }

		}	
	}
}