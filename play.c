#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h> 
#include "board.h"
#include "pos.h"
#include "logic.h"

int main(int argc, char *argv[]){

    unsigned int h, w, s, l;
    if(argv[2] == NULL || argv[4] == NULL || argv[6] == NULL || 
       argv[8] == NULL || argv[2] < 0 || argv[4] < 0 ||
       argv[6] < 0 || argv[8] < 0){
           fprintf(stderr, "Wrong or No Imput");
           exit(1);
    }

    for(unsigned int i = 1; i < argc; i += 2){
        if( strcmp(argv[i], "-h") == 0){
            h = atoi(argv[i+1]);
        }
        else if( strcmp(argv[i], "-w") == 0){                                        
            w = atof(argv[i+1]);                                                      
        } 
        else if( strcmp(argv[i], "-s") == 0){                                        
            s = atof(argv[i+1]);                                                      
        } 
        else{                                        
            l = atof(argv[i+1]);                                                      
        } 
    }

    game* g = new_game(s, l, w, h, MATRIX);  
    board_show(g->b);

    unsigned int lb1 = 48, ub1 = 57, lb2 = 65, ub2 = 90, lb3 = 97, ub3 = 122,
                 cnv_cap = 55, cnv_lowc = 60;

    while(game_outcome(g) == IN_PROGRESS){

        if(g->player == BLACKS_TURN){
            printf("Blacks turn: ");
        }
        else{
            printf("Whites turn: ");
        }
        printf("Please enter a character: ");
        char ch = 0;
        scanf("%c%*c", &ch);
        int col; 
        if(ch == '!'){
            magnetize(g);
            board_show(g->b);
            continue;
        }
        else if(ch >= lb1 && ch <= ub1){
            col = ch - lb1;
        }
        else if(ch >= lb2 && ch <= ub2){                                             
            col = ch - cnv_cap;                                                         
        }
        else if(ch >= lb3 && ch <= ub3){                                             
            col = ch - cnv_lowc;                                                         
        }
        else{
            printf("Invalid Inputttt\n");
            continue;
        }
        if(drop_piece(g, col)){
        }
        else {
            printf("Input not possible\n");
            continue;
        }
        board_show(g->b);
        continue;

    }
    if(game_outcome(g) == 1){
        printf("Outcome: Black won!\n");
    }
    else if(game_outcome(g) == 2){
        printf("Outcome: White won!\n");
    }
    else printf("Outcome: Draw\n");

    return 0;

}
