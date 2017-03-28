#include <stdlib.h>

typedef unsigned short u16;
typedef unsigned int u32;

#define NUMOBJS 7

#define REG_DISPCTL *(unsigned short *)0x4000000
#define MODE3 3
#define BG2_ENABLE (1<<10)
#define BG1_ENABLE (1<<9)
#define BG3_ENABLE (1<<11)
#define CHNGBG(bg1, bg2) ((MODE3 | (bg1)) ^ (bg2)) 

#define SCANLINECOUNTER *(volatile unsigned short *)0x4000006

#define OFFSET(row, col, rowlen)  ((row)*(rowlen)+(col))

unsigned short *videoBuffer;

#define COLOR(r, g, b) ((r) | (g)<<5 | (b)<<10)
#define WHITE COLOR(31,31,31)
#define RED COLOR(31,0,0)
#define GREEN COLOR(0,31,0)
#define BLUE COLOR(0,0,31)
#define MAGENTA COLOR(31, 0, 31)
#define CYAN COLOR(0, 31, 31)
#define YELLOW COLOR(31, 31, 0)
#define GREY COLOR(25, 25, 25)
#define BLACK 0

// Buttons

#define BUTTON_A		(1<<0)
#define BUTTON_B		(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT		(1<<5)
#define BUTTON_UP		(1<<6)
#define BUTTON_DOWN		(1<<7)
#define BUTTON_R		(1<<8)
#define BUTTON_L		(1<<9)

#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)

#define BUTTONS *(volatile unsigned int *)0x4000130

/* DMA */

#define REG_DMA0SAD         *(volatile u32*)0x40000B0 		// source address
#define REG_DMA0DAD         *(volatile u32*)0x40000B4       // destination address
#define REG_DMA0CNT         *(volatile u32*)0x40000B8       // control register

// DMA channel 1 register definitions
#define REG_DMA1SAD         *(volatile u32*)0x40000BC 		// source address
#define REG_DMA1DAD         *(volatile u32*)0x40000C0       // destination address
#define REG_DMA1CNT         *(volatile u32*)0x40000C4       // control register

// DMA channel 2 register definitions
#define REG_DMA2SAD         *(volatile u32*)0x40000C8 		// source address
#define REG_DMA2DAD         *(volatile u32*)0x40000CC       // destination address
#define REG_DMA2CNT         *(volatile u32*)0x40000D0       // control register

// DMA channel 3 register definitions
#define REG_DMA3SAD         *(volatile u32*)0x40000D4 		// source address
#define REG_DMA3DAD         *(volatile u32*)0x40000D8       // destination address
#define REG_DMA3CNT         *(volatile u32*)0x40000DC       // control register

typedef struct
{
	const volatile void *src;
	volatile void *dst;
	unsigned int cnt;
} DMA_CONTROLLER;

#define DMA ((volatile DMA_CONTROLLER *) 0x040000B0)

// Defines
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)


#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)

typedef struct {
	int row;
	int col;
	int rd;
	int cd;
	int height;
    int width;
	const unsigned short *image;
} MOVOBJ;

enum GBAState {
    TITLE,
    GAMESETUP,
    GAME,
    WIN,
    WINSETUP,
    LOSE,
    LOSESETUP
};

MOVOBJ bullets[100];
MOVOBJ enemies[100];
MOVOBJ ship;
MOVOBJ bigBad;
int bcnt;
int bfrnt;
int ecnt;
int efrnt;
int shot;
int timer;
int deathTime;
int lives;
int hp;
int score;
int num1;
int num2;
int num3;
int time;



// Prototypes
void setPixel(int row, int col, unsigned short color);
void drawImage3(int row, int col, int height, int width, const unsigned short *image);
void updateBottom(int row, int col, int height,int width, int offset, int bgwidth, const unsigned short* image);
void updateBack(int row, int col, int height, int offset, int bgwidth, const unsigned short* image);
void updateTop(int row, int col, int width, int offset, int bgwidth, const unsigned short* image);
void updateFront(int row, int col, int height,int width, int offset, int bgwidth, const unsigned short* image);
void updateBG(int row, int col, int height, int width, int bgwidth, int offset, const unsigned short* image);
void drawRect(int row, int col, int height, int width, volatile unsigned short color);
void delay(int n);


void waitForVblank();

