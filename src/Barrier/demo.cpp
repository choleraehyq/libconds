#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include "Barrier.h"

const int N = 100;
my_tt::Barrier<> b(N, [](int i) { std::cout << "OK!\n"; }, 1);
void haha(int x) {
	std::cout << std::to_string(x)+"\n";
}

int main() {
	std::vector<std::thread> a;
	for ( int i = 0; i < N; i++ )
		a.emplace_back(haha, i);

	return 0;
}