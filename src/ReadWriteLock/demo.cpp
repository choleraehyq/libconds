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
    std::thread r1(heheread);
    std::thread r2(heheread);
    std::thread w1(hehewrite);
    std::thread w2(hehewrite);
    return 0;
}
