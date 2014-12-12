#include <iostream>
#include <thread>
#include <functional>
#include "CountDownLatch.h"

void haha(my_tt::CountDownLatch &c) {
	c.countDown();
	c.await();
	std::cout << "fuck" << std::endl;
	//c.countDown();
	//c.await();
	std::cout << "shit" << std::endl;
}
int main() {
	my_tt::CountDownLatch c(3);
	std::thread t1(haha, std::ref(c));
	std::thread t2(haha, std::ref(c));
	std::cout << c.getParties() << std::endl;
	std::thread t3(haha, std::ref(c));
	t1.join();
	t2.join();
	t3.join();
	system("pause");
	return 0;
}
