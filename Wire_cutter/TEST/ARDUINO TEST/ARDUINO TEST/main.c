//
//  main.c
//  ARDUINO TEST
//
//  Created by Jeremy Bartos on 1/18/15.
//  Copyright (c) 2015 Jeremy Bartos. All rights reserved.
//

#include <stdio.h>
#include <math.h>

int main(int argc, const char * argv[]) {
    // insert code here...
    const float C_PI = 3.1415926;
    const float C_DIAM = 1;
    const int STEPS = 200;
    const int MICROSTEP = 8;
    int length = 37;
    float CIRC = (2*C_PI*(C_DIAM/2));
    float CIRC_PER_STEP = (CIRC / (STEPS*MICROSTEP));
    int steps_til_done = (length/CIRC_PER_STEP);
    float f_steps_til_done = (length/CIRC_PER_STEP);
    printf("CIRC = %1.7f\n",CIRC);
    printf("CIRC_PER_STEP = %1.7f\n",CIRC_PER_STEP);
    int x = CIRC_PER_STEP;
    printf("INT = %i\n",x);
    printf("STEPS til done = %i\n",steps_til_done);
    printf("STEPS til done = %3.7f\n",f_steps_til_done);
    float modelo = f_steps_til_done - steps_til_done;
        printf("STEPS til done dec only = %3.7f\n",modelo);
    if ((f_steps_til_done - steps_til_done) >= .5){
        steps_til_done = steps_til_done+1;
    }
        printf("STEPS til done w/ fix = %i\n",steps_til_done);
    for (int x = 0; x < steps_til_done; x++) {
        int length_left = x*CIRC_PER_STEP;
        printf("%i Inches cut\n",length_left);
    }
    printf("Cut Code\n");
    return 0;
}
