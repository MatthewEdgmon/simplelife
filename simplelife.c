/**
 * main.c - simplelife
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

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "simplelife.h"

game_board_t* NewGameBoard(int size_x, int size_y) {

    game_board_t* board = (game_board_t*) malloc(sizeof(game_board_t));

    board->size_x = size_x;
    board->size_y = size_y;
    board->cells = malloc(board->size_x * sizeof(*board->cells));

    // Allocate a sub-pointer inside the double pointer.
    for(int i = 0; i < board->size_x; i++) {
        board->cells[i] = malloc(board->size_y * sizeof(board->cells[0]));
    }

    // Mark all cells as dead to begin with.
    for(int i = 0; i < board->size_x; i++) {
        for(int j = 0; j < board->size_y; j++) {
            board->cells[i][j].alive = false;
            board->cells[i][j].next_state = false;
        }
    }

    return board;
}

void CleanupGameBoard(game_board_t* board) {

    // Check if board exists before deallocating.
    if(board == NULL) {
        return;
    }

    // Free each sub-pointer inside the double pointer.
    for(int i = 0; i < board->size_x; i++) {
        free(board->cells[i]);
    }

    free(board->cells);
    free(board);
}

void StepGameBoard(game_board_t* board) {

    // Iterate through each cell and apply the rules to it, and save
    // the result into next_state.
    for(int i = 0; i < board->size_x; i++) {
        for(int j = 0; j < board->size_y; j++) {
            // By default, all cells die.
            board->cells[i][j].next_state = false;
            // Any living cell with 2 or 3 live neighbors survives.
            if(board->cells[i][j].alive == true) {
                if(CountLivingNeighbours(board, i, j) == 2 || CountLivingNeighbours(board, i, j) == 3) {
                    board->cells[i][j].next_state = true;
                }
            }
            // Any dead cell with 3 live neighbors lives.
            if(board->cells[i][j].alive == false) {
                if(CountLivingNeighbours(board, i, j) == 3) {
                    board->cells[i][j].next_state = true;
                }
            }
        }
    }

    // Finally, set every cell's alive to it's value of next_state
    // to simulate death, survival, and resurrection simultaionusly.
    for(int i = 0; i < board->size_x; i++) {
        for(int j = 0; j < board->size_y; j++) {
            board->cells[i][j].alive = board->cells[i][j].next_state;
        }
    }
}

int CountDeadNeighbours(game_board_t* board, int cell_x, int cell_y) {
    int count = 0;

    if(cell_x <= 0 || cell_x >= board->size_x) {
        return 0;
    }

    if(cell_y <= 0 || cell_y >= board->size_y) {
        return 0;
    }

    if(board->cells[cell_x - 1][cell_y - 1].alive == false) { count++; } // Top Left
    if(board->cells[cell_x    ][cell_y - 1].alive == false) { count++; } // Top Center
    if(board->cells[cell_x + 1][cell_y - 1].alive == false) { count++; } // Top Right
    if(board->cells[cell_x - 1][cell_y    ].alive == false) { count++; } // Left
    if(board->cells[cell_x + 1][cell_y    ].alive == false) { count++; } // Right
    if(board->cells[cell_x - 1][cell_y + 1].alive == false) { count++; } // Bottom Left
    if(board->cells[cell_x    ][cell_y + 1].alive == false) { count++; } // Bottom Center
    if(board->cells[cell_x + 1][cell_y + 1].alive == false) { count++; } // Bottom Right

    return count;
}

int CountLivingNeighbours(game_board_t* board, int cell_x, int cell_y) {
    int count = 0;

    if(cell_x <= 0 || cell_x >= (board->size_x - 1)) {
        return 0;
    }

    if(cell_y <= 0 || cell_y >= (board->size_y - 1)) {
        return 0;
    }

    if(board->cells[cell_x - 1][cell_y - 1].alive == true) { count++; } // Top Left
    if(board->cells[cell_x    ][cell_y - 1].alive == true) { count++; } // Top Center
    if(board->cells[cell_x + 1][cell_y - 1].alive == true) { count++; } // Top Right
    if(board->cells[cell_x - 1][cell_y    ].alive == true) { count++; } // Left
    if(board->cells[cell_x + 1][cell_y    ].alive == true) { count++; } // Right
    if(board->cells[cell_x - 1][cell_y + 1].alive == true) { count++; } // Bottom Left
    if(board->cells[cell_x    ][cell_y + 1].alive == true) { count++; } // Bottom Center
    if(board->cells[cell_x + 1][cell_y + 1].alive == true) { count++; } // Bottom Right

    return count;
}

void WritePPM(game_board_t* board, const char* file_name) {

    FILE* image_file;

    image_file = fopen(file_name, "wb");

    if(image_file == NULL) {
        printf("Failed to open image file \"%s\".\n", file_name);
        return;
    }

    fprintf(image_file, "P6\n");
    fprintf(image_file, "# File generated by simplelife.\n");
    fprintf(image_file, "%d %d\n", board->size_x, board->size_y);
    fprintf(image_file, "255\n");

    for(int i = 0; i < board->size_x; i++) {
        for(int j = 0; j < board->size_y; j++) {
            if(board->cells[i][j].alive == true) {
                // Black for alive.
                fputc(0, image_file); // R
                fputc(0, image_file); // G
                fputc(0, image_file); // B
            } else {
                // White for dead.
                fputc(255, image_file); // R
                fputc(255, image_file); // G
                fputc(255, image_file); // B
            }
        }
    }

    fclose(image_file);
}

int main(int argc, char** argv) {

    // Seed RNG with time.
    time_t t;
    srand(time(&t));

    game_board_t* board = NewGameBoard(256, 256);
    char file_name[255];

    for(int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }

    // Fill the gameboard with random data.
    for(int i = 0; i < board->size_x; i++) {
        for(int j = 0; j < board->size_y; j++) {
            board->cells[i][j].alive = rand() % 2;
        }
    }

    WritePPM(board, "initial.ppm");

    StepGameBoard(board);

    for(int i = 0; i < 255; i++) {
        StepGameBoard(board);
        snprintf(file_name, 255, "step%d.ppm", i);
        WritePPM(board, file_name);
    }

    WritePPM(board, "final.ppm");

    CleanupGameBoard(board);

    return 0;
}