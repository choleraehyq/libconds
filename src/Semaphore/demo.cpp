#include <iostream>
#include <thread>
#include <queue>
#include <atomic>
#include "Semaphore.h"

int main() {
	const int N = 100;
	int buffer[N], in = 0;
	std::atomic<int> out(0);
	my_tt::semaphore sem;
	std::thread consumer(
		[&] {
			while (out != N) {
				p(sem);
				std::cout << buffer[out++] << std::endl;
			}
		}
	);
	std::thread consumer2(
		[&] {
			while ( out != N ) {
				p(sem);
				std::cout << buffer[out++] << std::endl;
			}
		}
	);
	std::thread productor(
		[&] {
			for ( int i = 0; i < N; i++ ) {
				v(sem);
				buffer[in++] = i;
			}
		}
	);
	consumer.join();
	consumer2.join();
	productor.join();
	system("pause");
	return 0;
}