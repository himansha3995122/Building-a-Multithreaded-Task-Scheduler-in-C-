// This is a jthread demo presentation
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

std::mutex mutex_;



int main(){
    {
    std::lock_guard<std::mutex> Lock(mutex_);
    std::cout << "jthread demo" << std::endl;

    }
    std::jthread t([](std::stop_token st){
        int count = 0;
        while(!st.stop_requested()){
            std::lock_guard<std::mutex> Lock(mutex_);
            std::cout << "count:" << count << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            count++;
    }
        std::lock_guard<std::mutex> Lock(mutex_);
        std::cout << "stop requested, exiting thread" << std::endl;
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(1200));
    {
        std::lock_guard<std::mutex> Lock(mutex_);
        std::cout << "Letting go out of the scope" << std::endl;
    }

    return 0;
}