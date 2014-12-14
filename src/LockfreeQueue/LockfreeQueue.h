#ifndef LOCKFREEQUEUE_H
#define LOCKFREEQUEUE_H

#include <atomic>
#include <stdexcept>

namespace my_tt {
	template<typename T>
	using std::atomic_compare_exchange_weak<T>;
	template<typename T>
	using node = struct {
		T data;
		node *next;
	};
	template<typename T>
	class LockfreeQueue {
	public:
		LockfreeQueue();
		LockfreeQueue(const LockfreeQueue &) = delete;
		LockfreeQueue &operator=(const LockfreeQueue &) = delete;
		void enqueue(const T &x);
		T dequeue();
		int size();
		bool empty();
		~LockfreeQueue();
	private:
		volatile std::atomic<node<T>> *head, *tail;
		std::atomic<int> len;
	};
	template<typename T>
	LockfreeQueue<T>::LockfreeQueue(): head(new node<T>), tail(nullptr) {
		head->next = nullptr;
	}
	template<typename T> 
	int LockfreeQueue<T>::size() {
		return this->len;
	}
	template<typename T>
	bool LockfreeQueue<T>::empty() {
		return !this->len;
	}
	template<typename T>
	LockfreeQueue::~LockfreeQueue() {
		auto *cur = head, *p = cur;
		while ( cur != nullptr ) {
			free(p);
			cur = cur->next;
		}
	}
	template<typename T>
	void LockfreeQueue<T>::enqueue(const T &x) {
		auto *q = new std::atomic<node<T>>;
		q->data = x;
		q->next = nullptr;
		auto *p = tail;
		do {
			p = tail;
		} while ( !atomic_compare_exchange_weak<node<T>>(p->next, nullptr, q) );
		atomic_compare_exchange_weak<node<T>>(tail, p, q);
	}
	template<typename T>
	T LockfreeQueue<T>::dequeue() {
		if ( head->next == nullptr )
			throw std::runtime_error("Queue is empty");
		auto *p = head;
		auto *oldp = p;
		while ( !atomic_compare_exchange_weak<node<T>>(head, p, p->next) );
		delete oldp;
		return p->next->value;
	}
}
#endif