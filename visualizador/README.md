# Visualizador simplificado de dinâmica 2D

Este visualizador foi criado para a Disciplina de SuperComputação do INSPER (2018/2)
para o projeto 1. Ele é implementado em *SDL2* e usa CMake para compilação.

Se você não conhece CMake, leia [este tutorial](https://cmake.org/cmake-tutorial/). Este outro texto ([CMake by Example](http://derekmolloy.ie/hello-world-introductions-to-cmake/)) também é interessante e pode ser uma introdução um pouco mais prática a esta ferramenta. 


O código do visualizador pode ser modificado por vocês. 
 
Limitações:

1. O código desenha bolinhas quando o método `draw()` é chamado. O ideal seria usar
   recursos de hardware para os desenhos.
2. Não é feito nenhum controle de tempo nem existe nenhum mostrador do tempo da simulação
   e sua relação com o tempo do relógio. 

### Dependências de compilação

* SDL2
* SDL2_gfx

