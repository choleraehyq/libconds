#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <thread>
#include <future>
#include <mutex>
#include <functional>
#include <condition_variable>
#include <vector>
#include <queue>
#include <memory>

namespace my_tt {
	class threadPool {
	public:
		threadPool(const int& size);
		threadPool(const threadPool &t) = delete;
		threadPool &operator=(const threadPool &) = delete;
		template<typename F, typename... Args>
		auto enqueue(F &&f, Args&&... args)
			->std::future<typename std::result_of<F(Args...)>::type>;
		~threadPool();
	private:
		bool isStop;
		std::vector< std::thread > workers;
		std::queue< std::function<void()> > tasks;
		std::condition_variable cv;
		std::mutex queue_mutex;
	};
	threadPool::threadPool(const int &size): isStop(false) {
		for (int i = 0; i < size; i++) {
			workers.emplace_back(
				[this] {
					for (;;) {
						std::function<void()> task;
						{
							std::unique_lock<std::mutex> lock(this->queue_mutex);
							cv.wait(lock, 
								[this] { return this->isStop || !this->tasks.empty();} );
							if (this->isStop && this->tasks.empty())
								return;
							task = std::move(this->tasks.front());
							this->tasks.pop();
						}
						task();
					}
				}
			);
		}
	}
	template<typename F, typename... Args>
	auto threadPool::enqueue(F&& f, Args&&... args)
		->std::future< typename std::result_of<F(Args...)>::type >
	{
		using return_type = typename std::result_of<F(Args...)>::type;
		auto task = std::make_shared< std::packaged_task< return_type()> >(
			std::bind(std::forward<F>(f), std::forward<Args>(args)...));
		//std::packaged_task< return_type() > task(std::bind(f, args...));
		std::future<return_type> ret = task->get_future();
		{
			std::unique_lock<std::mutex> lock(this->queue_mutex);
			if (isStop)
				throw std::runtime_error("Enqueueing when threadpool is stopping!");
			this->tasks.emplace([task]{ (*task)(); });
		}
		cv.notify_one();
		return ret;
	}
	threadPool::~threadPool() {
		{
			std::unique_lock<std::mutex> lock(this->queue_mutex);
			this->isStop = true;
		}
		this->cv.notify_all();
		for (std::thread &worker : workers) {
			worker.join();
		}
	}
}

#endif