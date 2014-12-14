#ifndef BARRIER_H
#define BARRIER_H

#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>

namespace my_tt {
	template <typename F, typename... Args>
	class Barrier {
	public:
		Barrier(int n, F&& f, Args&&... args)
			:target(n), func(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
		Barrier() = delete;
		Barrier(const Barrier &) = delete;
		Barrier &operator=(const Barrier &) = delete;
		void await();
		int getParties();
		auto get_future()->std::future<std::result_of<F(Args...)>::type>;
	private:
		int target;
		volatile int nwaiting;
		std::mutex mtx;
		std::condition_variable cv;
		std::packaged_task<std::result_of<F(Args...)>::type> func;
	};
	template <typename F&&, typename... Args&&>
	int Barrier<F&&, Args&&...>::getParties() {
		return this->nwaiting;
	}
	template <typename F&&, typename... Args&&>
	auto Barrier<F&&, Args&&...>::get_future
		->std::future < std::result_of<F(Args...)>::type >
	{
		return this->func.get_future();
	}
	template <typename F&&, typename... Args&&>
	void Barrier<F&&, Args&&...>::await() {
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