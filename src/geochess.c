//===================================================================================
//
//                                  GEOCHESS
//
// geoChess is a chess game for GEOS under the Commodore 64 and 128 computers
//
// Written by Scott Hutter
// Nov 2023
// 
// You are free to modify this code as desired, as long as original author credit
// is mentioned for both the geos code and the included AI engines
//===================================================================================

#include <geos.h>
//#include "geochess-res.h"
#include "geochess.h"
#include "geochess-ai.h"
#include <string.h>

void main(void)
{
    char msg[16];
    const char* versionString = TOSTRING(VERSION);

	osType = get_ostype();
    
    strcpy(msg, "GeoChess v");
    strcat(msg, versionString);

    hook_into_system();
    atexit(&remove_hook); 
    
    if(ISGEOS128)
	{	
		if(C128_40_COL_MODE)
        {
			DlgBoxOk (msg, "Commodore 128 40 column mode");
            sc_width = 1;
            ((struct menu *)&mainMenu)->size.right = 27;
            ((struct menu *)&mainMenu)->items[0].rest = (void *)&subMenu128_40;
        }
		else {
			DlgBoxOk (msg, "Commodore 128 80 column mode");
            sc_width = 2;
            ((struct menu *)&mainMenu)->size.right = 35;
            ((struct menu *)&mainMenu)->items[0].rest = (void *)&subMenu128_80;
        }
	}
	else
    {
		DlgBoxOk (msg, "For the Commodore 64");
        sc_width = 1;
        ((struct menu *)&mainMenu)->size.right = 27;
        ((struct menu *)&mainMenu)->items[0].rest = (void *)&subMenu64;
    }

    memset(gboard, 0, sizeof(gboard));

    LoadFont();
    InitScreen();
    NewGame();
    MainLoop();
}

void LoadFont(void)
{
    char fname[15] = "geochessfont40";

    if (ISGEOS128)
    {
        if(C128_80_COL_MODE) {
            fname[12] = '8';
        }
    }

    if(OpenRecordFile(fname) != 0)
    {
        DlgBoxOk ("Error accessing fonts.", "'geochessfont40/80' not found.");
        EnterDeskTop();
    }
	
	PointRecord(16);
	ReadRecord(fontbuffer, FONTBUFFERSIZE);
	CloseRecordFile();
}

void DrawRect(unsigned char pattern, struct window *square) 
{
    SetPattern(pattern);
 
    square->left = square->left  * sc_width;
    square->right = square->right * sc_width;
 
    InitDrawWindow(square);
    Rectangle();
}

void DrawStdRect(unsigned char pattern, struct window *square) 
{
    SetPattern(pattern);
    InitDrawWindow(square);
    Rectangle();
}

unsigned char GetPieceChar(unsigned char row, unsigned char col) 
{
    unsigned char piece;

    switch(gboard[row][col][0])
    {
        case WHT_KING:
            piece = (gboard[row][col][1] == WHT ? WHT_KING_WHT_SQR : WHT_KING_BLK_SQR);
            break;
        case WHT_QUEEN:
            piece = (gboard[row][col][1] == WHT ? WHT_QUEEN_WHT_SQR: WHT_QUEEN_BLK_SQR);
            break;
        case WHT_BISHOP:
            piece = (gboard[row][col][1] == WHT ? WHT_BISHOP_WHT_SQR: WHT_BISHOP_BLK_SQR);
            break;
        case WHT_KNIGHT:
            piece = (gboard[row][col][1] == WHT ? WHT_KNIGHT_WHT_SQR: WHT_KNIGHT_BLK_SQR);
            break;
        case WHT_ROOK:
            piece = (gboard[row][col][1] == WHT ? WHT_ROOK_WHT_SQR: WHT_ROOK_BLK_SQR);
            break;
        case WHT_PAWN:
            piece = (gboard[row][col][1] == WHT ? WHT_PAWN_WHT_SQR: WHT_PAWN_BLK_SQR);
            break;
        case BLK_KING:
            piece = (gboard[row][col][1] == WHT ? BLK_KING_WHT_SQR : BLK_KING_BLK_SQR);
            break;
        case BLK_QUEEN:
            piece = (gboard[row][col][1] == WHT ? BLK_QUEEN_WHT_SQR: BLK_QUEEN_BLK_SQR);
            break;
        case BLK_BISHOP:
            piece = (gboard[row][col][1] == WHT ? BLK_BISHOP_WHT_SQR: BLK_BISHOP_BLK_SQR);
            break;
        case BLK_KNIGHT:
            piece = (gboard[row][col][1] == WHT ? BLK_KNIGHT_WHT_SQR: BLK_KNIGHT_BLK_SQR);
            break;
        case BLK_ROOK:
            piece = (gboard[row][col][1] == WHT ? BLK_ROOK_WHT_SQR: BLK_ROOK_BLK_SQR);
            break;
        case BLK_PAWN:
            piece = (gboard[row][col][1] == WHT ? BLK_PAWN_WHT_SQR: BLK_PAWN_BLK_SQR);
            break;
    }

    return piece;
}

