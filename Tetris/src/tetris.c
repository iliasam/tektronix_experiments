// Tetris for TDS540 oscilloscope.
// Based on this project: https://github.com/c0pperdragon/ArduinoGameConsole

#include "stdint.h"

#define TITLE_SPACING   3

#define TITLE_WIDTH     20
#define TITLE_HEIGHT    20

#define SCREEN_OFFSET_X 10

#define PFWIDTH 10
#define PFHEIGHT 20

#define MOVE_PERIOD     20

#define TILE_J 1
#define TILE_L 1
#define TILE_O 1
#define TILE_S 1
#define TILE_T 1
#define TILE_Z 1

#define TILE_H0 1
#define TILE_H1 1
#define TILE_H2 1
#define TILE_H3 1

#define TILE_I0 1
#define TILE_I1 1
#define TILE_I2 1
#define TILE_I3 1

#define KEY_LEFT        0xC00A //Key "4"
#define KEY_RIGHT       0xC00C //Key "6"
#define KEY_UP          0xC00E //Key "8"
#define KEY_DOWN        0xC00B //Key "5"

extern void _ringBell(void);
extern void _horzLine(short lineX1,short lineY2,short length,char line_pattern);
extern void _pokeDiag(uint16_t lineX1,short lineY1,short lineX2,short lineY2);
extern void _vertLine(short line_x,short line_y1,short length, uint8_t mask);
extern void _dsyDiagFillTextPlane(int param_1,char param_2);
extern int _taskDelay(int delay);
extern uint16_t _millisecondsToTicks(int delay);
extern void _clearDisplay(void);
extern uint32_t _tickGet(void);
extern int _printf(const char *format, ...);
extern int _sprintf(char *__s,char *__format,...);
extern void _GpibInput(char *param_1);


//************************************************************************************************

uint8_t pieceShapes[] =
{
    // 0 = The J piece
    0, 0, TILE_J, 0,
    0, 0, TILE_J, 0,
    0, TILE_J, TILE_J, 0,
    0, 0, 0, 0,
    //
    0, 0, 0, 0,
    0, TILE_J, 0, 0,
    0, TILE_J, TILE_J, TILE_J,
    0, 0, 0, 0,
    //
    0, 0, 0, 0,
    0, TILE_J, TILE_J, 0,
    0, TILE_J, 0, 0,
    0, TILE_J, 0, 0,
    //
    0, 0, 0, 0,
    TILE_J, TILE_J, TILE_J, 0,
    0, 0, TILE_J, 0,
    0, 0, 0, 0,
    //
    // 1 = The L piece
    0, TILE_L, 0, 0,
    0, TILE_L, 0, 0,
    0, TILE_L, TILE_L, 0,
    0, 0, 0, 0,
    //
    0, 0, 0, 0,
    0, TILE_L, TILE_L, TILE_L,
    0, TILE_L, 0, 0,
    0, 0, 0, 0,
    //
    0, 0, 0, 0,
    0, TILE_L, TILE_L, 0,
    0, 0, TILE_L, 0,
    0, 0, TILE_L, 0,
    //
    0, 0, 0, 0,
    0, 0, TILE_L, 0,
    TILE_L, TILE_L, TILE_L, 0,
    0, 0, 0, 0,
    //
    // 2 = The O piece
    0, 0, 0, 0,
    0, TILE_O, TILE_O, 0,
    0, TILE_O, TILE_O, 0,
    0, 0, 0, 0,
    //
    0, 0, 0, 0,
    0, TILE_O, TILE_O, 0,
    0, TILE_O, TILE_O, 0,
    0, 0, 0, 0,
    //
    0, 0, 0, 0,
    0, TILE_O, TILE_O, 0,
    0, TILE_O, TILE_O, 0,
    0, 0, 0, 0,
    //
    0, 0, 0, 0,
    0, TILE_O, TILE_O, 0,
    0, TILE_O, TILE_O, 0,
    0, 0, 0, 0,
    //
    // 3 = The S piece
    0, 0, 0, 0,
    0, TILE_S, TILE_S, 0,
    TILE_S, TILE_S, 0, 0,
    0, 0, 0, 0,
    //
    0, TILE_S, 0, 0,
    0, TILE_S, TILE_S, 0,
    0, 0, TILE_S, 0,
    0, 0, 0, 0,
    //
    0, 0, 0, 0,
    0, TILE_S, TILE_S, 0,
    TILE_S, TILE_S, 0, 0,
    0, 0, 0, 0,
    //
    0, TILE_S, 0, 0,
    0, TILE_S, TILE_S, 0,
    0, 0, TILE_S, 0,
    0, 0, 0, 0,
    //
    // 4 = The T piece
    0, 0, 0, 0,
    TILE_T, TILE_T, TILE_T, 0,
    0, TILE_T, 0, 0,
    0, 0, 0, 0,
    //
    0,  TILE_T, 0, 0,
    TILE_T, TILE_T, 0, 0,
    0, TILE_T, 0, 0,
    0, 0, 0, 0,
    //
    0, TILE_T, 0, 0,
    TILE_T, TILE_T, TILE_T, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    //
    0, TILE_T, 0, 0,
    0, TILE_T, TILE_T, 0,
    0, TILE_T, 0, 0,
    0, 0, 0, 0,
    //
    // 5 = The Z piece
    0, 0, 0, 0,
    TILE_Z, TILE_Z, 0, 0,
    0, TILE_Z, TILE_Z, 0,
    0, 0, 0, 0,
    //
    0, 0, TILE_Z, 0,
    0, TILE_Z, TILE_Z, 0,
    0, TILE_Z, 0, 0,
    0, 0, 0, 0,
    //
    0, 0, 0, 0,
    TILE_Z, TILE_Z, 0, 0,
    0, TILE_Z, TILE_Z, 0,
    0, 0, 0, 0,
    //
    0, 0, TILE_Z, 0,
    0, TILE_Z, TILE_Z, 0,
    0, TILE_Z, 0, 0,
    0, 0, 0, 0,
    //
    // 6 = The I piece
    0, 0, 0, 0,
    0, 0, 0, 0,
    TILE_H0, TILE_H1, TILE_H2, TILE_H3,
    0, 0, 0, 0,
    //
    0, TILE_I0, 0, 0,
    0, TILE_I1, 0, 0,
    0, TILE_I2, 0, 0,
    0, TILE_I3, 0, 0,
    //
    0, 0, 0, 0,
    0, 0, 0, 0,
    TILE_H0, TILE_H1, TILE_H2, TILE_H3,
    0, 0, 0, 0,
    //
    0, TILE_I0, 0, 0,
    0, TILE_I1, 0, 0,
    0, TILE_I2, 0, 0,
    0, TILE_I3, 0, 0,
};


