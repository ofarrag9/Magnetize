#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h> 
#include <stdbool.h>
#include "logic.h"



game* new_game(unsigned int square, unsigned int maglock, unsigned int width,
               unsigned int height, enum type type){

    if(square > width || square > height || square == 0){
        fprintf(stderr, "Impossible to win");
        exit(1);
    }
    game* g = (game*)malloc(sizeof(game));
    g->square = square;
    g->maglock = maglock;
    g->b = board_new(width, height, type);
    g->black_rem = 0;
    g->white_rem = 0;
    g->player = BLACKS_TURN;
    return g;

}



void game_free(game* g){
    board_free(g->b);
    free(g);
}



void drop_helper(game* g, pos p){

    unsigned int row = p.r;
    for(unsigned int i = row; i  < g->b->height; i++){
        p.r = i;
        if((board_get(g->b, p) == EMPTY) && (i == g->b->height - 1)){
            row = i;
            break;
        }
        else if(board_get(g->b, p) == EMPTY){
            continue;
        }
        else{
            row = i - 1;
            break;
        } 
         
    }
    p.r = row;
    if(g->player == BLACKS_TURN){
        board_set(g->b, p, BLACK);
    }
    else{
        board_set(g->b, p, WHITE);
    }

}



void drop_mag_helper(game* g, pos p){
    pos p_next_r = make_pos(p.r + 1, p.c);
    if((board_get(g->b, p) == EMPTY) || (board_get(g->b, p_next_r) != EMPTY)){
        return;
    }
    pos p_orig = make_pos(p.r, p.c);
    unsigned int row = p.r + 1;
    for(unsigned int i = row; i < g->b->height; i++){
        p.r = i;
        if((board_get(g->b, p) == EMPTY) && (i == g->b->height - 1)){
            row = i;
            break;
        }
        else if(board_get(g->b, p) == EMPTY){
            continue;
        }
        else{
            row = i - 1;
            break;
        }
    }
    p.r = row;
    if(g->player == BLACKS_TURN){
        board_set(g->b, p_orig, EMPTY);
        board_set(g->b, p, BLACK);
    }
    else{
        board_set(g->b, p_orig, EMPTY); 
        board_set(g->b, p, WHITE);
    } 
}



void magnet_helper(game* g, pos p){

    if(g->player == WHITES_TURN){

        unsigned int r_orig = p.r;
        unsigned int c_orig = p.c;
        unsigned int col = p.c + 1;
        for(unsigned int i = col; i < g->b->width; i++){
            p.c = i;
            if((board_get(g->b, p) == EMPTY) && (i == g->b->width - 1)){
                col = i;
                break;
            }
            else if(board_get(g->b, p) == EMPTY){
                continue;
            }
            else{
                col = i - 1;
                break;
            }

        }
        p.c = col;
        pos p_orig = make_pos(r_orig, c_orig);
        board_set(g->b, p_orig, EMPTY);
        board_set(g->b, p, WHITE);
    }

    
    else{
        unsigned int r_orig = p.r;
        unsigned int c_orig = p.c;
        unsigned int col = p.c - 1;
        for(int i = col; i > -1; i--){
            p.c = i;
            if((board_get(g->b, p) == EMPTY) && (i == 0)){
                col = i;
                break;
            }
            else if(board_get(g->b, p) == EMPTY){
                continue;
            }
            else{
                col = i + 1;
                break;
            }

        }
        p.c = col;
        pos p_orig = make_pos(r_orig, c_orig);
        board_set(g->b, p_orig, EMPTY);
        board_set(g->b, p, BLACK);
    }

}



void drop_after_magnet(game* g, int drop_all){                                  
    pos p = make_pos(0,0);                                                      
    for(int row = g->b->height - 2; row > -1; row--){                           
        for(int col = g->b->width - 1; col > -1; col--){                        
            p.r = row;                                                          
            p.c = col;                                                          
            if(drop_all){                                                       
                if(board_get(g->b, p) == BLACK){                                
                    g->player = BLACKS_TURN;                                    
                    drop_mag_helper(g, p);                                      
                }                                                               
                else if(board_get(g->b, p) == WHITE){                           
                    g->player = WHITES_TURN;                                    
                    drop_mag_helper(g, p);                                      
                }                                                               
                else continue;                                                  
            }                                                                   
            else{                                                               
                if(g->player == BLACKS_TURN){                                   
                    if(board_get(g->b, p) == BLACK){                            
                        drop_mag_helper(g, p);                                  
                    }                                                           
                    else continue;                                              
                }                                                               
                else{                                                           
                    if(board_get(g->b, p) == WHITE){                            
                        drop_mag_helper(g, p);                                  
                    }                                                           
                    else continue;                                              
                }                                                               
            }                                                                   
        }                                                                       
    }                                                                           
}