void InitScreen(void) {

    char *s = "  GeoChess  ";
	unsigned short title_left = 0;
    unsigned short screen_width = 0;
    struct window rect;   

    screen_width = (sc_width == 1 ? 319 : 639);
    title_left = (sc_width == 1 ? 200 : 400);

    // clear the background area at top
    rect.top = 0;
    rect.bot = 20;
    rect.left = 0;
    rect.right = screen_width;
    DrawStdRect(2,&rect);

    HorizontalLine(255, 20, 0, screen_width);
    
    // clear game area
    rect.top = 21;
    rect.bot = 199;
    rect.left = 0;
    rect.right = screen_width;
    DrawStdRect(0,&rect);

    // clear area under title
    rect.top = 0;
    rect.bot = 15;
    rect.left = title_left;
    rect.right = screen_width;
    DrawStdRect(0,&rect);

    HorizontalLine(255, 1, title_left, screen_width);
	HorizontalLine(255, 4, title_left, screen_width);
	HorizontalLine(255, 6, title_left, screen_width);
	HorizontalLine(255, 8, title_left, screen_width);
	HorizontalLine(255, 10, title_left, screen_width);
	HorizontalLine(255, 13, title_left, screen_width);
	
	UseSystemFont();
	PutString (s, 9, title_left+20);

}

