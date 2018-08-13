#include <stdbool.h>

// author: Rubab Hossain
// rhossain6
// 903226977

typedef enum state {
    START_SCREEN,
    GAME_PLAY,
    END
} gameState;


typedef struct position {
	int row;
	int col;
	int leftBound;
	int rightBound;
	int topBound;
	int bottomBound;
	int riverLevel;
	int quadrant;
} POSITION;


/*prototypes*/
bool key_down(int key);
void drawImage3(int r, int c, int width, int height, const u16* image);
void waitForVblank();
void drawWater();
void drawStartScreen();
void drawLosingScreen();
void drawWinningScreen();
void drawRectangle(int, int, int, int, unsigned int);
bool checkOutOfBounds(POSITION);

void drawBrownRect(int, int);
void drawBlueRect(int, int);

void updateRiver(int[]);
void drawLogs(int[], int[]);