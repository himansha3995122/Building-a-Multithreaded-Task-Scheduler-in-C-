#include <iostream>
#include <mutex>
#include <thread>
#include <queue>
#include <condition_variable>


template<typename T>
class BlockingQueue{
    public:
        explicit BlockingQueue(size_t capacity) : capacity_(capacity){}

        void push(T value){
            std::unique_lock<std::mutex> lock(mutex_);
            not_full_cv.wait(lock, [&]{
                return queue_.size() < capacity_;
            });
            queue_.push(std::move(value));

            lock.unlock();
            not_empty_cv.notify_one();
        }

        T pop(){
            std::unique_lock<std::mutex> lock(mutex_);
            not_empty_cv.wait(lock, [&]{
                return !queue_.empty();
            });

            T value = std::move(queue_.front());
            queue_.pop();
            lock.unlock();
            not_full_cv.notify_one();

            return value;
        }


    private:
        size_t capacity_;
        std::mutex mutex_;
        std::condition_variable not_empty_cv;
        std::condition_variable not_full_cv;
        std::queue<T> queue_;
};


int main(){

    BlockingQueue<int> q(5); // intializing capacity

    // Producer
    std::thread producer([&]{
        for(int i = 0; i < 20; i++){
            q.push(i);
            std::cout << "Pushed to the queue" << std::endl;
        }
    });

    std::thread consumer([&]{
        for(int i = 0; i < 20; i++){
            int v = q.pop();
            std::cout << "poped from the queue" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    });
    producer.join();
    consumer.join();

    return 0;
}