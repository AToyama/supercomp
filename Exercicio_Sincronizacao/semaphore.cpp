#include <iostream>
#include <mutex>
#include <condition_variable>

class Semaphore{
    
    private:
	    int count;
	    mutex m;
	    condition_variable cv;

	public:

		void  init(){
			count = 0;
		}

		void acquire(){

	        std::unique_lock<std::mutex> lk(m);
		    count--;

		    if(n < 0){
		    	cv.wait(lk);
		    }
		}	

		void release(){
		
			unique_lock<mutex> lk(m);
		    count++;
		    cv.notify_one();
		}		
};