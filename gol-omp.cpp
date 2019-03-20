/*

    Name: Gregory Sullivan
    BlazerID: dgregs
    Course Section: CS 632
    Homework #: 3
    
*/

/*
    Compile with the following flags:
    -fopenmp -g -O3 -Wall
    
*/

#include <iostream>
#include <stdlib.h>
#include <omp.h>

using namespace std;

int size, iter, thds;
int buffer, size_with_buffer;
double t1, t2;

bool** get_array_2d (int rows, int cols) {
    
    bool* data = (bool*)malloc(rows*cols*sizeof(bool));
    
    bool** arr = (bool**)malloc(rows*sizeof(bool*));
    
    for (int i = 0; i < rows; i++) {
        arr[i] = &(data[cols*i]);
    }
    
    return arr;
        
}

void count_cells (bool** board, int g) {
    
    int alive = 0;
    
    for (int r = 1; r < buffer; r++) {
        for (int c = 1; c < buffer; c++) {
            if (board[r][c]) {
                alive++;
            }
        }
    }
    
    cout << "There are " << alive << " cells alive" << flush;
    cout << " at generation " << g+1 << ".\n" << endl;
    
}

void print_board (bool** board, int g) {
    
    cout << "Generation " << g+1 << ":\n" << endl;
    
    for (int r = 0; r < size_with_buffer; r++) {
        for (int c = 0; c < size_with_buffer; c++) {
            cout << board[r][c] << flush;
            if (c != buffer) {
                cout << " " << flush;
            }
        }
        cout << endl;
    }
    cout << endl;
    
}

int main (int argc, char **argv) {
    
    cout << endl;
    cout << "Conway's Game of Life\n" << endl;

    if (argc != 4) {
        cout << "Usage: " << argv[0] << flush;
        cout << " <size> <max. iterations> <threads>\n" << endl;
        exit(-1);
    }
    
    size = atoi(argv[1]);
    iter = atoi(argv[2]);
    thds = atoi(argv[3]);
    
    buffer = size+1;
    size_with_buffer = size+2;
    
    omp_set_num_threads(thds);
    
    cout << "Board size:        " << size << " x " << size << endl;
    cout << "Max. iterations:   " << iter << endl;
    cout << "Threads:           " << thds << "\n" << endl;
    
    bool** main_board = get_array_2d(size_with_buffer, size_with_buffer);
    bool** temp_board = get_array_2d(size_with_buffer, size_with_buffer);
    bool** ptr;
    
    /* Randomize the main board values */
    for (int r = 1; r < buffer; r++) {
        for (int c = 1; c < buffer; c++) {
            main_board[r][c] = rand() % 2;
        }
    }
    
    int current_generation = 0;
    
    count_cells(main_board, current_generation);

    t1 = omp_get_wtime();
        
    #pragma omp parallel
    {
        
        int thread_id = omp_get_thread_num() + 1;
        int thread_ct = omp_get_num_threads();
        
        for (int i = 0; i < iter; i++) {
            
            #pragma omp single
            {
                
                /* Set the buffer cells */
                main_board[0][0]                = main_board[size][size];   /* TOP LFT CORNER */
                main_board[0][buffer]           = main_board[size][1];      /* TOP RGT CORNER */
                main_board[buffer][0]           = main_board[1][size];      /* BTM LFT CORNER */
                main_board[buffer][buffer]      = main_board[1][1];         /* BTM RGT CORNER */
                
                for (int x = 1; x < buffer; x++) {
                    
                    main_board[0][x]            = main_board[size][x];      /* TOP ROW */
                    main_board[x][0]            = main_board[x][size];      /* LFT COL */
                    main_board[x][buffer]       = main_board[x][1];         /* RGT COL */
                    main_board[buffer][x]       = main_board[1][x];         /* BTM ROW */
                    
                }
                
                current_generation = i;
                
            }

            for (int r = thread_id; r < buffer; r+=thread_ct) {
                for (int c = 1; c < buffer; c++) {

                    int n = main_board[r-1][c-1] + main_board[r-1][c] + main_board[r-1][c+1] \
                        + main_board[r][c-1] + main_board[r][c+1] \
                        + main_board[r+1][c-1] + main_board[r+1][c] + main_board[r+1][c+1];

                    /* If the cell is alive */
                    if (main_board[r][c]) {
                        if (n < 2 || n > 3) {
                            temp_board[r][c] = 0;
                        } else {
                            temp_board[r][c] = 1;
                        }
                        
                    /* If the cell is dead */
                    } else {
                        if (n == 3) {
                            temp_board[r][c] = 1;
                        } else {
                            temp_board[r][c] = 0;
                        }
                    }

                }
            }
            #pragma omp barrier
            
            #pragma omp single
            {
                ptr = main_board;           /* Save the main board */
                main_board = temp_board;    /* Swap the temp board */
                temp_board = ptr;           /* Swap the main board */
            }
            
        } /* End of iteration loop */
        
    } /* End of OMP parallel region */

    t2 = omp_get_wtime();
    
    count_cells(main_board, current_generation);

    cout << "Completed in " << t2-t1 << " seconds.\n" << endl;

}
