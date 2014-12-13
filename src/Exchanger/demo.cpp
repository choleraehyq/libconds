#include <iostream>
#include <thread>
#include <random>
#include <string>
#include "Exchanger.h"

std::random_device ran;
void haha(my_tt::Exchanger<int> &e, const int &id) {
	int i = ran();
	std::cout << std::to_string(id) + std::to_string(i) + "\n";
	int x = e.exchange(i);
	std::cout << std::to_string(id) + std::to_string(x) + "\n";
}
int main() {
	my_tt::Exchanger<int> e;
	std::thread t1(haha, std::ref(e), 1);
	std::thread t2(haha, std::ref(e), 2);
	t1.join();
	t2.join();
	system("pause");
	return 0;
}