void InitBoard(unsigned char initalPosition) 
{
    unsigned int i, j;
    unsigned int start_top_offset = BOARD_TOP+1;
    unsigned int start_left_offset = BOARD_LEFT+1;
    unsigned int top_offset = start_top_offset;
    unsigned int left_offset = start_left_offset;
    unsigned char toggle;
    unsigned char piece;          
    struct window rect;

    // clear the area
    rect.top = BOARD_TOP;
    rect.left = BOARD_LEFT;
    rect.bot = BOARD_TOP+144;
    rect.right = BOARD_LEFT+160;
    DrawRect(0,&rect);

    // Draw chess board

    // Horizonal lines
    j=BOARD_TOP;
    for(i=0;i<9;i++)
    {
        HorizontalLine (255, j, BOARD_LEFT * sc_width, (BOARD_LEFT*sc_width)+(160*sc_width)); 
       j+=SQUARE_HEIGHT+2;
    }

    // Veritcal Lines
    j=BOARD_LEFT * sc_width;
    for(i=0;i<9;i++)
    {
        VerticalLine(255, BOARD_TOP, BOARD_TOP+144, j);
        j+=(SQUARE_WIDTH*sc_width)+(2*sc_width);
    }

    // set up the click regions (squares)
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            vboard[i][j].top = top_offset;
            vboard[i][j].bot = top_offset + SQUARE_HEIGHT;
            vboard[i][j].left = left_offset;
            vboard[i][j].right = left_offset + SQUARE_WIDTH;

            left_offset += SQUARE_WIDTH + 2;
        }

        left_offset = start_left_offset;
        top_offset += SQUARE_HEIGHT + 2;
    }

    // color the squares
    toggle = 0;
    for(i=0;i<8;i++)
    {
        for(j=0;j<8;j++)
        {   
            DrawRect(toggle, &vboard[i][j]);
            gboard[i][j][1] = (toggle == 0 ? WHT : BLK);
            toggle = (toggle == 1 ? 0 : 1);
        }
        toggle = (toggle == 1 ? 0 : 1);

    }

    // grid notations
    UseSystemFont();
    PutChar('a', 184, 31 * sc_width);
    PutChar('b', 184, 51 * sc_width);
    PutChar('c', 184, 71 * sc_width);
    PutChar('d', 184, 91 * sc_width);
    PutChar('e', 184, 111 * sc_width);
    PutChar('f', 184, 131 * sc_width);
    PutChar('g', 184, 151 * sc_width);
    PutChar('h', 184, 171 * sc_width);

    PutChar('8', 44, 10 * sc_width);
    PutChar('7', 62, 10 * sc_width);
    PutChar('6', 80, 10 * sc_width);
    PutChar('5', 98, 10 * sc_width);
    PutChar('4', 118, 10 * sc_width);
    PutChar('3', 136, 10 * sc_width);
    PutChar('2', 154, 10 * sc_width);
    PutChar('1', 172, 10 * sc_width);

    if (initalPosition == 0)
    {
        // initial piece positions
        gboard[0][0][0] = BLK_ROOK;
        gboard[0][1][0] = BLK_KNIGHT;
        gboard[0][2][0] = BLK_BISHOP;
        gboard[0][3][0] = BLK_QUEEN;
        gboard[0][4][0] = BLK_KING;
        gboard[0][5][0] = BLK_BISHOP;
        gboard[0][6][0] = BLK_KNIGHT;
        gboard[0][7][0] = BLK_ROOK;
        gboard[1][0][0] = BLK_PAWN;
        gboard[1][1][0] = BLK_PAWN;
        gboard[1][2][0] = BLK_PAWN;
        gboard[1][3][0] = BLK_PAWN;
        gboard[1][4][0] = BLK_PAWN;
        gboard[1][5][0] = BLK_PAWN;
        gboard[1][6][0] = BLK_PAWN;
        gboard[1][7][0] = BLK_PAWN;

        gboard[6][0][0] = WHT_PAWN;
        gboard[6][1][0] = WHT_PAWN;
        gboard[6][2][0] = WHT_PAWN;
        gboard[6][3][0] = WHT_PAWN;
        gboard[6][4][0] = WHT_PAWN;
        gboard[6][5][0] = WHT_PAWN;
        gboard[6][6][0] = WHT_PAWN;
        gboard[6][7][0] = WHT_PAWN;
        gboard[7][0][0] = WHT_ROOK;
        gboard[7][1][0] = WHT_KNIGHT;
        gboard[7][2][0] = WHT_BISHOP;
        gboard[7][3][0] = WHT_QUEEN;
        gboard[7][4][0] = WHT_KING;
        gboard[7][5][0] = WHT_BISHOP;
        gboard[7][6][0] = WHT_KNIGHT;
        gboard[7][7][0] = WHT_ROOK;
    }

    // switch character sets
    LoadCharSet ((struct fontdesc *)(fontbuffer));

    // now place pieces according the gboard matrix
    for(i=0;i<8;i++)
    {
        for(j=0;j<8;j++)
        {   
            piece = GetPieceChar(i,j);
            if(piece != EMPTY)
                PutChar(piece, 50+(18*i), (27 * sc_width) + ((20*j) * sc_width) ) ;
        }
    }

}

