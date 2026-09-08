#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <mutex>


class BankAccount{

    public:
        explicit BankAccount(int balance) : balance_(balance) {}


        void deposit(int amount){
            std::lock_guard<std::mutex> lock(mutex_);
            balance_ += amount;
        }

        void withdraw(int amount){
            std::lock_guard<std::mutex> lock(mutex_);
            balance_ -= amount;
        }

        void print_balance(){
            std::lock_guard<std::mutex> lock(mutex_);
            std::cout << "Current balance: " << balance_ << std::endl;
        }

    private:
        int balance_;
        std::mutex mutex_;

};

void deposit_many(BankAccount& account, int times){
    for(int i = 0; i < times; i++){
        account.deposit(1);
    }
}

int main(){

    std::cout << "Shared Bank Account demo" << std::endl;

    // Calling Bank Accountobject with constructor
    BankAccount account(0);
    const int num_threads = 8;
    const int num_deposits = 100000;

    std::vector<std::thread> threads;
    for(int i = 0; i < num_threads; i++){
        threads.push_back(std::thread(deposit_many, std::ref(account), num_deposits));
    }

    for(auto& t: threads){
        t.join();
    }

    account.print_balance();
    return 0;
}