bool outcome_helper(game* g, pos p){                                            
    if(board_get(g->b, p) == EMPTY){                                            
        return false;                                                           
    }                                                                           
    pos p_orig = make_pos(p.r, p.c);                                            
    for(unsigned int r = 0; r < g->square; r++){                                
        for(unsigned int c = 0; c < g->square; c++){                            
            if(p.c > g->b->width - 1 || p.r > g->b->height - 1){                
                return false;                                                   
            }                                                                   
            if(board_get(g->b, p_orig) == BLACK){                               
                if(board_get(g->b, p) == BLACK){                                
                    p.c +=  1;                                                  
                    continue;                                                   
                }                                                               
                else return false;                                              
            }                                                                   
            else if(board_get(g->b, p_orig) == WHITE){                          
                if(board_get(g->b, p) == WHITE){                                
                    p.c += 1;                                                   
                    continue;                                                   
                }                                                               
                else return false;                                              
            }                                                                   
            else return false;                                                  
            p.c +=  1;                                                          
        }                                                                       
        p.c = p_orig.c;                                                         
        p.r += 1;                                                               
    }                                                                           
    return true;                                                                
} 



bool draw_helper(game* g){                                                      
    pos p = make_pos(0,0);                                                      
    for(unsigned int r = 0; r < g->b->height; r++){                             
        p.r = r;                                                                
        for(unsigned int c = 0; c < g->b->width; c++){                          
            p.c = c;                                                            
            if(board_get(g->b, p) == EMPTY){                                    
                return false;                                                   
            }                                                                   
            else continue;                                                      
        }                                                                       
    }                                                                           
    return true;                                                                
}                                                                               
                                                                                
                                                                                
                                                                                
outcome game_outcome(game* g){                                                  
                                                                                
    pos p = make_pos(0, 0);                                                        
    int w_win = 0;                                                                 
    int b_win = 0;                                                                 
    for(unsigned int r = 0; r < g->b->height; r++){                                
        for(unsigned int c = 0; c < g->b->width; c++){                             
            p.r = r;                                                               
            p.c = c;                                                               
            if(outcome_helper(g, p)){                                              
                if(board_get(g->b, p) == BLACK){                                   
                    b_win = 1;                                                     
                }                                                                  
                else{                                                              
                    w_win = 1;                                                     
                }                                                                  
            }                                                                      
            else continue;                                                         
        }                                                                          
    }                                                                              
                                                                                   
    if(w_win == 0 && b_win == 1){                                                  
        return BLACK_WIN;                                                       
    }                                                                           
    else if(w_win == 1 && b_win == 0){                                          
        return WHITE_WIN;                                                       
    }                                                                           
    else if(w_win == 1 && b_win == 1){                                          
        return DRAW;                                                            
    }                                                                           
    else if(draw_helper(g)){                                                    
        return DRAW;                                                            
    }                                                                           
    else return IN_PROGRESS;                                                    
                                                                                
} 



