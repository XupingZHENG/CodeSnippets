#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

std::mutex mtxA, mtxB, mtxAB;
std::condition_variable condA, condB, condAB;

void printA()
{
    while (true)
    {
        {
            std::unique_lock<std::mutex> lg(mtxA);
            condA.wait(lg);
            printf("a");
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        condB.notify_one();
    }
}

void printB()
{
    while (true)
    {
        {
            std::unique_lock<std::mutex> lg(mtxB);
            condB.wait(lg);
            printf("b");
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        condAB.notify_one();
    }
}

void printAB()
{
    while (true)
    {
        {
            std::unique_lock<std::mutex> lg(mtxAB);
            condAB.wait(lg);
            printf("ab");
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        condA.notify_one();
    }
}

int main()
{
    std::thread t1(printA);
    std::thread t2(printB);
    std::thread t3(printAB);

    condA.notify_one();

    t1.join();
    t2.join();
    t3.join();

    return 0;
}