uint8_t playfield[PFWIDTH][PFHEIGHT] = {0};
uint8_t nextpieces[7] = {0};
uint8_t playfield_changed = 0;
uint8_t piecex = 0;
uint8_t piecey = 0;
uint8_t nextcursor = 0;
uint8_t piecetype = 0;
uint8_t piecerotation = 0;
uint8_t turntimeleft = MOVE_PERIOD;
uint8_t speedup = 0;
uint16_t tetris_lines = 0;
uint8_t game_over = 0;

uint8_t test_cnt = 0;

uint8_t prev_piecex = 0;
uint8_t prev_piecey = 0;
uint8_t prev_piecerotation = 0;
uint8_t prev_piecetype = 0;

void draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t color);
void paint_tile(uint8_t x, uint8_t y, uint8_t t);
void paint_playfield(void);
void paint_next_piece(void);
void generate_next_pieces(void);
uint8_t set_piece(uint8_t x, uint8_t y, uint8_t typ, uint8_t rot, uint8_t toPlayfield, uint8_t toScreen);
void clear_piece(uint8_t x, uint8_t y, uint8_t typ, uint8_t rot);
uint8_t remove_complete_rows(uint8_t onlyTest, uint8_t blink);
uint8_t game_final_fix(void);
void tetris_cycle(uint16_t key);
void tetris_prepare(void);
uint16_t read_key_func(void);
void draw_lines_text(void);



//************************************************************************************************

// Main function, called by user
int tetris(void)
{
    _printf("TETRIS FOR TDS5XX OSCILLOSCOPE. ILIASAM 2023.\n");
    //Clear screen
    _dsyDiagFillTextPlane(0,0);
    _clearDisplay();
    
    tetris_prepare();
    uint16_t delay_ticks = _millisecondsToTicks(10);
    
    uint8_t i = 0;
    while (game_over == 0)
    {
        uint16_t key = read_key_func();
        tetris_cycle(key);
        _taskDelay(delay_ticks);
    }
    
    _GpibInput("mess:state OFF");
    _ringBell();
    _printf("TETRIS DONE\n");
    return 0;
}

// Return read RAM value
uint16_t read_key_func(void)
{
    uint16_t data = (*(uint16_t*)0x0558a274);
    (*(uint16_t*)0x0558a274) = 0; 
    return data;
}