bool magnetize(game* g){

    pos p = make_pos(0,0);
    if(g->player == WHITES_TURN){
        if(g->white_rem == 0){
            for(int row = g->b->height - 1; row > -1; row--){
                for(int col1 = g->b->width - 2; col1 > -1; col1--){
                    p.r = row; 
                    p.c = col1;
                    if(board_get(g->b, p) == WHITE){ 
                        magnet_helper(g, p);
                    }
                    else continue;
                }
                g->player = BLACKS_TURN;
                for(int col2 = g->b->width - 2; col2 > -1; col2--){           
                    if(g->black_rem > 1){
                        break;
                    }
                    if(row == 0){
                        break;
                    }
                    p.r = row - 1;
                    p.c = col2;
                    if(board_get(g->b, p) == BLACK){                                                   
                        drop_mag_helper(g, p); 
                    } 
                    else continue;                                             
                }
                g->player = WHITES_TURN;
            }
            if(g->black_rem > 1){
                g->black_rem -= 1; 
                if(g->black_rem == 1){
                    g->player = BLACKS_TURN;                                                                                  
                    if(g->white_rem > 1){                                       
                        magnetize(g);                                           
                        int white_rem_true = g->white_rem;                      
                        g->white_rem = 0;                                       
                        g->player = WHITES_TURN;                                
                        magnetize(g);                                           
                        g->player = BLACKS_TURN;                                                                    
                        g->white_rem = white_rem_true - 1;                      
                        g->black_rem = 0;                                       
                        return true;                                            
                    }                                                           
                    else{                                                       
                        magnetize(g);                                           
                        g->black_rem = 0;                                                                           
                        g->player = BLACKS_TURN;                                
                        return true;                                            
                    }                                                           
                }   
                g->player = WHITES_TURN;            
            }
            else {
                g->player = BLACKS_TURN;
            }
            g->white_rem = g->maglock + 1;
            return true;
        }
        else if(g->white_rem == 1){                                              
                if(g->black_rem > 1){                                           
                    drop_after_magnet(g, 0);                                                                              
                    g->player = BLACKS_TURN;           
                }                                                               
                else{                                                          
                    drop_after_magnet(g, 1);                                                                             
                    g->player = BLACKS_TURN;            
                } 
                return false;                                                              
            } 
        else{
            g->white_rem -= 1;           
            return false;
        }
    }

    else{
        if(g->black_rem == 0){
            for(int row = g->b->height - 1; row > -1; row--){              
                for(unsigned int col1 = 1; col1 < g->b->width; col1++){        
                    p.r = row;
                    p.c = col1; 
                    if(board_get(g->b, p) == BLACK){                                              
                        magnet_helper(g, p);   
                    }
                    else continue;                                         
                }                                                                   
                g->player = WHITES_TURN;                                            
                for(unsigned int col2 = 1; col2 < g->b->width; col2++){        
                    if(row == 0){                                                   
                        break;                                                      
                    }                                                             
                    p.r = row - 1;
                    p.c = col2;
                    if(board_get(g->b, p) == WHITE){                                          
                        drop_mag_helper(g, p);
                    }   
                    else continue;                                              
                }                                                                 
                g->player = BLACKS_TURN;                                             
            }
            if(g->white_rem > 1){                                                  
                g->white_rem -= 1;                                                 
                if(g->white_rem == 1){                                                                                        
                    g->player = WHITES_TURN;                                        
                    if(g->black_rem > 1){                                          
                        magnetize(g);                                              
                        int black_rem_true = g->black_rem;                         
                        g->black_rem = 0;                                          
                        g->player = BLACKS_TURN;                                   
                        magnetize(g);                                              
                        g->player = WHITES_TURN;                                    
                        g->black_rem = black_rem_true - 1;                         
                        g->white_rem = 0;                                          
                        return true;                                               
                    }                                                              
                    else{                                                          
                        magnetize(g);                                              
                        g->white_rem = 0;                                           
                        g->player = WHITES_TURN;                                   
                        return true;                                               
                    }                                                              
                }                                                                  
                g->player = BLACKS_TURN;                                           
            }                                                                     
            else {                                                                 
                g->player = WHITES_TURN;                                           
            }                                                                      
            g->black_rem = g->maglock + 1;                                             
            return true;          
        }
        else if(g->black_rem == 1){                                                  
            if(g->white_rem > 1){                                              
                drop_after_magnet(g, 0); 
                g->player = WHITES_TURN;                    
            }                                                                  
            else{                                                             
                drop_after_magnet(g, 1);                                                                                    
                g->player = WHITES_TURN;         
            }
            return false;                                                                  
        }
        else{
            g->black_rem -= 1;
            return false;
        }
    }

}



