#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <condition_variable>
#include <mutex>

namespace my_tt {
	class Semaphore {
		friend void p(Semaphore &sem);
		friend void v(Semaphore &sem);
	public:
		Semaphore() :num(0) {}
		Semaphore(const int &x) :num(x) {}
		Semaphore(const Semaphore &) = delete;
		Semaphore &operator=(const Semaphore &) = delete;
		~Semaphore();
	private:
		std::mutex mtx;
		std::condition_variable cv;
		int num;
	};
	void v(Semaphore &sem) {
		std::unique_lock<std::mutex> lock(sem.mtx);
		sem.num++;
		sem.cv.notify_one();
	}
	void p(Semaphore &sem) {
		std::unique_lock<std::mutex> lock(sem.mtx);
		sem.num--;
		sem.cv.wait(lock, [&sem] { return sem.num >= 0; });
	}
	Semaphore::~Semaphore() {
		this->num = 1;
		this->cv.notify_all();
	}
}

#endif