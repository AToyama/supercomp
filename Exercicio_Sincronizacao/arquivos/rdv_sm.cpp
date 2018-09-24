#include <iostream> 
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "semaphore.h"

std::mutex A, B;
std::condition_variable cA, cB;

bool cond_a = false;
bool cond_b = false;

void tarefaA(Semaphore &sm,Semaphore &sm1) {

    std::cout << "Faz trabalho A\n";
    
    sm.release();
    sm1.acquire();
    
    
// DESEJO ESPERAR POR B AQUI!
    std::cout << "Fim trabalho A\n";
}

void tarefaB(Semaphore &sm,Semaphore &sm1) {

    std::cout << "Faz trabalho B\n";

    sm1.release();
    sm.acquire();

// DESEJO ESPERAR POR A AQUI!
    std::cout << "Fim trabalho B\n";
}

int main(int argc, char *argv[]) {
    
    Semaphore sm;
    Semaphore sm1;
    sm.init();
    sm1.init();

    std::thread t1(tarefaA,std::ref(sm),std::ref(sm1));
    std::thread t2(tarefaB,std::ref(sm),std::ref(sm1));
    
    
    t1.join();
    t2.join();
    return 0;
}
