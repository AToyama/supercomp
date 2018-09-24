#include <iostream> 
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex A, B;
std::condition_variable cA, cB;

bool cond_a = false;
bool cond_b = false;

void tarefaA() {

    std::cout << "Faz trabalho A" << std::endl;
    
    {
        std::unique_lock<std::mutex> lk(A);
        cA.notify_one();
        cond_a = true;
    }
    {
        std::unique_lock<std::mutex> lk(B);
        cB.wait(lk, []{return cond_b; });
    }
    
    
// DESEJO ESPERAR POR B AQUI!
    std::cout << "Fim trabalho A" << std::endl;
}

void tarefaB() {

    std::cout << "Faz trabalho B" << std::endl;
    {
        std::unique_lock<std::mutex> lk(B);
        cB.notify_one();
        cond_b = true;
    }
    {
        std::unique_lock<std::mutex> lk(A);
        cA.wait(lk, []{return cond_a; });
    }
// DESEJO ESPERAR POR A AQUI!
    std::cout << "Fim trabalho B" << std::endl;
}

int main(int argc, char *argv[]) {
    std::thread t1(tarefaA);
    std::thread t2(tarefaB);
    
    
    t1.join();
    t2.join();
    return 0;
}
