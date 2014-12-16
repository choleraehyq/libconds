#include <iostream>
#include <thread>
#include <string>
#include "ReadWriteLock.h"

int num = 0;
my_tt::ReadWriteLock lock;

void heheread() {
    lock.get_read_lock();
    std::cout << std::to_string(num) + "\n";
    lock.release_read_lock();
}

void hehewrite() {
    lock.get_write_lock();
    num++;
    lock.release_write_lock();
}

int main() {
    std::thread w1(hehewrite);
    std::thread w2(hehewrite);
	std::thread r1(heheread);
	std::thread r2(heheread);
	r1.join();
	r2.join();
	w1.join();
	w2.join();
	system("pause");
    return 0;
}
