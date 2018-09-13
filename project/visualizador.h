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

#ifndef VISUALIZADOR_H
#define VISUALIZADOR_H

#include "SDL2/SDL.h"
#include "body.h"
#include <vector>

using namespace std;

/**S
 * Visualizador simplificado para a disciplina de Super Computação do INSPER - 2018/2.
 *
 * Este Visualizador recebe um vector de corpos circulares com massa, raio, posição e
 * velocidade, as dimensões do campo de simulação e o passo da simulação. A cada frame
 * da simulação a função `do_iteration` é executada. Coloque o código da sua simulação
 * neste método. Você pode escolher criar uma subclasse ou simplesmente escrever seu
 * código neste método. Criar uma subclasse tem a vantagem de poder criar visualizadores
 * para os diferentes modelos de física.
 *
 */
class Visualizador
{
public:
    long iter;
    double delta_t;

    Visualizador(std::vector<Ball> &bodies, int field_width, int field_height, double delta_t);
    ~Visualizador();

    void do_iteration();
    void run();

private:
    SDL_Window *win;
    SDL_Renderer *renderer;
    int win_width, win_height;
    int field_width, field_height;
    std::vector<Ball> &bodies;

    const int max_dimension = 700;

    void draw();

};

#endif // VISUALIZADOR_H
