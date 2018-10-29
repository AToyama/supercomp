#include <iostream>
#include <mutex>
#include <condition_variable>

using namespace std;

class Semaphore{
    
    private:
	    int count;
	    mutex m;
	    condition_variable cv;

	public:

		void  init(int n){
			count = n;
		}

		void acquire(){

	        std::unique_lock<std::mutex> lk(m);

	        cv.wait(lk, [this]{return count > 0;});

		    count--;
		    }

		void release(){
		
			unique_lock<mutex> lk(m);
		    count++;
		    cv.notify_one();
		}		
};