void tetris_prepare(void)
{
    int i,j;

    // start one round
    for (i=0; i<PFHEIGHT; i++)
        for (j=0; j<PFWIDTH; j++)
            playfield[j][i] = 0;
    playfield_changed = 1;
    
    for (i=0; i<6; i++)
        nextpieces[i] = 0;

    generate_next_pieces();
    piecex = 4;
    piecey = 1;
    piecerotation = 0;
    nextcursor = 0;
    piecetype = nextpieces[nextcursor++];
    speedup = 0;
    tetris_lines = 0;
    turntimeleft = MOVE_PERIOD;
    game_over = 0;
    paint_playfield();
    paint_next_piece();

    uint16_t x_right = (TITLE_SPACING + TITLE_WIDTH) * PFWIDTH + SCREEN_OFFSET_X + 1;
    uint16_t y_down = (TITLE_SPACING + TITLE_HEIGHT) * PFHEIGHT + 2;
    
    _vertLine(SCREEN_OFFSET_X - 2, 0, y_down, 0xF);//left line
    _vertLine(x_right, 0, y_down, 0xF);//right line
    _horzLine(SCREEN_OFFSET_X, y_down, (x_right - SCREEN_OFFSET_X), 0xF);
    
    _GpibInput("mess:box 350,200,480,250");
    
    draw_lines_text();
}



void tetris_cycle(uint16_t key)
{
    uint8_t fix_happen = 0;
    
    if ((key == KEY_LEFT) && piecex>0)
    {
        if (!set_piece(piecex-1,piecey,piecetype,piecerotation,0,0))
        {
            piecex--;
        }
    }
    if ((key == KEY_RIGHT) && piecex<100)
    {
        if (!set_piece(piecex+1,piecey,piecetype,piecerotation,0,0))
        {
            piecex++;
        }
    }
    if (key == KEY_UP)
    {
      if (!set_piece(piecex,piecey,piecetype,(piecerotation+1)&3,0,0))
      {   piecerotation = (piecerotation+1) & 3;
      }
      else if (!set_piece(piecex-1,piecey,piecetype,(piecerotation+1)&3,0,0))
      {   piecerotation = (piecerotation+1) & 3;
          piecex--;
      }
      else if (!set_piece(piecex+1,piecey,piecetype,(piecerotation+1)&3,0,0))
      {   piecerotation = (piecerotation+1) & 3;
          piecex++;
      }
    }
    if (key == KEY_DOWN)
    {
        speedup = 1;
        turntimeleft = 0;
    }
    
    
    // game time progressing
    if (turntimeleft > 0)
    {
        turntimeleft--;
    }
    else
    {
        if (speedup)
            turntimeleft = 1;
        else
            turntimeleft = MOVE_PERIOD;
        fix_happen = game_final_fix();
    }

    paint_playfield();
    
    if (fix_happen == 0)
    {
        if ((piecex != prev_piecex) || (prev_piecey != piecey) ||
            (prev_piecerotation != piecerotation)  || (prev_piecetype != piecetype) )
        {
            clear_piece(prev_piecex, prev_piecey, prev_piecetype, prev_piecerotation);
            set_piece(piecex, piecey, piecetype, piecerotation, 0, 1);
        }
    }
     
    prev_piecex = piecex;
    prev_piecey = piecey;
    prev_piecerotation = piecerotation;
    prev_piecetype = piecetype;
}

//Move piece down or fix it
uint8_t game_final_fix(void)
{
    // piece moves down
    if (!set_piece(piecex,piecey+1,piecetype,piecerotation,0,0))
    {
        piecey++;
    }
    else
    {
        speedup = 0;
        // piece can not move down
        // piece locks in place
        set_piece(piecex,piecey,piecetype,piecerotation,1,0);
        paint_playfield();

        // test for removal of full tetris_lines
        uint8_t removed = remove_complete_rows(1, 1);
        if (removed>0)
        {

            uint8_t rem_cnt;
            for (rem_cnt = 0; rem_cnt < 5; rem_cnt++)
            {
                paint_playfield();
                remove_complete_rows(0, 1);
            }

            // track number of removed tetris_lines
            tetris_lines += removed;
            draw_lines_text();
        }

        piecex = 4;
        piecey = 1;
        piecetype = nextpieces[nextcursor++];
        piecerotation = 0;

        if (nextcursor>6)
        {
            generate_next_pieces();
            nextcursor=0;
        }

        paint_next_piece();
        // if the next piece can not be spawned, the game is over
        if (set_piece(piecex, piecey, piecetype, piecerotation, 0, 0))
        {
            game_over = 1;
            return 0;
        }

        return 1;
    }

    return 0;
}