void InitMovePanel(void) 
{
    struct window rect;

    // clear the area
    rect.top = 44;
    rect.left = 208;
    rect.bot = 175;
    rect.right = 310;
    DrawRect(0,&rect);

    // Draw panel borders and lines
    HorizontalLine (255, 33, 207 * sc_width, 312 * sc_width);
    HorizontalLine (255, 35, 207 * sc_width, 312 * sc_width);
    HorizontalLine (255, 37, 207 * sc_width, 312 * sc_width);
    HorizontalLine (255, 39, 207 * sc_width, 312 * sc_width);
    HorizontalLine (255, 41, 207 * sc_width, 312 * sc_width);
    HorizontalLine (255, 43, 207 * sc_width, 312 * sc_width);
    HorizontalLine (255, 45, 207 * sc_width, 312 * sc_width);
    HorizontalLine (255, 177, 207 * sc_width, 312 * sc_width);
    HorizontalLine (255, 178, 209 * sc_width, 313 * sc_width);
    
    VerticalLine(255, 33, 177, 207 * sc_width);
    VerticalLine(255, 33, 177, 312 * sc_width);
    VerticalLine(255, 40, 178, 313 * sc_width);
    VerticalLine(255, 45, 174, 257 * sc_width);

    //rect.top = 34;
    //rect.left = 212;
    //rect.bot = 42;
    //rect.right = 268;
    //DrawRect(0,&rect);

    UseSystemFont();
    PutString("  move log  ", 40, 215 * sc_width);

}

void NewGame(void)
{
    notation_row_count = 0;
    notation_text_position = 55;

    InitBoard(0);
    InitMovePanel();
    engine_init();
    UpdateStatus("Your move.");
    gameState = INPROGRESS;
    DoMenu((struct menu *)&mainMenu);
}

void UpdateNotation(unsigned char player, unsigned char src_row, unsigned char src_col, unsigned char dest_row, unsigned char dest_col) 
{
    char *current_move;
    struct window rect;

    current_move = user_move;

    if(player == 1)
        current_move = ai_move;
        

    current_move[0] = 97+src_col;
    current_move[1] = 48+(8-src_row);

    if(dest_row != 0 )
    {
        current_move[2] = 97+dest_col;
        current_move[3] = 48+(8-dest_row);
    }
    else
        current_move[2] = 0;

    current_move[4] = 0;

    if(notation_row_count == 11)
    {
        // clear the page
        rect.top = 44;
        rect.left = 208;
        rect.bot = 175;
        rect.right = 310;
        DrawRect(0,&rect);
        notation_row_count = 0;
        notation_text_position = 55;
        VerticalLine(255, 45, 174, 257 * sc_width);
    }

    UseSystemFont();
    
    if(player == 0)
        PutString("White", notation_text_position, 215 * sc_width);
    else
        PutString("Black", notation_text_position, 215 * sc_width);
        
    PutString(current_move, notation_text_position, 275 * sc_width);
    notation_text_position += 11;
    notation_row_count++;

    
}

void UpdateStatus(char *message)
{
    UseSystemFont();
    PutString("                    ", 188, 215 * sc_width);
    PutString(message, 188, 215 * sc_width);
}

