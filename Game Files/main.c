#include <stdlib.h>
#include "stdio.h"
#include "myLib.h"
#include "BG.h"
#include "GameScreen.h"
#include "Ship.h"
#include "BlueBullet2.h"
#include "MainTitle.h"
#include "MinorEnemy.h"
#include "life1.h"
#include "life2.h"
#include "life3.h"
#include "WinScreen.h"
#include "text.h"

int main()
{
    REG_DISPCTL = MODE3 | BG2_ENABLE;
    enum GBAState state = TITLE;
	
    while(1)
    {
        switch(state)
        {
            case TITLE:
                drawImage3(0, 0, MAINTITLE_HEIGHT, MAINTITLE_WIDTH, MainTitle_data);
                if(KEY_DOWN_NOW(BUTTON_START)) 
                {
                    state = GAMESETUP;
                }
                break;
            case GAMESETUP:
                drawImage3(0, 0, BG_HEIGHT, BG_WIDTH, BG_data);
                drawImage3(12, 0, GAMESCREEN_HEIGHT, GAMESCREEN_WIDTH, GameScreen_data);
                drawImage3(148, 3, LIFE1_HEIGHT, LIFE1_WIDTH, life1_data);
                drawImage3(148, 28, LIFE2_HEIGHT, LIFE2_WIDTH, life2_data);
                drawImage3(148, 53, LIFE2_HEIGHT, LIFE2_WIDTH, life3_data);
                drawRect(151, 160, 5, 75, RED);
    
                bcnt = 0;
                bfrnt = 0;
                
                ecnt = 0;
                efrnt = 0;

                ship.row = 80;
                ship.col = 12;
                ship.rd = 0;
                ship.cd = 0;
                ship.height = SHIP_HEIGHT;
                ship.width = SHIP_WIDTH;
                ship.image = Ship_data;

                drawImage3(ship.row, ship.col, ship.height, ship.width, ship.image);

                shot = 0;
                int k = 0;
                int t = 0;
                char buff[4];
                time = 100;
                timer = 1;
                deathTime = 0;
                lives = 3;
                hp = 75;
                updateBG(2, 105, 8, 24, BG_WIDTH, 0, BG_data);
                num1 = 1;
                num2 = 0;
                num3 = 0;

                state = GAME;
                break;
            case GAME:
                if (timer % 75 == 0)
                {
                    time--;
                    updateBG(2, 105, 8, 24, BG_WIDTH, 0, BG_data);
                    num1 = time /100;
                    num2 = (time / 10) % 10;
                    if (num2 > 6) 
                    {
                        num2 -= 4;
                        time -= 40;
                    }
                    num3 = time % 10;
                    if (time == 0)
                    {
                        state = WINSETUP;
                    }
                }
                sprintf(buff,"%d:%d%d", num1, num2, num3);
                drawString(2, 105, buff, YELLOW);
                
                if(KEY_DOWN_NOW(BUTTON_SELECT))
                {
                    state = TITLE;
                }
                if (deathTime == 0)
                {
                    timer++;
                    if(KEY_DOWN_NOW(BUTTON_UP)) 
                    {
                
                        updateBottom(ship.row, ship.col, ship.height, ship.width, 12, GAMESCREEN_WIDTH, GameScreen_data);
                        ship.row--;
                        if(ship.row < 13)
                        {
                            ship.row = 13;
                        }       
                        drawImage3(ship.row, ship.col, ship.height, ship.width, ship.image);    
                    }
                    if(KEY_DOWN_NOW(BUTTON_DOWN)) 
                    {
                        updateTop(ship.row, ship.col, ship.width, 12, GAMESCREEN_WIDTH, GameScreen_data);
                        ship.row++;
                        if(ship.row > (160 - ship.width))
                        {
                            ship.row = (160 - ship.width);
                        }
                        drawImage3(ship.row, ship.col, ship.height, ship.width, ship.image);  
                    }
                    if(KEY_DOWN_NOW(BUTTON_RIGHT)) 
                    {
                        updateBack(ship.row, ship.col, ship.height, 12, GAMESCREEN_WIDTH, GameScreen_data);
                        ship.col++;
                        if (ship.col > (107 - ship.width))
                        {
                            ship.col = (107 - ship.width);
                        }
                        drawImage3(ship.row, ship.col, ship.height, ship.width, ship.image);  
                    }
                    if(KEY_DOWN_NOW(BUTTON_LEFT)) 
                    {
                        updateFront(ship.row, ship.col, ship.height, ship.width, 12, GAMESCREEN_WIDTH, GameScreen_data);
                        ship.col--;
                        if(ship.col < 0) 
                        {
                            ship.col = 0;
                        }
                        drawImage3(ship.row, ship.col, ship.height, ship.width, ship.image);  
                    }
                    if(KEY_DOWN_NOW(BUTTON_A) && shot == 0) 
                    {
                        shot = 1;
                        bullets[bcnt].row = ship.row + 5;
                        bullets[bcnt].col = ship.col + ship.width;
                        bullets[bcnt].rd = 0;
                        bullets[bcnt].cd = 3;
                        bullets[bcnt].height = BLUEBULLET2_HEIGHT;
                        bullets[bcnt].width = BLUEBULLET2_WIDTH;
                        bullets[bcnt].image = BlueBullet2_data;
                        bcnt++;
                        if (bcnt == 50) {
                            bcnt = 0;
                        }
                    }
                    if(!KEY_DOWN_NOW(BUTTON_A)) 
                    {
                       shot = 0;
                    }
                }
                else if(deathTime > 1)
                {
                    deathTime--;
                }
                else if(deathTime == 1)
                {
                    ship.row = 80;
                    ship.col = 12;
                    drawImage3(ship.row, ship.col, ship.height, ship.width, ship.image);
                    deathTime--;
                }
                delay(2);
                k = 0;
                while((k + bfrnt) - bcnt != 0)
                {
            
                    t = k + bfrnt;
                    for (int i = 0; i < bullets[t].cd; i++) {
                        updateBack(bullets[t].row, bullets[t].col + i, bullets[t].height, 12, GAMESCREEN_WIDTH, GameScreen_data);
                    }
                    bullets[t].col += bullets[t].cd;
                    drawImage3(bullets[t].row, bullets[t].col, bullets[t].height, bullets[t].width, bullets[t].image);
                    if (bullets[t].col >= (240 - bullets[t].width))
                    {
                        updateBG(bullets[t].row, bullets[t].col, bullets[t].height, bullets[t].width,GAMESCREEN_WIDTH, 12, GameScreen_data);
                        bfrnt++;
                    }
                    else if(bullets[t].col >= 214 - bullets[t].width && bullets[t].col <= 240 - bullets[t].width && bullets[t].row <= 90
                     && (bullets[t].row >= 72 || bullets[t].row + bullets[t].height >= 72))
                    {
                        if(hp == 0 && deathTime == 0)
                        {
                            state = WINSETUP;
                        }
                        else
                        {
                        updateBG(bullets[t].row, bullets[t].col, bullets[t].height, bullets[t].width,GAMESCREEN_WIDTH, 12, GameScreen_data);
                        bfrnt++;
                            hp--;
                            updateBack(151, 240 - (hp + 6), 5, 0, BG_WIDTH, BG_data);
                        }
                    }
                    if (bfrnt != bcnt) 
                    {
                        k++;
                        if((k + bfrnt) == 50)
                        {
                            k = (0 - bfrnt);
                        } 
                        else if(bfrnt == 50)
                        {
                            bfrnt = 0;
                        }
                    }
                }
        
                if (timer % 10 == 0 && deathTime == 0) 
                {

                    enemies[ecnt].row = rand()%125 + 12;
                    enemies[ecnt].col = 190;
                    enemies[ecnt].rd = 0;
                    enemies[ecnt].cd = -3;
                    enemies[ecnt].height = MINORENEMY_HEIGHT;
                    enemies[ecnt].width = MINORENEMY_WIDTH;
                    enemies[ecnt].image = MinorEnemy_data;
                    ecnt++;
                    if (ecnt == 50) {
                        ecnt = 0;
                    }
                }
                k = 0;
                while((k + efrnt) - ecnt != 0)
                {
           
                    t = k + efrnt;
                    for (int i = 0; i < 0 - enemies[t].cd; i++) {
                        updateFront(enemies[t].row, enemies[t].col + i, enemies[t].height, enemies[t].width, 12, GAMESCREEN_WIDTH, GameScreen_data);
                    }
                    enemies[t].col += enemies[t].cd;
                    drawImage3(enemies[t].row, enemies[t].col, enemies[t].height, enemies[t].width, enemies[t].image);
                    if (enemies[t].col <= 2)
                    {
                        updateBG(enemies[t].row, enemies[t].col, enemies[t].height, enemies[t].width + 2, GAMESCREEN_WIDTH, 12, GameScreen_data);
                        efrnt++;
                    }
                    else if ((enemies[t].col <= (ship.col + ship.width) && (enemies[t].col >= ship.col))
                    && (enemies[t].row >= (ship.row) || enemies[t].row + enemies[t].height >= (ship.row)) && (enemies[t].row <= (ship.row + ship.height))
                    && deathTime == 0)
                    {
                        switch(lives)
                        {
                           case 3:
                                updateBG(148, 3, LIFE1_HEIGHT, LIFE1_WIDTH, BG_WIDTH, 0, BG_data);
                                break;
                            case 2:
                                updateBG(148, 28, LIFE2_HEIGHT, LIFE2_WIDTH, BG_WIDTH, 0, BG_data);
                                break;
                            case 1:
                                updateBG(148, 53, LIFE2_HEIGHT, LIFE2_WIDTH, BG_WIDTH, 0, BG_data);
                                break;
                            case 0:
                                state = LOSESETUP;
                            break;
                        }
                            lives--;
                            updateBG(ship.row, ship.col, ship.height, ship.width, GAMESCREEN_WIDTH, 12, GameScreen_data);
                            deathTime = 250;
                    }
                    if (efrnt != ecnt) 
                    {
                        k++;
                        if((k + efrnt) == 50)
                        {
                            k = (0 - efrnt);
                        } 
                        else if(efrnt == 50)
                        {
                            efrnt = 0;
                        }
                    }    
                }
                waitForVblank();
                break;
            case LOSE:
                if(KEY_DOWN_NOW(BUTTON_SELECT))
                {
                    state = TITLE;
                }
                break;
            case LOSESETUP:
                drawRect(0, 0, BG_HEIGHT, BG_WIDTH, BLACK);
                drawString(70, 90, "GAME OVER", RED);
                drawString(90, 88, "try again?", RED);
                state = LOSE;
                break;
            case WIN:
                if(KEY_DOWN_NOW(BUTTON_SELECT))
                {
                    state = TITLE;
                }
                break;
            case WINSETUP:
                drawImage3(0, 0, WINSCREEN_HEIGHT, WINSCREEN_WIDTH, WinScreen_data);
                if(time < 1) 
                {
                    score = (lives + 1) * 1500;
                }
                else
                {
                score = ((time * 12) * (lives + 1) * 4) / 2;
                }
                int i = 0;
                int x = score;
                while (x != 0) {
                    i++;
                    x /= 10;
                }
                char buffer[i + 2];
                sprintf(buffer, "%d", score + 100);
                drawString(150, 55, buffer, YELLOW);
                state = WIN;
                break;
            
        }
    }
        
}
