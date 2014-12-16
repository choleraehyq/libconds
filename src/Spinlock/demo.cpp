#include <iostream>
#include <thread>
#include <mutex>
#include "Spinlock.h"

my_tt::Spinlock spinlock;
//std::mutex mtx;
int count;

void haha() {
	for ( int i = 0; i < 10000000; i++ ) {
		std::lock_guard<my_tt::Spinlock> lock(spinlock);
		//std::lock_guard<std::mutex> lock(mtx);
		count++;
	}
}

int main() {
	std::thread t1(haha);
	std::thread t2(haha);
	t1.join();
	t2.join();
	std::cout << count << std::endl;
	//system("pause");
	return 0;
}