unsigned char isKingInCheck(unsigned char kingRow, unsigned char kingCol)
{
    unsigned char i;
    unsigned char j;
    int pawnDirections[2][2] = {{1, 1}, {1, -1}};
    int dir;
    int dr;
    int dc;
    int newRow;
    int newCol;
    int piece;
    int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    int diagonalDirections[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    int knightMoves[8][2] = {
        {-2, -1}, {-2, 1},
        {-1, -2}, {-1, 2},
        {1, -2}, {1, 2},
        {2, -1}, {2, 1}
    };

    // find the white king (if a position hasn't been supplied)
    if(kingRow == 255 && kingCol == 255)
    {
        for(i=0;i<8;i++)
            for(j=0;j<8;j++)
            {
                if (gboard[i][j][0] == WHT_KING)
                {
                    kingRow = i;
                    kingCol = j;
                }
            }
    }

    // Check for enemy queen, rook, or bishop attacking horizontally, vertically, or diagonally
    for (dir = 0; dir < 4; dir++) {
        dr = directions[dir][0];
        dc = directions[dir][1];
        for (i = 1; i < 8; i++) {
            newRow = kingRow + i * dr;
            newCol = kingCol + i * dc;
            if (newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8) {
                break; // Out of the board
            }
            piece = gboard[newRow][newCol][0];
            if (piece != EMPTY) {
                if (piece == BLK_QUEEN || piece == BLK_ROOK) {
                    return 1; // King is under threat
                } else {
                    break; // Blocking piece
                }
            }
        }
    }

    // Check for enemy queen or bishop attacking diagonally
    for (dir = 0; dir < 4; dir++) {
        dr = diagonalDirections[dir][0];
        dc = diagonalDirections[dir][1];
        for (i = 1; i < 8; i++) {
            newRow = kingRow + i * dr;
            newCol = kingCol + i * dc;
            if (newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8) {
                break; // Out of the board
            }
            piece = gboard[newRow][newCol][0];
            if (piece != EMPTY) {
                if (piece == BLK_QUEEN || piece == BLK_BISHOP) {
                    return 1; // King is under threat
                } else {
                    break; // Blocking piece
                }
            }
        }
    }


    for (i = 0; i < 8; i++) {
        newRow = kingRow + knightMoves[i][0];
        newCol = kingCol + knightMoves[i][1];
        if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
            piece = gboard[newRow][newCol][0];
            if (piece == BLK_KNIGHT) {
                return 1; // King is under threat
            }
        }
    }

    // check if pawn or king are above, diagonal left to this spot
    if (kingRow > 0 && kingCol > 0)
        if (gboard[kingRow-1][kingCol-1][0] == BLK_PAWN || gboard[kingRow-1][kingCol-1][0] == BLK_KING)
            return 1;

    // check if pawn or king are above, diagonal right to this spot
    if (kingRow > 0 && kingCol < 7)
        if (gboard[kingRow-1][kingCol+1][0] == BLK_PAWN || gboard[kingRow-1][kingCol+1][0] == BLK_KING)
            return 1;

    // check if king is below, diagonal left to this spot
    if (kingRow < 7 && kingCol > 0)
        if (gboard[kingRow+1][kingCol-1][0] == BLK_KING)
            return 1;

    // check if king is below, diagonal right to this spot
    if (kingRow < 7 && kingCol < 7)
        if (gboard[kingRow+1][kingCol+1][0] == BLK_KING)
            return 1;

    // check if king is below this spot
    if (kingRow < 7)
        if (gboard[kingRow+1][kingCol][0] == BLK_KING)
            return 1;

    // check if king is above this spot
    if (kingRow > 0)
        if (gboard[kingRow-1][kingCol][0] == BLK_KING)
            return 1;

    // check if king is left this spot
    if (kingCol > 0)
        if (gboard[kingRow][kingCol-1][0] == BLK_KING)
            return 1;

    // check if king is right this spot
    if (kingCol < 7)
        if (gboard[kingRow][kingCol+1][0] == BLK_KING)
            return 1;

    return 0; // King is not in check
}

unsigned char isPieceBetweenUD(unsigned char src_row, unsigned char dest_row, unsigned char col)
{
    unsigned char r;

    if(src_row < dest_row)
    {
        // scan squares below piece
        for(r=src_row+1;r<dest_row;r++)
            if(gboard[r][col][0] != EMPTY)
                return 1;
    }
    else
    {
        // scan squares above piece
        for(r=src_row-1;r>dest_row;r--)
            if(gboard[r][col][0] != EMPTY)
                return 1;
    }

    return 0;
}

