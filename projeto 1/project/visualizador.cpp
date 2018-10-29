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

#include "visualizador.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"

#include <stdio.h>
#include <math.h>
#include <chrono>
#include <omp.h>

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

Visualizador::Visualizador(std::vector<Ball> &bodies, int field_width, int field_height, double delta_t) :

    delta_t(delta_t),
    field_width(field_width),
    field_height(field_height),
    bodies(bodies) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    double ratio = (double) field_width / field_height;
    if (ratio > 1) {
        win_width = max_dimension;
        win_height = max_dimension / ratio;
    } else {
        win_width = max_dimension * ratio;
        win_height = max_dimension;
    }
    win = SDL_CreateWindow("Visualizador SUPERCOMP", SDL_WINDOWPOS_CENTERED,
                           SDL_WINDOWPOS_CENTERED, win_width, win_height, 0);
    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    iter = 0;

}

Visualizador::~Visualizador() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
}


void Visualizador::draw() {
    SDL_SetRenderDrawColor(renderer, 160, 160, 185, 255);
    SDL_RenderClear(renderer);
    for (auto i = bodies.begin(); i != bodies.end(); i++) {
        filledCircleRGBA(renderer, i->x / field_width * win_width,
                         i->y / field_height* win_height,
                         i->radius / field_width * win_width,
                         255, 0 ,0, 255);
    }
    SDL_RenderPresent(renderer);
}


void Visualizador::run() {

    SDL_ShowWindow(win);
    draw();
    while (!SDL_QuitRequested()) {
        do_iteration();       
        draw();
    }
}

void Visualizador::do_iteration() {

    //capture enviroment variable GUI
    char* gui = getenv("GUI");

    if(gui){
        if(atof(gui) == 0)
        {   
            //print real time of simulation
            if(iter%1000 == 0){
                printf("\n%d seconds\n",int(iter/1000));
            }

            //prints a list of information from all bodies every second
            for (int unsigned i = 0; i < bodies.size(); ++i){
                if(iter%1000 == 0){
                    printf("%d %f %f %f %f %f %f\n", bodies[i].id,bodies[i].radius,bodies[i].mass,bodies[i].x,bodies[i].y,bodies[i].vx,bodies[i].vy);
                }
            }          

        }
    }

    //create obj table
    Table table;
    table.width = field_width;
    table.height = field_height;

    int i,j;
    vector<int> update;

    #pragma omp parallel for
    for (int unsigned k = 0; k < bodies.size(); ++k) {
        update.push_back(1);
    }

    #pragma omp parallel for private(j)
    for (i = 0; i < bodies.size(); ++i) {            
        //iterate with the rest of the list
        for (j = 0; j < bodies.size(); j++){

            if(bodies[i].id != bodies[j].id){
                //check and resolve collision ball to ball
                if(colide(bodies[i],bodies[j])){
                    ball_to_ball_col(bodies[i],bodies[j]);
                    update_ball(bodies[i],delta_t);
                    update_ball(bodies[j],delta_t);
                    update[i] = 0;
                    update[j] = 0;
                }
            }
        }
    }

    #pragma omp parallel for
    for (i = 0; i < bodies.size(); ++i) {
        if (update[i])
        {
            //update ball position
            update_ball(bodies[i],delta_t);

            //check collision with the table
            ball_table_col(bodies[i], table);   
        }
    }

    //iter++;
}