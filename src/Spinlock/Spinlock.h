#ifndef SPINLOCK_H
#define SPINLOCK_H
#include <atomic>

namespace my_tt {
	class Spinlock {
	public:
		Spinlock() :flag(ATOMIC_FLAG_INIT);
		//Spinlock() :flag(ATOMIC_FLAG_INIT) {}
		Spinlock(const Spinlock &) = delete;
		Spinlock &operator=(const Spinlock &) = delete;
		void lock();
		void unlock();
		~Spinlock() = default;
	private:
		std::atomic_flag flag;
	};
	void Spinlock::lock() {
		while ( this->flag.test_and_set(std::memory_order_acquire) )
			;
	}
	void Spinlock::unlock() {
		this->flag.clear(std::memory_order_release);
	}
}

#endif