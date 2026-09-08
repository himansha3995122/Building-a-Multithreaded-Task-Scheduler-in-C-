#include <iostream>
#include <thread> 


class ScopeThread{
    public:
        explicit ScopeThread(std::thread t) : thread_(std::move(t)){
            if(!thread_.joinable()){
                throw std::logic_error("ScopeThread requires a joinable thread");
            }
        }

        ~ScopeThread(){
            if(thread_.joinable()){
                thread_.join();
            }
        }

        // delete copy constructor and copy assignment operator

        ScopeThread(const ScopeThread&) = delete;
        ScopeThread& operator = (const ScopeThread&) = delete;
        ScopeThread(ScopeThread&&) = default;
        ScopeThread& operator = (ScopeThread&&) = default;

    private:
        std::thread thread_;
};

// Following is using Lamda function to create a thread and run it in the background.
void run_n_workers(int n){

    // std::thread t([&n](){
    //     std::this_thread::sleep_for(std::chrono::milliseconds(200));
    //     std::cout << n << std::endl;
    // });
    // // t.detach();
    // t.join();

    ScopeThread t(std::thread([&n](){
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        std::cout << n << std::endl;
    }));
}

// following method is without a lambda function, but using a normal function to create a thread and run it in the background.

int main() {
    std::cout << "Task Scheduler project scaffold" << std::endl;
    std::thread t1(run_n_workers, 15);
    t1.join();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return 0;
}
