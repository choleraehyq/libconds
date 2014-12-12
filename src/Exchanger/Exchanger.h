#ifndef EXCHANGER_H
#define EXCHANGER_H

#include <mutex>
#include <condition_variable>
#include <memory>

namespace my_tt {
	template<typename T>
	class Exchanger {
	public:
		Exchanger() :hasExchanged(true), temp1(nullptr), temp2(nullptr) {}
		//hasExchanged will be spined first in Exchanger::exchange
		T exchange(T& swapItem);
	private:
		bool hasExchanged;
		std::shared_ptr<T> temp1, temp2;
		std::mutex mtx;
		std::condition_variable cv;
	};
	template<typename T>
	T Exchange<T>::exchange(T& swapItem) {
		std::unique_lock<std::mutex> lock(this->mtx);
		this->hasExchanged = !this->hasExchanged;
		if ( this->hasExchanged ) {
			this->temp1 = std::make_shared<T>(swapItem);
			this->cv.wait(lock, [this] { return !this->hasExchanged; });
		}
		else {
			this->temp2 = std::make_shared<T>(swapItem);
			//this->cv.notify_one();
			//Please review the lock and condition_variable first tomorrow.
		}
	}
}
#endif