#include <stdio.h>
#include <math.h>
#include <chrono>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <omp.h>

#include "body.h"


using namespace std;


typedef std::chrono::high_resolution_clock Time;

// function to check if the speed is positive or negative
int check_sign(double num) 
{ 
    int res; 
    if (num >= 0) {
      res = 1; 
    }
    else {
     res = 0; 
    }
    return res; 
} 

// function that updates the ball position and speed for each iteration
void update_ball(Ball& ball)
{

    double dT = 0.01;

    ball.x += ball.vx*dT;
    ball.y += ball.vy*dT;

    if(check_sign(ball.vx)){
        if (ball.vx - ball.acc*dT <= 0){
            ball.vx = 0;
        }
        else{
            ball.vx -= ball.acc*dT; 
        }    
    }
    else{
        if (ball.vx + ball.acc*dT >= 0){
            ball.vx = 0;
        }
        else{
            ball.vx += ball.acc*dT; 
        }  
    }

    if(check_sign(ball.vy)){
        
        if (ball.vy - ball.acc*dT <= 0){
            ball.vy = 0;
        }
        else{

            ball.vy -= ball.acc*dT; 
        }    
    }
    else{
        if (ball.vy + ball.acc*dT >= 0){
            ball.vy = 0;
        }
        else{
            ball.vy += ball.acc*dT; 
        }  
    }
}

// function that treats the ball speed after the collision with the table
void ball_table_col(Ball& ball, Table table)
{
    if (ball.y - ball.radius <= 0 || ball.y + ball.radius >= table.height)
        {
            ball.vy *= -1;
        }

    if (ball.x - ball.radius <= 0 || ball.x + ball.radius >= table.width)
        {   
            ball.vx *= -1;
        }   

}


// function that check if there were a collision between two balls
bool colide(Ball ball1, Ball ball2){
    double srq_radius = pow((ball1.radius + ball2.radius),2);
    double dist_sqr = pow((ball1.x - ball2.x),2) + pow((ball1.y - ball2.y),2);

    if (dist_sqr <= srq_radius)
    {
        return true;
    }

    return false;

}

// function that treats that speed vectors after a collision between two balls
void ball_to_ball_col(Ball& ball1, Ball& ball2){

    double vx1,vx2,vy1,vy2;
    double dT = 0.01;

    vx1 = (ball1.vx*(ball1.mass - ball2.mass) + 2*ball2.mass*ball2.vx)/(ball1.mass + ball2.mass);
    vy1 = (ball1.vy*(ball1.mass - ball2.mass) + 2*ball2.mass*ball2.vy)/(ball1.mass + ball2.mass);
    vx2 = (ball2.vx*(ball2.mass - ball1.mass) + 2*ball1.mass*ball1.vx)/(ball2.mass + ball1.mass);
    vy2 = (ball2.vy*(ball2.mass - ball1.mass) + 2*ball1.mass*ball1.vy)/(ball2.mass + ball1.mass);

    ball1.vx = vx1;
    ball1.vy = vy1;
    ball2.vx = vx2;
    ball2.vy = vy2;
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
        ball.acc = 9.8*ball.mass*input[3];
        balls.push_back(ball);
    }

    //create obj table
    Table table;
    table.width = input[0];
    table.height = input[1];

    int iter = 100000;
    int i,j,w;

    chrono::time_point<chrono::high_resolution_clock> start, end;
    start = chrono::high_resolution_clock::now();

    for (w = 0; w < iter; ++w)
    {
        #pragma omp parallel
        {

            int nthreads = omp_get_num_threads();
            int id  = omp_get_thread_num();

            for (int i = id *balls.size()/nthreads; i < (id +1)*balls.size()/nthreads; ++i) {

                //update ball position
                update_ball(balls[i]);

                //check collision with the table
                ball_table_col(balls[i], table);

                //iterate with the rest of the list
                for (j = i+1; j < balls.size(); j++){

                    //check and resolve collision ball to ball
                    if(colide(balls[i],balls[j])){
                        ball_to_ball_col(balls[i],balls[j]);
                    }
                }
            }
        }

    
    }

    end = chrono::high_resolution_clock::now();
    chrono::duration<double> func0 = end - start;
    cout << "time: " << func0.count() << "s\n";


    return 0; 
}