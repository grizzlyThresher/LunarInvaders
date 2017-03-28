

#include "myLib.h"

unsigned short *videoBuffer = (unsigned short *)0x6000000;

void delay(int n)
{
	volatile int x;
	for(int i=0; i<n*50; i++)
	{
		x = x + 1;
	}
}

void setPixel(int row, int col, unsigned short color)
{
	videoBuffer[OFFSET(row,col, 240)] = color;
}

void drawImage3(int row, int col, int height, int width, const unsigned short* image)
{
	for(int r=0; r<height; r++)
	{
		REG_DMA3SAD = (u32)image;
		REG_DMA3DAD = (u32)(&videoBuffer[OFFSET(row+r, col, 240)]);
		REG_DMA3CNT = width | DMA_ON | DMA_SOURCE_INCREMENT;
        image += width;
	}
}

void updateTop(int row, int col, int width, int offset, int bgwidth, const unsigned short* image)
{
    image += (bgwidth * (row - offset) + col);
    REG_DMA3SAD = (u32)image;
    REG_DMA3DAD = (u32)(&videoBuffer[OFFSET(row, col, 240)]);
    REG_DMA3CNT = width | DMA_ON | DMA_SOURCE_INCREMENT;   
}
void updateBack(int row, int col, int height, int offset, int bgwidth, const unsigned short* image)
{
    image += (bgwidth * (row - offset) + col);
    for(int r = 0; r < height; r++)
    {
        REG_DMA3SAD = (u32)image;
        REG_DMA3DAD = (u32)(&videoBuffer[OFFSET(row+r, col, 240)]);
        REG_DMA3CNT = 1 | DMA_ON | DMA_SOURCE_INCREMENT;
        image += bgwidth;
    }

}
void updateFront(int row, int col, int height, int width, int offset, int bgwidth, const unsigned short* image)
{
    image += (bgwidth * (row - offset) + col);
    for(int r = 0; r < height; r++)
    {
        REG_DMA3SAD = (u32)image + (width - 1) * 2;
        REG_DMA3DAD = (u32)(&videoBuffer[OFFSET(row+r, col + (width - 1) , 240)]);
        REG_DMA3CNT = 1 | DMA_ON | DMA_SOURCE_INCREMENT;
        image += bgwidth;
    }
    
}
void updateBottom(int row, int col, int height,int width, int offset, int bgwidth, const unsigned short* image)
{
    image += (bgwidth * (row - offset + height) + col);
    REG_DMA3SAD = (u32)image;
    REG_DMA3DAD = (u32)(&videoBuffer[OFFSET(row + (height - 1), col, 240)]);
    REG_DMA3CNT = width | DMA_ON | DMA_SOURCE_INCREMENT;
    
}
void updateBG(int row, int col, int height, int width, int bgwidth, int offset, const unsigned short* image)
{
    image += (bgwidth * (row - offset) + col);
    for(int r=0; r<height; r++)
    {
        REG_DMA3SAD = (u32)image;
        REG_DMA3DAD = (u32)(&videoBuffer[OFFSET(row+r, col, 240)]);
        REG_DMA3CNT = width | DMA_ON | DMA_SOURCE_INCREMENT;
        image += bgwidth;
    }
}
void drawRect(int row, int col, int height, int width, volatile unsigned short color)
{
	for(int r=0; r<height; r++)
	{
		REG_DMA3SAD = (u32)&color;
		REG_DMA3DAD = (u32)(&videoBuffer[OFFSET(row+r, col, 240)]);
		REG_DMA3CNT = width | DMA_ON | DMA_SOURCE_FIXED;
	}
}
void waitForVblank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}
