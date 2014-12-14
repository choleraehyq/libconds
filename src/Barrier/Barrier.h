#ifndef BARRIER_H
#define BARRIER_H

#include <mutex>
#include <condition_variable>
#include <functional>

namespace my_tt {
	class Barrier {
	public:
		template <typename F&&, typename... Args&&>
		Barrier(int n, F&& f, Args&&... args)
			:target(n), func(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
		Barrier() = delete;
		Barrier(const Barrier &) = delete;
		Barrier &operator=(const Barrier &) = delete;
		void await();
		int getParties();
	private:
		int target;
		volatile int nwaiting;
		std::mutex mtx;
		std::condition_variable cv;
		std::function<void()> func;
	};
	int Barrier::getParties() {
		return this->nwaiting;
	}
	void Barrier::await() {
		std::unique_lock<std::mutex> lock(this->mtx);
		this->nwaiting++;
		if ( this->nwaiting == this->target ) {
			cv.notify_all();
			this->func();
		}
		else 
			this->cv.wait(lock, [this] { return this->nwaiting == this->target; });
	}
}

#endif