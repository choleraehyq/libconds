#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <condition_variable>
#include <mutex>

namespace my_tt {
	class semaphore {
		friend void p(semaphore &sem);
		friend void v(semaphore &sem);
	public:
		semaphore() :num(0) {}
		semaphore(const int &x) :num(x) {}
		semaphore(const semaphore &) = delete;
	private:
		std::mutex mtx;
		std::condition_variable cv;
		int num;
	};
	void v(semaphore &sem) {
		std::unique_lock<std::mutex> lock(sem.mtx);
		sem.num++;
		sem.cv.notify_one();
	}
	void p(semaphore &sem) {
		std::unique_lock<std::mutex> lock(sem.mtx);
		sem.num--;
		sem.cv.wait(lock, [&sem] { return sem.num >= 0; });
	}
}

#endif