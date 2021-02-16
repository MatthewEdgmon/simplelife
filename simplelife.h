/**
 * simplelife.h - Structures and function prototypes.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __SIMPLELIFE_H__
#define __SIMPLELIFE_H__

typedef struct cell {
    bool alive;
    bool next_state;
} cell_t;

typedef struct game_board {
    cell_t** cells;
    int size_x;
    int size_y;
} game_board_t;

game_board_t* NewGameBoard(int size_x, int size_y);
void CleanupGameBoard(game_board_t* board);
void StepGameBoard(game_board_t* board);

int CountDeadNeighbours(game_board_t* board, int cell_x, int cell_y);
int CountLivingNeighbours(game_board_t* board, int cell_x, int cell_y);

void WritePPM(game_board_t* board, const char* file_name);

#endif /* __SIMPLELIFE_H__ */