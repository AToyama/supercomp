//time: 4194.42s

#include <stdio.h>
#include <math.h>
#include <chrono>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>


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
void update_ball(Ball& ball,double dT)
{
	//ball position update
    ball.x += ball.vx*dT;
    ball.y += ball.vy*dT;

    //ball speed update
    if(check_sign(ball.vx)){
        if (ball.vx - ball.accx*dT <= 0){
            ball.vx = 0;
        }
        else{
            ball.vx -= ball.accx*dT; 
        }    
    }
    else{
        if (ball.vx + ball.accx*dT >= 0){
            ball.vx = 0;
        }
        else{
            ball.vx += ball.accx*dT; 
        }  
    }

    if(check_sign(ball.vy)){
        
        if (ball.vy - ball.accy*dT <= 0){
            ball.vy = 0;
        }
        else{

            ball.vy -= ball.accy*dT; 
        }    
    }
    else{
        if (ball.vy + ball.accy*dT >= 0){
            ball.vy = 0;
        }
        else{
            ball.vy += ball.accy*dT; 
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
bool colide(Ball ball1, Ball ball2)
{
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

    vx1 = (ball1.vx*(ball1.mass - ball2.mass) + 2*ball2.mass*ball2.vx)/(ball1.mass + ball2.mass);
    vy1 = (ball1.vy*(ball1.mass - ball2.mass) + 2*ball2.mass*ball2.vy)/(ball1.mass + ball2.mass);
    vx2 = (ball2.vx*(ball2.mass - ball1.mass) + 2*ball1.mass*ball1.vx)/(ball2.mass + ball1.mass);
    vy2 = (ball2.vy*(ball2.mass - ball1.mass) + 2*ball1.mass*ball1.vy)/(ball2.mass + ball1.mass);

    ball1.vx = vx1;
    ball1.vy = vy1;
    ball2.vx = vx2;
    ball2.vy = vy2;
}

//function that calculate the acceleration projection in x and y
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
    for (int unsigned i = 0; i < input[2]; ++i)
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

    //create obj table
    Table table;
    table.width = input[0];
    table.height = input[1];

	double delta_t;

    char* dT = getenv("DELTA_T");

    if(dT){
    	delta_t = atof(dT);
    }
    else{
    	delta_t = 0.01;
    }

    int iter = 100000;

    chrono::time_point<chrono::high_resolution_clock> start, end;
    start = chrono::high_resolution_clock::now();

    //simulate 100.000 iterations of the simulation
    for (int w = 0; w < iter; ++w)
    {
	    
        //iterate a list with every ball
        for (int unsigned i = 0; i < balls.size(); i++){

            //update ball position
            update_ball(balls[i],delta_t);

            //check collision with the table
            ball_table_col(balls[i], table);

            //iterate with the rest of the list
            for (int unsigned j = 0; j < balls.size(); j++){

                if(balls[i].id != balls[j].id){
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