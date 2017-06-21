//
//  rldecode.c
//  rldecode
//
//  Created by Zhang Yunhe on 12/03/2017.
//  Copyright © 2017 Zhang Yunhe. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    /* 
        since max amount of a compressed character is 4 bytes,
        create a size 4 array and initialize it.
     */
    char * a;
    a = (char *)malloc(4);
    int cur_a = 3;                                          //for recording current point in array a
    while (cur_a != 0) {                                    //clear the array "a" and set current point to 0
      a[cur_a] = '\0';
      cur_a--;
    }
    
    /*
        determine the number of parameter and set variable t
        1 parameter -> t = 0
        2 parameter -> t = 1
        no parameter -> end program
     */
    int t = 0;
    switch (argc) {
        case 1:
            printf("No Parameter");
            return 0;
            break;
        case 3:
            t = 1;
        default:
            break;
    }
    
    FILE *f_in = fopen(argv[1], "r");                   //set input file "f_in"
    FILE *f_out = NULL;
    if (t == 1) {
        f_out = fopen(argv[2], "w");                    //set output file "f_out" if have second parameter
    }
    
    int num = 0;                                        //for counting the number of continuous repeated bytes
    unsigned char pre_ch, cur_ch;                       //store the current character and previous character
    fread(&cur_ch, 1, 1, f_in);                         //initialize operation
    pre_ch = '\0';
    
    
    /*  
        read byte by byte from file
     
        1. if first bit is 0, check if array "a" is empty, if not, calculate the number from array "a",then
        output the number or wirte the number(plus 2) of previous byte to output file, clear the array. 
        No matter array "a" is empty or not, print or write current byte and update previous byte to current byte.
        
        2. if first bit is 1, set first bit to 0  and store it into array "a".
     */
    while (1) {
        if (cur_ch >> 7 == 0 || feof(f_in)) {                           //if first bit is 0 or reach end of file
            if (cur_a > 0) {                                            //check if array "a" is empty
                int i;
                for (i = 0; i < cur_a; i++) {                           //calculate number from array "a", store in "num"
                    num = (int)(num << 7 | a[i]);
                }
                if (t == 0) {                                           //if no output file, print number on screen
                    printf("[%d]", num);
                }
                else {                                                  //if have output file
                    int i;
                    for (i = 0; i < num + 2; i++) {
                        fwrite(&pre_ch, sizeof(char), 1, f_out);        //write number(num + 2) of privious bytes in file
                    }
                }
                while (cur_a != 0) {                                    //clear the array "a" and set current point to 0
                    a[cur_a] = '\0';
                    cur_a--;
                }
                num = 0;                                                //initialze the "num" to 0
            }
            if (feof(f_in)) {                                           //reach end of file,end the loop
                break;
            }
            if (t == 0) {                                               //print or write the current byte
                printf("%c", cur_ch);
            }
            else {
                fwrite(&cur_ch, sizeof(char), 1, f_out);
            }
            pre_ch = cur_ch;                                            //update the previous byte to current byte
        }
        else {                                                          //if first bit is 1
            a[cur_a] = (int)cur_ch & 0X7F;                              //set first bit to 0 then store it into array "a"
            cur_a++;                                                    //update the current point in array "a"
        }
        fread(&cur_ch, 1, 1, f_in);                                     //get next byte
    }
    fclose(f_in);                                                       //close file
    if (t == 1) {
        fclose(f_out);
    }
    return 0;
}
