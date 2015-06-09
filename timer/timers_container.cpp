/* 
 * File:   timer_poll.cpp
 * Author: Administrator
 *
 */
 
#include <cstdlib>
#include "timers_container.h"
 
using namespace std;
 
timer::timer(const timer& ptimer)
{
    timer_internal = ptimer.timer_internal;
    cb = ptimer.cb;
    timer_id = ptimer.timer_id;
    repeat = ptimer.repeat;
    userdata = ptimer.userdata;
}
 
timer & timer::operator =(const timer& ptimer)
{
    if (this == &ptimer)
    {
        return *this;
    }
 
    timer_internal = ptimer.timer_internal;
    cb = ptimer.cb;
    timer_id = ptimer.timer_id;
    repeat = ptimer.repeat;
    userdata = ptimer.userdata;
    return *this;
}
 
int timer::start()
{
    struct itimerspec ptime_internal = {0};
    ptime_internal.it_value.tv_sec = (int) timer_internal;
    ptime_internal.it_value.tv_nsec = (timer_internal - (int) timer_internal)*1000000;
    if(repeat)
    {
        ptime_internal.it_interval.tv_sec = ptime_internal.it_value.tv_sec;
        ptime_internal.it_interval.tv_nsec = ptime_internal.it_value.tv_nsec;
    }
     
    timerfd_settime(timer_id, 0, &ptime_internal, NULL);
    timers->add_timer(*this);
    return 0;
}
 
int timer::stop()
{
    timers->del_timer(*this);
    close(timer_id);
    return 0;
}
 
int timer::restart()
{
    start();
}

int timer::restart(double timer_internal)
{
    this->timer_internal = timer_internal;
    start();
}

bool timer::set_non_block (int fd)
{
    int flags = fcntl (fd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    if (-1 == fcntl (fd, F_SETFL, flags))
    {
        return false;
    }
    return true;
}

void timers_container::init(int max_num)
{
    active = 1;
    epfd = epoll_create(max_num);
    count = 0;
}

int timers_container::deactive()
{
    active = 0;
    count = 0;
}

int timers_container::add_timer(timer& ptimer)
{
    int timer_id = ptimer.get_id();
    struct epoll_event ev;
    ev.data.ptr = (void*)&ptimer;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl (epfd, EPOLL_CTL_ADD, timer_id, &ev);
    //ptimer.start();
    count++; 
    return 0;
}
 
int timers_container::del_timer(timer& ptimer)
{
    int timer_id = ptimer.get_id();
    struct epoll_event ev;
    ev.data.ptr = (void*)&ptimer;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl (epfd, EPOLL_CTL_DEL, timer_id, &ev);  
    count--;
    return 0;
}
 
void timers_container::run()
{
    for (; active && count > 0 ; )
    {
        struct epoll_event events[MAXFDS] ={0};
        int nfds = epoll_wait (epfd, events, MAXFDS, -1);
        for (int i = 0; i < nfds; ++i)
        {
            timer* ptimer = (timer*)events[i].data.ptr;
            while (read(ptimer->get_id(), read_buf, 8) > 0)
            {
                ptimer->get_user_callback()(*ptimer);
            }
            memset(read_buf, 0, 8);
        }
    }
}
