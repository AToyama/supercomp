#include <iostream>
#include <thread>
#include <condition_variable>
#include <chrono>
#include <mutex>

std::mutex r0, r1;
std::condition_variable t0, t1, t2;
bool done1 = false;
bool done2 = false;

void thread0(int &resultado_para_thread1) {
    // faz trabalho longo
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    {
        std::unique_lock<std::mutex> lk(r0);
        resultado_para_thread1 = 10;
        done1 = true;
        t0.notify_one();
        t2.notify_one();
    }
    
    // faz trabalho longo
    std::this_thread::sleep_for(std::chrono::milliseconds(1300));
    std::cout << "Fim thread0!" << std::endl;
}

void thread1(int const &resultado_da_thread0, int &resultado_para_thread2) {

    {
        std::unique_lock<std::mutex> lk(r0);
        t0.wait(lk, []{return done1; });
    }

    // faz trabalho long com resultado de thread0
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));

    {
        std::unique_lock<std::mutex> lk(r1);
        
        // finja que isto depende da thread0
        resultado_para_thread2 = 20; 
        done2 = true;

        t1.notify_one();
    }
    
    std::cout << "Fim thread1!" << std::endl;
}

void thread2(int const &resultado_thread_0, int const &resultado_thread_1) {
    
    {
        std::unique_lock<std::mutex> lk(r0);
        t2.wait(lk, []{return done1; });
    }

    // faz trabalho longo com resultado de thread0
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    
    {
        std::unique_lock<std::mutex> lk(r1);
        t1.wait(lk, []{return done2; });
    }

    // faz trabalho longo com resultado de thread1
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
   
    std::cout << "thread2:" << resultado_thread_0 + resultado_thread_1 << "\n";
    std::cout << "Fim thread2!" << std::endl;
}

int main(int argc, char **argv) {
    int res_t0, res_t1;
    
    std::thread t1(thread0,std::ref(res_t0));
    std::thread t2(thread1,std::ref(res_t0), std::ref(res_t1));
    std::thread t3(thread2,std::ref(res_t0), std::ref(res_t1));
    
    t1.join();
    t2.join();
    t3.join();

    return 0;
}
