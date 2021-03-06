/*
 * Copyright (c) 2018 Igor Montagner igordsm@gmail.com
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <math.h>
#include <stdlib.h>

#include "visualizador.h"

using namespace std;

void acc_calc(double mu, Ball& ball) {

    double v = sqrt(pow(ball.vx,2) + pow(ball.vy,2));
    double cos = ball.vx/v;
    double sen = ball.vy/v;

    double acc = 9.8*ball.mass*mu;

    ball.accx = acc*cos;
    ball.accy = acc*sen;

    if(ball.vx == 0 and ball.vy == 0){
        ball.accx = 0;
        ball.accy = 0;
    }

    if(ball.accx < 0){
        ball.accx *= -1;
    }

    if(ball.accy < 0){
        ball.accy *= -1;
    }   
}

int main(int argc, char ** argv) {

    // input parsing

    ifstream inFile(argv[1]);
    string value;
    vector<double> input;

    while(inFile >> value){
        input.push_back(stod(value));
    }

/* input guide

    0 - table width
    1 - table height
    2 - number of balls
    3 - mu
    4 - alpha_w
    5 - alpha_b

    ----------- // --------

    6 - id
    7 - radius
    8 - mass
    9 - x
    10 - y
    11 - vx
    12 - vy

    then repeat the last block for the number of balls

*/
    vector<Ball> balls;

    // create balls, with the respective attributes

    for (int i = 0; i < input[2]; ++i)
    {
        Ball ball;
        ball.id = input[6+7*i];
        ball.radius = input[7+7*i];
        ball.mass = input[8+7*i];
        ball.x = input[9+7*i];
        ball.y = input[10+7*i];
        ball.vx = input[11+7*i];
        ball.vy = input[12+7*i];
        acc_calc(input[3], ball);
        balls.push_back(ball);
    }

    //capture the enviroment variable DELTA_T
    char* dT = getenv("DELTA_T");

    //srun the simulation with the right DELTA_T
    if(dT){
        Visualizador v(balls, input[0], input[1], atof(dT));
        v.run();
    }
    else{
        Visualizador v(balls, input[0], input[1], 0.01);   
        v.run();
    }
    

    return 0; 
}