void generate_next_pieces(void)
{
    uint8_t i;
    for (i = 0; i < 7; i++)
    {
        nextpieces[i] = i;
    }
    
    uint8_t random = (uint8_t)(_tickGet() % 7);
    for (i = 0; i < 6; i++)
    {
        random++;
        if (random > 6)
            random = 0;
        
        uint8_t dummy = nextpieces[i];
        nextpieces[i] = nextpieces[random];
        nextpieces[random] = dummy;
    }
}

void paint_next_piece(void)
{
    uint8_t x;
    uint8_t y;
    uint8_t type = nextpieces[nextcursor];
    for (y=0; y<4; y++)
    {   for (x=0; x<4; x++)
        {
            uint8_t tile = pieceShapes[((int)type)*64+x+y*4];
            paint_tile(x+15,y+2,tile);
        }
    }
}

void clear_piece(uint8_t x, uint8_t y, uint8_t typ, uint8_t rot)
{
    uint8_t i;

    for (i=0; i<16; i++)
    {
        uint8_t tile = pieceShapes[((int)typ)*64+rot*16+i];
        if (tile!=0)
        {
            uint8_t tx = x - 1 + i%4;
            uint8_t ty = y - 1 + i/4;

            if (tx>=PFWIDTH || ty>=PFHEIGHT) {}
            else
            {
                paint_tile(tx, ty, 0);//clear
            }
        }
    }
}

uint8_t set_piece(uint8_t x, uint8_t y, uint8_t typ, uint8_t rot, uint8_t toPlayfield, uint8_t toScreen)
{
    uint8_t anycollisions = 0;
    uint8_t i;

    for (i=0; i<16; i++)
    {
        uint8_t tile = pieceShapes[((int)typ)*64+rot*16+i];
        if (tile!=0)
        {
            uint8_t tx = x - 1 + i%4;
            uint8_t ty = y - 1 + i/4;

            if (tx>=PFWIDTH || ty>=PFHEIGHT) { anycollisions=1; }
            else
            {   if (playfield[tx][ty]!=0) { anycollisions=1; }
                if (toPlayfield) 
                { 
                    playfield[tx][ty] = tile;
                    playfield_changed = 1;
                }
                if (toScreen)
                {
                    paint_tile(tx, ty, tile);
                }
            }
        }
    }
    return anycollisions;
}

uint8_t remove_complete_rows(uint8_t onlyTest, uint8_t blink)
{
    uint8_t i,j;
    uint8_t removedrows=0;

    for (i=PFHEIGHT; i!=255; i--)
    {
        uint8_t completerow = 1;
        for (j=0; j<PFWIDTH; j++)
        {
            if (playfield[j][i]==0) { completerow=0; }
        }

        if (completerow)
        {   removedrows++;
            if (blink && i>=4)
            {   for (j=0; j<PFWIDTH; j++)
                {
                    paint_tile(2+j, i-4, 0);
                }
            }
        }
        else
        {   if (!onlyTest)
            {   for (j=0; j<PFWIDTH; j++)
                {
                    playfield[j][i+removedrows] = playfield[j][i];
                    playfield_changed = 1;
                }
            }
        }
    }
    if (!onlyTest)
    {   for (i=0; i<removedrows; i++)
        {
            for (j=0; j<PFWIDTH; j++) { playfield[j][i] = 0; }
            playfield_changed = 1;
        }
    }
    return removedrows;
}

void draw_lines_text(void)
{
    char tmp_buf[32];
    _sprintf(tmp_buf, "mess:show \"\n  LINES: %04d\"", tetris_lines);
    _GpibInput(tmp_buf);
}

//Painting
//++++++++++++++++++++++++++++++++++++++++++

void paint_playfield(void)
{
    uint8_t x=0;
    uint8_t y=0;
    
    if (playfield_changed == 0)
        return;
    
    for (x = 0; x < PFWIDTH; x++)
    {
        for (y = 0; y < PFHEIGHT; y++)
        {
            paint_tile(x, y, playfield[x][y]);
        }
    }
    playfield_changed = 0;
}

void paint_tile(uint8_t x, uint8_t y, uint8_t t)
{
    uint8_t color = 0;
    if (t != 0)
        color = 0xFF;

    uint16_t pos_x = x * (TITLE_WIDTH + TITLE_SPACING) + SCREEN_OFFSET_X;
    uint16_t pos_y = y * (TITLE_HEIGHT + TITLE_SPACING);
    draw_rectangle(pos_x, pos_y, TITLE_WIDTH, TITLE_HEIGHT, color);
}

void draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t color)
{
    _horzLine(x + 5, y + 10, 10, color); //Center line  not good solution
    
    _horzLine(x, y, width, color);
    _horzLine(x, y + height, width, color);
    
    _vertLine(x,y,height,color);
    _vertLine(x + width,y,height,color);
}