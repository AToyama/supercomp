#include <thread>
#include <iostream>
#include <mutex>

using namespace std;


void print_paralelo(int a, mutex& a_mutex) {

	lock_guard<mutex> lock(a_mutex);
 	cout << "id = " << a << "\n";

}

int main() {

	mutex a_mutex;

    thread t1(print_paralelo, 0,ref(a_mutex));
    thread t2(print_paralelo, 1,ref(a_mutex));
    thread t3(print_paralelo, 2,ref(a_mutex));
    thread t4(print_paralelo, 3,ref(a_mutex));

    t1.join();
    t2.join();
    t3.join();
    t4.join();

}