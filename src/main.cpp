#include <iostream>
#include <thread> 

// Following is using Lamda function to create a thread and run it in the background.
void run_n_workers(int n){

    std::thread t([&n](){
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        std::cout << n << std::endl;
    });
    // t.detach();
    t.join();
}

// following method is without a lambda function, but using a normal function to create a thread and run it in the background.

int main() {
    std::cout << "Task Scheduler project scaffold" << std::endl;
    std::thread t1(run_n_workers, 15);
    t1.join();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return 0;
}
