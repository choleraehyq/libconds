#ifndef READWRITELOCK_H
#define READWRITELOCK_H
#include <mutex>
#include <condition_variable>

namespace my_tt {
    class ReadWriteLock {
    public:
        ReadWriteLock():curread(0), curwrite(false) {}
        ReadWriteLock(const ReadWriteLock &) = delete;
        ReadWriteLock &operator=(const ReadWriteLock &) = delete;
        void get_read_lock();
        void release_read_lock();
        void get_write_lock();
        void release_write_lock();
        ~ReadWriteLock() = default;
    private:
        std::mutex rlock, wlock;
        std::condition_variable rwcond;
        int curread;
        bool curwrite;
    };
    void ReadWriteLock::get_read_lock() {
        std::unique_lock<std::mutex> lock(this->rlock);
        rwcond.wait(lock, [this] { return !this->curwrite; });
        curread++;
    }
    void ReadWriteLock::release_read_lock() {
        std::unique_lock<std::mutex> lock(this->rlock);
        curread--;
        if (curread == 0)
            rwcond.notify_one();
    }
    void ReadWriteLock::get_write_lock() {
        std::unique_lock<std::mutex> lock(this->wlock);
        rwcond.wait(lock, [this] { return this->curread == 0 && !curwrite; });
        curwrite = true;
    }
    void ReadWriteLock::release_write_lock() {
        std::unique_lock<std::mutex> lock(this->wlock);
        curwrite = false;
        rwcond.notify_one();
    }
}

#endif // READWRITELOCK_H
