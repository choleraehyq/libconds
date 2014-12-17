#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include "Barrier.h"

void haha() {
	std::cout << "haha\n";
}

const int N = 100;
my_tt::Barrier<N, void()> b(haha);

void lala(int i) {
	std::cout << i;
	b.await();
}

int main() {
	std::vector<std::thread> a;
	for ( int i = 0; i < N; i++ )
		a.emplace_back(haha, i);
	return 0;
}
