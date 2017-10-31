// Megan Phibbons 2B
#include <allegro.h>
#include <iostream>
#include <vector>

using namespace std;

void init();
void deinit();

struct block
    {
        int x1;
        int y1;
        int x2;
        int y2;
        int state;
        block(int firstX, int firstY, int secondX, int secondY, int blockState)
        {
            x1 = firstX;
            y1 = firstY;
            x2 = secondX;
            y2 = secondY;
            state = blockState;
        }
    };

int main()
{
    init();

    BITMAP *board = create_bitmap(SCREEN_W, SCREEN_H);

    const int BACKGROUND_COLOR = makecol(153, 255, 255);
    const int FRAME_COLOR = makecol(204, 153, 255);
    const int BALL_RADIUS = 10;
    const int BALL_COLOR = makecol(153, 0, 153);
    const int BOARD_X1 = 15, BOARD_Y1 = 15, BOARD_X2 = SCREEN_W - 15, BOARD_Y2 = SCREEN_H - 40;
    const int PADDLE_Y = BOARD_Y2 - 40;

    int paddleX = SCREEN_W / 2 - 40;
    int ballX = paddleX + 40;
    int ballY = PADDLE_Y - BALL_RADIUS - 1;
    int deltaBallX = 3, deltaBallY = -3;
    int numBlocks = 38;
    int numLives = 3;
    bool lost = false;
    bool won = false;

    enum brickState
    {
        STRONG = 0xFF3399, MIDDLE = 0xFF99CC, WEAK = 0xFFCCE5, BROKEN = 0x99FFFF
    };

    vector<block> blockStates;

    int x = BOARD_X1 + 12, y = BOARD_Y1 + 10;
    for(int i = 0; i < 10; i++)
    {
        blockStates.push_back(block(x, y, x + 45, y + 15, STRONG));
        x += 60;
    }
    x = BOARD_X1 + 41;
    y += 30;
    for(int i = 0; i < 9; i++)
    {
        blockStates.push_back(block(x, y, x + 45, y + 15, STRONG));
        x += 60;
    }
    x = BOARD_X1 + 12;
    y += 30;
    for(int i = 0; i < 10; i++)
    {
        blockStates.push_back(block(x, y, x + 45, y + 15, STRONG));
        x += 60;
    }
    x = BOARD_X1 + 41;
    y += 30;
    for(int i = 0; i < 9; i++)
    {
        blockStates.push_back(block(x, y, x + 45, y + 15, STRONG));
        x += 60;
    }

    while(!key[KEY_SPACE])
    {
        rectfill(board, 0, 0, SCREEN_W, SCREEN_H, makecol(51, 153, 255));
        textprintf_centre_ex(board, font, SCREEN_W / 2, SCREEN_H / 2 - 15, makecol(0, 0, 0), -1, "Welcome to breakout! Use the left and right arrow keys");
        textprintf_centre_ex(board, font, SCREEN_W / 2, SCREEN_H / 2, makecol(0, 0, 0), -1, "to control the paddle and break all of the bricks!");
        textprintf_centre_ex(board, font, SCREEN_W / 2, SCREEN_H / 2 + 15, makecol(0, 0, 0), -1, "Press the space bar to begin.");
        blit(board, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }

    while(!key[KEY_ESC])
    {
        while(!lost && !won)
        {
            rectfill(board, 0, 0, SCREEN_W, SCREEN_H, FRAME_COLOR);
            rectfill(board, BOARD_X1, BOARD_Y1, BOARD_X2, BOARD_Y2, BACKGROUND_COLOR);
            int triX = BOARD_X1 + 5;
            int triY = BOARD_Y2 + 10;
            for(int i = 0; i < numLives; i++)
            {
                triangle(board, triX, triY, triX + 7, triY + 7, triX - 7, triY + 7, makecol(255, 0, 0));
                triX += 10;
                triangle(board, triX, triY, triX + 7, triY + 7, triX - 7, triY + 7, makecol(255, 0, 0));
                triX -= 10;
                triY += 7;
                triangle(board, triX - 7, triY, triX + 17, triY, triX + 5, triY + 15, makecol(255, 0, 0));
                triX += 30;
                triY -= 7;
            }
            for(int i = 0; i < numBlocks; i++)
            {
                if((ballX - BALL_RADIUS <= blockStates[i].x2 && ballX + BALL_RADIUS >= blockStates[i].x1) &&
                   (ballY + BALL_RADIUS >= blockStates[i].y1 && ballY - BALL_RADIUS <= blockStates[i].y2))
                {
                    deltaBallY *= -1;
                    if(blockStates[i].state == STRONG)
                        blockStates[i].state = MIDDLE;
                    else if(blockStates[i].state == MIDDLE)
                        blockStates[i].state = WEAK;
                    else if(blockStates[i].state == WEAK)
                        {
                            blockStates.erase(blockStates.begin() + i);
                            numBlocks--;
                        }
                }
            }
            for(int i = 0; i < numBlocks; i++)
            {
                rectfill(board, blockStates[i].x1, blockStates[i].y1, blockStates[i].x2, blockStates[i].y2, blockStates[i].state);
            }
            rectfill(board, paddleX, PADDLE_Y, paddleX + 80, PADDLE_Y + 15, FRAME_COLOR);
            circlefill(board, ballX, ballY, BALL_RADIUS, BALL_COLOR);
            if(ballY - BALL_RADIUS <= BOARD_Y1)
                deltaBallY *= -1;
            if(ballX - BALL_RADIUS <= BOARD_X1 || ballX + BALL_RADIUS >= BOARD_X2)
                deltaBallX *= -1;
            if(ballY + BALL_RADIUS >= PADDLE_Y && !(ballY + BALL_RADIUS >= PADDLE_Y + 15) && ballX + BALL_RADIUS >= paddleX && ballX + BALL_RADIUS <= paddleX + 80)
            {
                deltaBallY *= -1;
            }
            if(ballY - BALL_RADIUS >= SCREEN_H + 25)
            {
                paddleX = SCREEN_W / 2 - 40;
                ballX = paddleX + 40;
                ballY = PADDLE_Y - BALL_RADIUS - 1;
                numLives --;
            }
            if(numLives == 0)
                lost = true;
            if(numBlocks == 0)
                won = true;
            if(key[KEY_RIGHT] && paddleX + 80 <= BOARD_X2)
                paddleX += 3;
            else if(key[KEY_LEFT] && paddleX >= BOARD_X1)
                paddleX -= 3;
            ballY += deltaBallY;
            ballX += deltaBallX;
            rest(1);
            blit(board, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        }
        if(lost)
        {
            rectfill(board, 0, 0, SCREEN_W, SCREEN_H, makecol(51, 153, 255));
            int numBrokenBlocks = 38 - numBlocks;
            textprintf_centre_ex(board, font, SCREEN_W / 2, SCREEN_H / 2, makecol(0, 0, 0), -1, "Sorry! You lost! You broke %d blocks! Good game!", numBrokenBlocks);
            blit(board, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        }
        else
        {
            rectfill(board, 0, 0, SCREEN_W, SCREEN_H, makecol(51, 153, 255));
            textprintf_centre_ex(board, font, SCREEN_W / 2, SCREEN_H / 2, makecol(0, 0, 0), -1, "Congrats! You won! You broke all of the blocks! Good game!");
            blit(board, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        }
    }
    deinit();
    return 0;
}
END_OF_MAIN()

void init()
{
    int depth, res;
    allegro_init();
    depth = desktop_color_depth();
    if(depth == 0)
        depth = 32;
    set_color_depth(depth);
    res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
    if(res != 0)
    {
        allegro_message(allegro_error);
        exit(-1);
    }
    install_timer();
    install_keyboard();
    install_mouse();
}

void deinit()
{
    clear_keybuf();
}
