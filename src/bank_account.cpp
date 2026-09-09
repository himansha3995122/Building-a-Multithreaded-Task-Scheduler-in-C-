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

        std::mutex& get_mutex(){ 
            return mutex_;
        }
        void raw_withdraw(int amount){
            balance_ -= amount;
        }
        void raw_deposit(int amount){
            balance_ += amount;
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

void transfer(BankAccount& from, BankAccount& to, int amount){
    // std::lock_guard<std::mutex> lock1(from.get_mutex());

    // std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // std::lock_guard<std::mutex> lock2(to.get_mutex());
    std::scoped_lock lock(from.get_mutex(), to.get_mutex());
    from.raw_withdraw(amount);
    to.raw_deposit(amount);
}

int main(){

    std::cout << "Shared Bank Account demo" << std::endl;

    // Calling Bank Accountobject with constructor
    BankAccount account(0);
    BankAccount account2(0);
    const int num_threads = 8;
    const int num_deposits = 100000;
    const int transfer_iterations = 10000;

    std::vector<std::thread> threads;
    for(int i = 0; i < num_threads; i++){
        threads.push_back(std::thread(deposit_many, std::ref(account), num_deposits));
    }

    // std::thread transfer_thread(transfer, std::ref(account), std::ref(account2), 100000);
    for(int  i = 0; i < transfer_iterations;i++){
        std::thread transfer_thread_1(transfer, std::ref(account), std::ref(account2), 20);
        std::thread transfer_thread_2(transfer, std::ref(account2), std::ref(account), 5);
        transfer_thread_1.join();
        transfer_thread_2.join();
    }

    for(auto& t: threads){
        t.join();
    }
    // transfer_thread.join();

    account.print_balance();
    std::cout << "Transferring 100000 from account to account2" << std::endl;
    account2.print_balance();
    return 0;
}
