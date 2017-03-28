// Draw an image from bmptoc
// Name: Owen Brahms

// If you want to test this with one of your own images, you may do so by
// including the appropriate header file. However, when you submit this file,
// it must contain ONLY #include "diddy.h" or else our grader will not work
// and you will not receive credit for this file.
#include "GameScreen.h"
#include "BG.h"

#define REG_DISPCNT *(unsigned short*) 0x4000000
#define MODE_3 3
#define BG1_EN (1 << 9)
#define BG2_EN (1 << 10)

unsigned short *videoBuffer = (unsigned short*) 0x6000000;

void drawImage3(int x, int y, int width, int height, const unsigned short *image) {
	
	for(int j = 0; j < height; j++) {
		for(int i = 0; i < width; i++) {
			videoBuffer[(y + j) * 240 + (x + i)] = *image;
			image++;
		}
	}

	// TODO Draw the image with the top left corner at (x,y).
	// Recall that the GBA screen is 240 pixels wide, and 160 tall.
	// The videoBuffer is a 1-dimensional array of these pixels.
	// For example, to set pixel (37,21) to white:
	//   videoBuffer[21 * 240 + 37] = 0x7FFF;



}

int main() {

	// 1. TODO Set REG_DISPCNT appropriately for drawing in mode 3.
	// Use the #define fields near the top of this file to accomplish this.
	REG_DISPCNT = (BG2_EN | MODE_3);



	// 2. TODO Call your drawing function using parameters from the header.
	// You can use whatever you want for X and Y. If you use a high X and the width
	// goes beyond the edge of the screen, it will wrap around onto the next line.
	// This is normal.
	// You may test your own images with calls to drawImage3 here using the
	// appropriate fields from your header, but when you submit this file it
	// must contain ONLY a valid call using the diddy.h parameters, or else
	// our grader will not work and you will not receive credit for this file.
	drawImage3(0, 0, BG_WIDTH, BG_HEIGHT, BG_data);	

	REG_DISPCNT = (BG1_EN | MODE_3) ^ BG2_EN;
	
	drawImage3(0, 12, GAMESCREEN_WIDTH, GAMESCREEN_HEIGHT, GameScreen_data);
	



	// 3. (already done) wait forever after drawing.
	while(1);
	return 0;
}