bool drop_piece(game* g, unsigned int column){                                  
                                                                                
    unsigned int row = 0;                                                       
    pos p = make_pos(row, column);                                              
                                                                                
    if((column < 0) || (column > g->b->width - 1) || 
       (board_get(g->b, p) != EMPTY)){                                            
        return false;                                                           
    }   

    if(g->black_rem > 1 && g->white_rem > 1){
        g->black_rem--;
        g->white_rem--;
        return true;
    } 
    
    if(g->black_rem == 1){
        g->player = BLACKS_TURN;
    }                           
    if(g->white_rem == 1){
        g->player = WHITES_TURN;
    }                                            
                                                                                                                                                                                                      
    if(g->player == BLACKS_TURN){                                           
        if(g->black_rem > 1){                                                  
            g->player = WHITES_TURN;                                                   
            return true;                                                    
        }                                                                   
        else if(g->black_rem == 1){                                                                                      
            if(g->white_rem > 1){
                magnetize(g);
                int white_rem_true = g->white_rem;                                 
                g->white_rem = 0;  
                g->player = WHITES_TURN;                                             
                magnetize(g);  
                g->player = WHITES_TURN;    
                p.r = 1;                                                            
                drop_helper(g, p);                                                  
                g->player = BLACKS_TURN;                                                              
                g->white_rem = white_rem_true - 1;   
                g->black_rem = 0;                              
                return true;
            }
            else{
                magnetize(g);
                g->black_rem = 0;
                g->player = WHITES_TURN;
                p.r = 1;                                                           
                drop_helper(g, p);                                              
                g->player = BLACKS_TURN;
                return true;  
            }                                                  
        }                                                                   
        else{  
            if(g->white_rem > 1){                                                  
                g->player = WHITES_TURN;                                           
                magnetize(g);                                                      
                if(g->white_rem == 1){                                             
                    p.r = 1;                                                       
                    g->player = BLACKS_TURN;                                        
                    drop_helper(g, p);                                              
                    g->player = WHITES_TURN;                                                                                    
                    if(g->black_rem > 1){                                           
                        magnetize(g);                                               
                        int black_rem_true = g->black_rem;                          
                        g->black_rem = 0;                                           
                        g->player = BLACKS_TURN;                                    
                        magnetize(g);                                               
                        g->player = WHITES_TURN;                                    
                        p.r = 1;                                                    
                        drop_helper(g, p);                                          
                        g->black_rem = black_rem_true - 1;                          
                        g->white_rem = 0;                                           
                        return true;                                                
                    }                                                               
                    else{                                                           
                        magnetize(g);                                               
                        g->white_rem = 0;                                           
                        g->player = BLACKS_TURN;                                                                    
                        return true;                                                
                    }                                                              
                }            
                else{
                    g->player = BLACKS_TURN;
                    drop_helper(g,p);
                }                                                      
                g->player = BLACKS_TURN;                                           
                return true;                                                       
            }                                                                      
            else{                                                                  
                p.r = 1;                                                                                                             
                drop_helper(g, p);                                                 
                g->player = WHITES_TURN;                                           
                return true;                                                       
            }                                                   
        }                                                       
    }                                                                       
    else{                                                                   
        if(g->white_rem > 1){                                                      
            g->player = BLACKS_TURN;                                                    
            return true;                                                       
        }                                                                      
        else if(g->white_rem == 1){                                                
            if(g->black_rem > 1){  
                magnetize(g);                                          
                int black_rem_true = g->black_rem;                                    
                g->black_rem = 0;                                                      
                g->player = BLACKS_TURN;                                            
                magnetize(g);                                                      
                g->player = BLACKS_TURN;  
                p.r = 1;                                                           
                drop_helper(g, p);                                              
                g->player = WHITES_TURN;                                          
                g->black_rem = black_rem_true - 1; 
                g->white_rem = 0;                                   
                return true; 
            }
            else{
                magnetize(g);
                g->white_rem = 0;
                p.r = 1;                                                           
                drop_helper(g, p);                                              
                g->player = WHITES_TURN;
                return true; 
            }                                                     
        }                                                                      
        else{    
            if(g->black_rem > 1){
                g->player = BLACKS_TURN;
                magnetize(g);
                if(g->black_rem == 1){
                    p.r = 1; 
                    g->player = WHITES_TURN;                                                          
                    drop_helper(g, p);                                                 
                    g->player = BLACKS_TURN;                                                            
                    if(g->white_rem > 1){                                                  
                        magnetize(g);                                                      
                        int white_rem_true = g->white_rem;                                 
                        g->white_rem = 0;                                                  
                        g->player = WHITES_TURN;                                            
                        magnetize(g);                                                      
                        g->player = BLACKS_TURN;                                           
                        p.r = 1;                                                            
                        drop_helper(g, p);                                                                                             
                        g->white_rem = white_rem_true - 1;                                 
                        g->black_rem = 0;                                                  
                        return true;                                                       
                    }                                                                      
                    else{                                                                  
                        magnetize(g);                                                    
                        g->black_rem = 0;                                                  
                        g->player = WHITES_TURN;                                                                                   
                        return true;                                                       
                    }
                }
                else{
                    g->player = WHITES_TURN;
                    drop_helper(g,p);
                }
                g->player = WHITES_TURN;
                return true;
            } 
            else{                                                             
                p.r = 1;                                                          
                drop_helper(g, p);                                                 
                g->player = BLACKS_TURN;                                           
                return true;   
            }                                                    
        }                                                      
    }                                                                       
                                                                                                                                                               
} 

