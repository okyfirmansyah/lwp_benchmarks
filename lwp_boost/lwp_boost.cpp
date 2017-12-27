#include <iostream>
#include <functional>
#include <boost/fiber/all.hpp>
#include <boost/thread.hpp>
#include <boost/thread/barrier.hpp>
#include <boost/bind.hpp>
#include <boost/atomic.hpp>
#include <chrono>
#include <thread>
#include <boost/asio.hpp>
#include <list>

using namespace std;

static boost::fibers::mutex mtx_count{};
static boost::fibers::condition_variable cnd_count{};
typedef std::unique_lock< boost::fibers::mutex > lock_t;

void worker_thread(boost::barrier* b, int N)
{
    boost::fibers::use_scheduling_algorithm< boost::fibers::algo::shared_work >();
    b->wait();

    while(1)
    {
        boost::this_fiber::yield();
        usleep(100);
        //boost::this_fiber::sleep_for(chrono::microseconds(100));

        /*if(boost::fibers::has_ready_fibers())
            boost::this_fiber::yield();
        else
            usleep(100);*/
    }
    lock_t lk(mtx_count);
    cnd_count.wait(lk, [](){
        return false;});
}


int main()
{
    const int N=thread::hardware_concurrency();
    boost::barrier bar(N);

    std:list<std::thread> thread_list;
    for(int i=0;i<N;i++)
        thread_list.emplace_back(worker_thread, &bar, N);// std::thread(worker_thread, &bar, N);
    bar.wait();

    boost::fibers::use_scheduling_algorithm< boost::fibers::algo::shared_work >();
    int i=5;
    function<void(void)> fn= [j=i](){
        cout <<"hehe "<<j<<"\n";
    };



    boost::fibers::fiber fb([](void){
        while(1){
            cout<<"this is called from fiber\n";
            boost::this_fiber::sleep_for(chrono::seconds(1));
        }
    });



    fn();

    for(int i=0;i<100000/*5000000*/;i++)
    {
        boost::fibers::fiber fb2([=](void){
            while(1){
                cout<<"fiber id="<<i<<" called from thread id="<<std::this_thread::get_id()<<"\n";
                boost::this_fiber::sleep_for(chrono::seconds(rand()%500000));
            }
        });
        fb2.detach();
    }

    while(1){
        boost::this_fiber::yield();
        usleep(100);
        //boost::this_fiber::sleep_for(chrono::microseconds(100));

        /*if(boost::fibers::has_ready_fibers())
            boost::this_fiber::yield();
        else
            usleep(100);*/
    }
    lock_t lk(mtx_count);
    cnd_count.wait(lk, [](){
        return false;});
    return 0;
}