unsigned char isPieceBetweenLR(unsigned char src_col, unsigned char dest_col, unsigned char row)
{
    unsigned char c;

    if(src_col < dest_col)
    {
        // scan squares to the right of piece
        for(c=src_col+1;c<dest_col;c++)
            if(gboard[row][c][0] != EMPTY)
                return 1;
    }
    else
    {
        // scan squares to the left of piece
        for(c=src_col-1;c>dest_col;c--)
            if(gboard[row][c][0] != EMPTY)
                return 1;
    }

    return 0;
}

unsigned char isDiagonalMovementBlocked(int srcX, int srcY, int destX, int destY) {

    int x, y, dx, dy;

    // Determine the direction of movement
    dx = (srcX < destX) ? 1 : -1;
    dy = (srcY < destY) ? 1 : -1;

    // Check for blocking pieces in the path
    x = srcX + dx;
    y = srcY + dy;
    while (x != destX && y != destY) {
        if (gboard[x][y][0] != EMPTY) {
            return 1; // There is a blocking piece
        }
        x += dx;
        y += dy;
    }

    return 0; // No blocking piece found
}

unsigned char isMoveIsValid(char src_row, char src_col, char dest_row, char dest_col)
{
    unsigned char invalidmove = 0;
    unsigned char moving_piece = gboard[src_row][src_col][0];
    unsigned char dest_square = gboard[dest_row][dest_col][0];
    unsigned char temp_val;

    // does dest square have a piece of same color?
    if(dest_square >= WHT_KING && dest_square <= WHT_PAWN)
        invalidmove++;
    else
    {
        switch (moving_piece)
        {
            case WHT_KING:
                if ( ((src_row > dest_row) ? (src_row - dest_row) : (dest_row - src_row)) > 1)
                    invalidmove++;
                if ( ((src_col > dest_col) ? (src_col - dest_col) : (dest_col - src_col)) > 1)
                    invalidmove++;
                if (isKingInCheck(dest_row, dest_col) == 1)
                    invalidmove++;
                break;
            case WHT_QUEEN:
                if (( (src_row == dest_row || src_col == dest_col || abs(src_row - dest_row) == abs(src_col - dest_col))? 1 : 0 ) == 0)
                    invalidmove++;
                if (src_col == dest_col)
                        invalidmove += isPieceBetweenUD(src_row, dest_row, src_col);
                if (src_row == dest_row)
                        invalidmove += isPieceBetweenLR(src_col, dest_col, src_row);
                if (src_col != dest_col && src_row != dest_row)
                        invalidmove += isDiagonalMovementBlocked(src_row, src_col, dest_row, dest_col);
                break;
            case WHT_BISHOP:
                if (( (abs(src_row - dest_row) == abs(src_col - dest_col)) ? 1 : 0 ) == 0)
                    invalidmove++;
                invalidmove += isDiagonalMovementBlocked(src_row, src_col, dest_row, dest_col);
                break;
            case WHT_KNIGHT:
                if (( ((abs(src_row-dest_row) == 2 && abs(src_col-dest_col) == 1) || (abs(src_row-dest_row) == 1 && abs(src_col-dest_col) == 2)) ? 1 : 0) == 0)
                    invalidmove++;
                break;
            case WHT_ROOK:
                if (( (src_row == dest_row || src_col == dest_col) ? 1 : 0 ) == 0)
                    invalidmove++;
                else
                    if(src_col == dest_col)
                        invalidmove += isPieceBetweenUD(src_row, dest_row, src_col);
                    else
                        invalidmove += isPieceBetweenLR(src_col, dest_col, src_row);
                break;
            case WHT_PAWN:
                if (src_row == 6 && src_row-dest_row == 2) 
                {
                    // first move, 2 squares
                    if (src_col != dest_col)
                        invalidmove++;
                } 
                else 
                {
                    // pawn can only move one square
                    if (src_row-dest_row == 1)
                    {
                        if(abs(src_col - dest_col) == 1)
                        {
                            // check if enemy piece is in diagonal square 
                            // (we already checked if square has a white piece, so only empty square is left)
                            if (dest_square == EMPTY)
                            {
                                invalidmove++;
                            }
                                
                        }
                        else 
                        {
                            // ordinary single move ahead
                            if (!(src_col == dest_col && dest_square == EMPTY))
                                invalidmove++;
                        }
                    }
                    else
                        invalidmove++;
                }
                break;
            
        }
    }

    if(invalidmove != 0)
    {
        // temporarily move the piece and then check if the move
        // puts the king in check
        temp_val = gboard[dest_row][dest_col][0];
        gboard[dest_row][dest_col][0] = gboard[src_row][src_col][0];
        gboard[src_row][src_col][0] = EMPTY;
        
        if (isKingInCheck(255, 255) == 1)
            invalidmove++;

        // put the piece back
        gboard[src_row][src_col][0] = gboard[dest_row][dest_col][0];
        gboard[dest_row][dest_col][0] = temp_val;
        
    }

    return invalidmove;
}

