#include <iostream>
#include "threadPool.h"

int main() {
	const int N = 4;
	my_tt::threadPool pool(N);
	std::vector< std::future<int> > result;
	for (int i = 0; i < N; i++) {
		result.emplace_back(
			pool.enqueue(
				[](int i) {
					std::cout << "Hello " << i << std::endl;
					return i*i;
				}, i)
			);
	}
	for (std::future<int> &ret : result) {
		std::cout << ret.get() << std::endl;
	}
	system("pause");
	return 0;
}