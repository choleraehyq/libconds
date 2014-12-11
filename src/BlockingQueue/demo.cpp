#include <iostream>
#include <vector>
#include <thread>
#include <memory>
#include "BlockingQueue.h"

int main() {
	const int N = 100000;
	auto pq = std::make_shared<my_tt::BlockingQueue<int> >(1, 0);
	std::thread consumer(
		[pq, N] {
			for ( int i = 1; i < N; i++ ) {
				pq->add(i);
			}
		}
	);
	std::thread productor(
		[pq, N] {
			for ( int i = 0; i < N; i++ ) {
				pq->remove();
			}
		}
	);
	consumer.join();
	productor.join();
	system("pause");
	return 0;
}