void MovePiece(unsigned char from_row, unsigned char from_col, unsigned char to_row, unsigned char to_col) 
{
    unsigned char pattern;
    unsigned char piece;

    // calls TempHideMouse for C128
    if (ISGEOS128)
    {
        if(C128_80_COL_MODE) {
            TEMP_HIDE_MOUSE
        }
    }

    // clear source square
    pattern = gboard[from_row][from_col][1];
    DrawStdRect(pattern, &vboard[from_row][from_col]);

    // clear dest square
    pattern = gboard[to_row][to_col][1];
    DrawStdRect(pattern, &vboard[to_row][to_col]);

    // move the piece to destination
    gboard[to_row][to_col][0] = gboard[from_row][from_col][0];
    gboard[from_row][from_col][0] = EMPTY;

    // switch character sets
    LoadCharSet ((struct fontdesc *)(fontbuffer));

    // draw proper piece at the new square
    piece = GetPieceChar(to_row, to_col);
    PutChar(piece, 50+(18*to_row), (27 * sc_width) + ((20*to_col) * sc_width) ) ;

}

void MouseClickHandler() 
{
    struct window *rect;
    unsigned char hittest;
    unsigned char r,c,z,m;
    struct pixel  location;
    unsigned short loop;
    unsigned char invalidmove;

    DrawSprite(2,square_cursor);
    DrawSprite(3,badmove_cursor);

    if (gameState == INPROGRESS)
    {
        for(r=0;r<8;r++)
        {
            for(c=0;c<8;c++)
            {
                rect = &vboard[r][c];
                hittest = IsMseInRegion(rect);

                // IsMseInRegion returns 255 if the mouseclick is in the given rectangle
                if(hittest == 255)
                {
                    // this is silly but seems necessary
                    if(tctr == 1 || tctr == 2) {
                        tctr++;
                        if(tctr==3)
                            tctr=0;
                        old_otherPressVec();
                        return;
                    }

                    if(tctr == 0)
                    {
                        if(sel_row1 == 255)
                        {
                            // 1st (select) mouse click
                            if(gboard[r][c][0] >= WHT_KING && gboard[r][c][0] <= WHT_PAWN)
                            {
                                // if player has selected a white piece
                                // display the square selected sprite and await
                                // the destination square selection

                                location.y = vboard[r][c].top;
                                location.x = vboard[r][c].left;
                                
                                // calls TempHideMouse for C128
                                if (ISGEOS128)
                                {
                                    if(C128_80_COL_MODE) {
                                        TEMP_HIDE_MOUSE
                                    }
                                }

                                PosSprite(2, &location);
                                EnablSprite(2);

                                sel_row1 = r;
                                sel_col1 = c;
                                tctr=1;
                                
                                old_otherPressVec();
                                return;
                            }
                            else
                            {
                                // player selected a square that does not have
                                // a white piece.  hide select sprite and reset
                                // click
                                DisablSprite(2);
                                sel_row1 = 255;
                                sel_col1 = 255;
                                
                                old_otherPressVec();
                                return;
                            }

                            
                        }
                        else
                        {
                            // this click is for the selected destination square
                            // first, check if the move is valid for the selected piece.
                            invalidmove = isMoveIsValid(sel_row1, sel_col1, r,c);

                            if(invalidmove != 0)
                            {
                                // Move is not valid.  Display error sprite
                                location.y = vboard[r][c].top;
                                location.x = vboard[r][c].left;

                                PosSprite(3, &location);
                                
                                // flash the error sprite
                                for(loop=0;loop<1500;loop++)
                                {
                                    EnablSprite(3);
                                    DisablSprite(3);
                                }
                                DisablSprite(2);

                                // reset click variables
                                sel_row1 = 255;
                                sel_col1 = 255;
                                old_otherPressVec();
                                return;
                            }
                            else 
                            {
                                // move is valid, update the notation log,
                                // move the piece, remove the selection sprite,
                                // execute the move with the engine, and await the AI's turn

                                UpdateNotation(0, sel_row1, sel_col1, r, c);
                                MovePiece(sel_row1, sel_col1, r, c);
                                DisablSprite(2);
                                
                                // Here we inform the chess engine the player move
                                memset(&user_move[0], 0, sizeof(user_move));
                                strcpy(user_move, gbnotation[sel_row1][sel_col1]);
                                strcat(user_move, gbnotation[r][c]);

                                playerMove();

                                // we have notified the engine of our move.
                                // now await AI move

                                UpdateStatus("Black is thinking...");

                                memset(&ai_move[0], 0, sizeof(ai_move));
                                gameState = aiMove();

                                if (gameState == STOPPED)
                                {
                                    // a checkmate has occurred
                                    UpdateStatus("Checkmate!");

                                    sel_row1 = 255;
                                    sel_col1 = 255;

                                    old_otherPressVec();
                                    return;
                                }
                                else 
                                {
                                    UpdateStatus("Your move.");

                                    // ai_move[] string now contains the ai move
                                    // translate the string to piece movement
                                    c = xlateCol(ai_move[0]);
                                    r = xlateRow(ai_move[1]);
                                    m = xlateCol(ai_move[2]);
                                    z = xlateRow(ai_move[3]);
                                    UpdateNotation(1, r, c, z, m);
                                    MovePiece(r,c,z,m);

                                    // let player know if king is in check
                                    if (isKingInCheck(255,255) == 1)
                                        UpdateStatus("**Check!**");

                                    sel_row1 = 255;
                                    sel_col1 = 255;

                                    old_otherPressVec();
                                    return;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    old_otherPressVec();
}

void NewGameMenuHandler(void) {

    RecoverAllMenus();
    NewGame();
}

void Switch4080MenuHandler(void)
{
    if (ISGEOS128)
	{
		if(C128_40_COL_MODE)
        {
            sc_width = 2;
            ((struct menu *)&mainMenu)->size.right = 35;
            ((struct menu *)&mainMenu)->items[0].rest = (void *)&subMenu128_80;
        }
            
        else
        {
            sc_width = 1;
            ((struct menu *)&mainMenu)->size.right = 27;
            ((struct menu *)&mainMenu)->items[0].rest = (void *)&subMenu128_40;
        }
            
            
        SetNewMode();
        LoadFont();
        InitScreen();
        InitBoard(1);
        InitMovePanel();
    }
    else
        RecoverAllMenus();

	UpdateStatus("Your move.");
    DoMenu((struct menu *)&mainMenu);
}

void hook_into_system(void) {
        old_otherPressVec = otherPressVec;
        otherPressVec = MouseClickHandler;
}

void remove_hook(void) {
        otherPressVec = old_otherPressVec;

        free((struct menu *)&mainMenu);
}



