//
//  main.c
//  Comp9319 ass1
//
//  Created by Zhang Yunhe on 11/03/2017.
//  Copyright © 2017 Zhang Yunhe. All rights reserved.
//

#include <stdio.h>
#include <math.h>

/* 
    this funciton for count bits of an integer
 */
int count_bits(int n);

int main(int argc, char **argv)
{
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
            return 1;
            break;
        case 3:
            t = 1;
        default:
            break;
    }
    
    FILE *f_in = fopen(argv[1], "r");                   //set input file "f_in"
    FILE *f_out = NULL;
    if (t == 1) {
        f_out = fopen(argv[2], "wb");                   //if t = 1, set output file "f_out"
    }
    unsigned char cur_ch, pre_ch;                       //for storing the current and previous chracters
    int num = 0;                                        //for counting the number of continous characters
    int a;                                              //for calculating number of bits of "num"
    unsigned char seg1, seg2, seg3, seg4;               //consider the max value of "num", 4 segments should be enough
    fread(&cur_ch, 1, 1, f_in);                         //initialize the current character.
    pre_ch = '\0';                                      //initialize the previous character.
    
    /*
        read byte by byte from input file
     
        1.if current character is different from previous character or reach the end of file,
        do compress operation then print result on the screen or write result in output file
     
        2.if current character is same as previous character, "num" plus 1
        
        update the previous and and current characters
     */
    while (1) {
        if (cur_ch != pre_ch || feof(f_in)) {                            //do compress operation
            if (num == 0 || num == 1) {                                  //if no repeat, or just repeat once
                if (pre_ch != '\0') {
                    int i;
                    for (i = 0; i <= num; i++){                      //print or write the character(s)
                        if (t == 0) {
                            printf("%c", pre_ch);
                        }
                        else {
                            fwrite(&pre_ch, sizeof(char), 1, f_out);
                        }
                    }
                }
            }
            else {                                                      //if repeat more than 3 times
                num = num - 2;
                if (t == 0) {                                           //print result on screen
                    printf("%c[%d]", pre_ch, num);
                }
                /*  
                    write the result to the file
                    1.write the character first
                    2.calculate the number of digits of num in binary, store in "a"
                    3.divide "num" into 4 segments and set each segments' first bit to 1
                    4.write segments into file, number of segments depends on value "a"
                */
                else {
                    fwrite(&pre_ch, sizeof(char), 1, f_out);
                    a = count_bits(num);
                    seg4 = ((num >> 21) & 0x7F) | 0x80 ;
                    seg3 = ((num >> 14) & 0x7F) | 0x80;
                    seg2 = ((num >> 7) & 0x7F) | 0x80;
                    seg1 = ((num >> 0) & 0x7F) | 0x80;
                    if (a <= 7){
                        fwrite(&seg1, sizeof(char), 1, f_out);
                    }
                    else if (a > 7 && a <= 14) {
                        fwrite(&seg2, sizeof(char), 1, f_out);
                        fwrite(&seg1, sizeof(char), 1, f_out);
                    }
                    else if (a > 14 && a <= 21) {
                        fwrite(&seg3, sizeof(char), 1, f_out);
                        fwrite(&seg2, sizeof(char), 1, f_out);
                        fwrite(&seg1, sizeof(char), 1, f_out);
                    }
                    else if (a > 21 && a <= 28) {
                        fwrite(&seg4, sizeof(char), 1, f_out);
                        fwrite(&seg3, sizeof(char), 1, f_out);
                        fwrite(&seg2, sizeof(char), 1, f_out);
                        fwrite(&seg1, sizeof(char), 1, f_out);
                    }
                    a = 0;
                }
            }
            num = 0;                                                    //end of compress operation, set num to 0
        }
        else {
            num++;                                                      //each time the character repeated, num plus 1
        }
        if (feof(f_in)) {                                               //reach end of file, get out of loop
            break;
        }
        pre_ch = cur_ch;                                                //update previous byte and get next byte
        fread(&cur_ch, 1, 1, f_in);
    }
    fclose(f_in);                                                       //close file
    if(t == 1){
        fclose(f_out);
    }
    return 0;
}

/*
 *  function for counting the bits of an integer
 */
int count_bits(int n)
{
  int count = 0;
  while(n)
  {
    count++;
    n = n >> 1;
  }
  return count;
}
