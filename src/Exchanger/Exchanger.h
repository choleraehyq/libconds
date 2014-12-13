#ifndef EXCHANGER_H
#define EXCHANGER_H

#include <mutex>
#include <condition_variable>
#include <memory>

namespace my_tt {
	template<typename T>
	class Exchanger {
	public:
		Exchanger() :hasExchanged(true) {}
		Exchanger(const Exchanger<T> &) = delete;
		//hasExchanged will be spined first in Exchanger::exchange
		T exchange(const T& swapItem);
	private:
		bool hasExchanged;
		T temp1, temp2;
		std::mutex mtx;
		std::condition_variable cv;
	};
	template<typename T>
	T Exchanger<T>::exchange(const T& swapItem) {
		std::unique_lock<std::mutex> lock(this->mtx);
		T ret;
		this->hasExchanged = !this->hasExchanged;
		if ( !this->hasExchanged ) {
			this->temp1 = std::move(swapItem);
			this->cv.wait(lock, [this] { return this->hasExchanged; });
			ret = std::move(temp2);
		}
		else {
			this->temp2 = std::move(swapItem);
			this->cv.notify_one();
			ret = std::move(temp1);
		}
		return ret;
	}
}
#endif