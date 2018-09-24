#include <iostream>
#include <queue>
#include <stdlib.h>
#include <math.h>
#include <chrono>
#include <thread>
#include "semaphore.h"

using namespace std;

void produce(queue<int>& q, Semaphore& sm){

	int n = rand() % 101;
	q.push(n);
	cout << "P: " << n << "\n";
	this_thread::sleep_for(std::chrono::milliseconds(500));
	sm.release();

	produce(q,sm);
}

void consume(queue<int>& q, Semaphore& sm){

	sm.acquire();

	cout << "C: " <<pow(q.front(),2) << "\n";
	q.pop();	
	this_thread::sleep_for(std::chrono::milliseconds(300));
	
	consume(q,sm);
}

int main(){

	srand (time(NULL));
	queue<int> q;

	Semaphore sm;
    sm.init();

	thread t1(produce,ref(q),ref(sm));
	thread t2(consume,ref(q),ref(sm));
	//thread t3(consume,ref(q),ref(sm));
	//thread t4(produce,ref(q),ref(sm));

	t1.join();
	t2.join();
	//t3.join();
	//t4.join();
}