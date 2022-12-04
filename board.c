#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h> 
#include "board.h"



board* board_new(unsigned int width, unsigned int height, enum type type){

    if(width == 0 || height == 0){
        fprintf(stderr, "Undefined Dimensions");
        exit(1);
    }

    board* brd = (board*)malloc(sizeof(board));
    brd->width = width;
    brd->height = height;
    brd->type = type;

    if(type == BITS){                                                              
        unsigned int arr_len = ((width * height)/16) + 1;                                            
        unsigned int* arr = (unsigned int*)malloc(sizeof(unsigned int) 
                                                         * arr_len);
        if (arr == NULL){                                                       
            fprintf(stderr, "Error allocating Memory");                            
            exit(1);                                                               
        }
        for(unsigned int i = 0; i < arr_len; i++){
            arr[i] = 0;
        }
        brd->u.bits = arr;
        return brd;
    }

    
    cell** mtx = (cell**)malloc(sizeof(cell*) * height);
    if (mtx == NULL){                                        
        fprintf(stderr, "Error allocating Memory"); 
        exit(1);                                                          
    }
    for(unsigned int i = 0; i < height; i++){
        mtx[i] = (cell*)malloc(sizeof(cell) * height);
        if (mtx[i] == NULL){                                               
            fprintf(stderr, "Error allocating Memory");                    
            exit(1);                                                        
        }
    }

    brd->u.matrix = mtx;

    for(unsigned int r = 0; r < width; r++){
        for(unsigned int c = 0; c < height; c++){
            brd->u.matrix[r][c] = EMPTY;
        }
    }

    return brd;

}



void board_free(board* b){
    if(b->type == MATRIX){
        for(unsigned int i = 0; i < b->height; i++){
            free(b->u.matrix[i]);   
        }
        free(b);
    }   
    else{
        free(b->u.bits);
        free(b);
    }
}



cell board_get(board* b, pos p){                                                   
                                                                                   
    switch(b->type){                                                               
                                                                                   
        case MATRIX:                                                               
            if((p.r <= b->height - 1) && (p.c <= b->width - 1)){                   
                return b->u.matrix[p.r][p.c];                                      
            }                                                                      
            else {                                                                 
                printf("Row: %d, column: %d", p.r,p.c);                            
                fprintf(stderr, "Out of bounds.");                                 
                exit(1);                                                           
            }                                                                      
        case BITS:                                                                 
            if((p.r <= b->height - 1) && (p.c <= b->width - 1)){                   
                unsigned int posit = ((p.r * b->width) + p.c) * 2;            
                unsigned int indx = (((p.r * b->width) + p.c + 1) * 2) - 2; 
                unsigned int bit1 = (b->u.bits[indx/32] >> posit) & 1;
                unsigned int bit2 = (b->u.bits[indx/32] >> (posit + 1)) & 1;
                if(bit1 == 0 && bit2 == 1){
                    return WHITE;
                }
                else if(bit1 == 1 && bit2 == 0){                                     
                    return BLACK;                                               
                }
                else{                                     
                    return EMPTY;                                               
                }
            }                                                                      
            else {                                                                 
                printf("Row: %d, column: %d", p.r,p.c);                            
                fprintf(stderr, "Out of bounds.");                                 
                exit(1);                                                           
            }                                                                      
    }                                                                              
} 



void board_show_helper(unsigned int i){

    if(i <= 9){                                                             
        printf("%c", i+48);                                                 
    }                                                                       
    else if(i > 9 && i <= 35){                                              
        printf("%c", i+55);                                                 
    } 
    else if(i > 35 && i <= 61){
        printf("%c", i+62);
    }
    else{
        printf("%c", '?');
    }

}



void board_show(board* b){

    printf("  ");                                                           
    for(unsigned int i = 0; i < b->width; i++){                             
        board_show_helper(i);                                               
    }                                                                       
    printf("\n\n");
    pos p; 

    for(unsigned int i = 0; i < b->height; i++){                               
        board_show_helper(i);                                                  
        printf(" ");                                                           
        for(unsigned int j = 0; j < b->width; j++){  
            p.r = i;
            p.c = j;                          
            if(board_get(b, p) == EMPTY){                                    
                printf(".");                                                   
            }                                                                  
            else if(board_get(b, p) == WHITE){                                    
                printf("o");                                                   
            }                                                                  
            else{                                    
                printf("*");                                                   
            }                                                                  
        }                                                                      
        printf("\n");                                                          
    }                                                                        
} 



void bits_clear(board* b, pos p){ 
    unsigned int posit = ((p.r * b->width) + p.c) * 2;
    unsigned int indx = (((p.r * b->width) + p.c + 1) * 2) - 2;
    b->u.bits[indx/32] &= ~(1 << posit);
    b->u.bits[indx/32] &= ~(1 << (posit + 1));
}



void board_set(board* b, pos p, cell c){

    if(b->type == MATRIX){
        if((p.r <= b->height - 1) && (p.c <= b->width - 1)){
            b->u.matrix[p.r][p.c] = c;
        }
        else {
            printf("Row: %d, column: %d", p.r,p.c);                        
            fprintf(stderr, "Out of bounds");                               
            exit(1);                                                        
        }
    }
    else{                                                              
        if((p.r <= b->height - 1) && (p.c <= b->width - 1)){                    
            unsigned int flag, posit = ((p.r * b->width) + p.c) * 2;
            unsigned int indx = (((p.r * b->width) + p.c + 1) * 2) - 2;
            if(c == WHITE){
                bits_clear(b, p);
                flag = 2;
                flag = flag << posit;
                b->u.bits[indx/32] = b->u.bits[indx/32] | flag;
            }   
            else if(c == BLACK){ 
                bits_clear(b, p);                                                    
                flag = 1;                                                       
                flag = flag << posit;                                           
                b->u.bits[indx/32] = b->u.bits[indx/32] | flag;                       
            }
            else{                                                     
                bits_clear(b, p);                  
            }
        }                                                                       
        else {                                                                  
            printf("Row: %d, set column: %d", p.r,p.c);                             
            fprintf(stderr, "Out of bounds");                                   
            exit(1);                                                            
        }
    }
    
}
