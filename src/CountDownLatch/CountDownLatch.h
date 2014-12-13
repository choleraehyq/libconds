#ifndef COUNTDOWNLATCH_H
#define COUNTDOWNLATCH_H
#include <functional>
#include <condition_variable>
#include <chrono>
#include <mutex>

namespace my_tt {
	class CountDownLatch {
	public:
		void countDown();
		void await();
		int getParties();
		CountDownLatch()
			:target(0), nwaiting(0) {}
		CountDownLatch(int num)
			:target(num), nwaiting(0) {}
		CountDownLatch(const CountDownLatch &) = delete;
		CountDownLatch &operator=(const CountDownLatch &) = delete;
		//I will implement it later after i completely understand rvalue.
	private:
		std::condition_variable cv;
		std::mutex mtx;
		int target;
		int nwaiting;
	};
	int CountDownLatch::getParties() {
		return this->nwaiting;
	}
	void CountDownLatch::await() {
		std::unique_lock<std::mutex> lock(this->mtx);
		this->nwaiting++;
		this->cv.wait(lock, [this] { return !this->target; });
	}
	void CountDownLatch::countDown() {
		std::unique_lock<std::mutex> lock(this->mtx);
		this->target--;
		if ( !this->target )
			this->cv.notify_all();
	}
}


#endif // CountDownLatch_H
