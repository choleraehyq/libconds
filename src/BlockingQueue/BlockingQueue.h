#ifndef BLOCKINGQUEUE_H
#define BLOCKINGQUEUE_H

#include <queue>
#include <mutex>
#include <limits>
#include <vector>
#include <condition_variable>
#include <initializer_list>

namespace my_tt {
	template<typename items_type>
	class BlockingQueue {
	public:
		BlockingQueue() :bq(), limits(std::numeric_limits<int>::max()) {}
		BlockingQueue(const int &limits) :bq(), this->limits(limits) {}
		BlockingQueue(const int &n, items_type x);
		BlockingQueue(std::initializer_list<items_type> lst);
		BlockingQueue(const BlockingQueue &q) = delete;
		int size();
		bool empty();
		void add(items_type item);
		void remove();
		items_type back();
		items_type front();
		~BlockingQueue();
	private:
		std::queue<items_type> bq;
		std::mutex add_query_mutex, del_mutex;
		std::condition_variable add_cv, del_cv;
		int limits;
	};
	template<typename items_type>
	BlockingQueue<items_type>::BlockingQueue(const int &n, items_type x)
		:limits(std::numeric_limits<int>::max())  {
		for ( int i = 0; i < n; i++ ) {
			this->bq.push(x);
		}
	}
	template<typename items_type>
	BlockingQueue<items_type>::BlockingQueue(std::initializer_list<items_type> lst)
		:limits(std::numeric_limits<int>::max()) {
		for ( auto x : lst ) {
			this->bq.push(x);
		}
	}
	template<typename items_type>
	int BlockingQueue<items_type>::size() {
		std::unique_lock<std::mutex> lock(this->add_query_mutex);
		return bq.size();
	}
	template<typename items_type>
	bool BlockingQueue<items_type>::empty() {
		std::unique_lock<std::mutex> lock(this->add_query_mutex);
		return bq.empty();
	}
	template<typename items_type>
	items_type BlockingQueue<items_type>::back() {
		std::unique_lock<std::mutex> lock(this->add_query_mutex);
		if ( this->bq.empty() )
			throw std::runtime_error("Queue is empty.");
		return bq.back();
	}
	template<typename items_type>
	items_type BlockingQueue<items_type>::front() {
		std::unique_lock<std::mutex> lock(this->add_query_mutex);
		if ( this->bq.empty() )
			throw std::runtime_error("Queue is empty.");
		return bq.front();
	}
	template<typename items_type>
	void BlockingQueue<items_type>::add(items_type item) {
		std::unique_lock<std::mutex> lock(this->add_query_mutex);
		this->add_cv.wait(lock, [this]{ return !(this->bq.size() == this->limits); });
		this->bq.push(item);
		this->del_cv.notify_one();
	}
	template<typename items_type>
	void BlockingQueue<items_type>::remove() {
		std::unique_lock<std::mutex> lock(this->del_mutex);
		this->del_cv.wait(lock, [this]{ return !this->bq.empty(); });
		this->bq.pop();
		this->add_cv.notify_one();
	}
	template<typename items_type>
	BlockingQueue<items_type>::~BlockingQueue() {
		std::unique_lock<std::mutex> lock(this->del_mutex);
		while ( !this->bq.empty() ) {
			this->bq.pop();
		}
	}
}

#endif