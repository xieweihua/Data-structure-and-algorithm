/* 
 * File:   timer_poll.h
 * Author: Administrator
 *
 */
 
#ifndef TIMER_CONTAINER_H
#define TIMER_CONTAINER_H
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/epoll.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/timerfd.h>
#include <unistd.h>
#include <pthread.h>
#include <map>
 
#define MAXFDS 128
#define EVENTS 100
class timer;
typedef int(*timer_callback)(timer &);//user callback
 
class timers_container
{
private:
    timers_container(){}
    timers_container(timers_container &){}
    timers_container & operator = (timers_container &){}

public:
    void init(int max_num=128);
    void run();
    int deactive();
    int add_timer(timer &ptimer);
    int del_timer(timer &ptimer);

    static timers_container* get_instance()
    {
        static timers_container timers;
        return &timers;
    }

    ~ timers_container(){}
private:
    int active;
    int count;
    int epfd;
    char read_buf[8];
} ;

class timer
{
public:
 
    timer() : timer_internal(0.0), cb(0), timer_id(0), repeat(0), userdata(0){}
    timer(double internal_value, int  (*callback)(timer &ptimer), void *data, int rep) : 
        timer_internal(internal_value), cb(callback), userdata(data), repeat(rep)
    {
        timer_id = timerfd_create(CLOCK_REALTIME, 0);
        set_non_block(timer_id);
        timers = timers_container::get_instance();
    }
 
    timer(const timer &ptimer);
    timer & operator=(const timer &ptimer);
    int start();
    int stop();
    int restart();
    int restart(double timer_internal);
 
    int get_id(){return timer_id;}
    void *get_userdata(){return userdata;}
    timer_callback get_user_callback(){return cb;}
 
    ~timer()
    {
        stop();
    }
 
private:
    bool set_non_block (int fd);
private:
    int     timer_id;
    double  timer_internal;
    void    *userdata;
    bool    repeat;
    timer_callback cb;
    timers_container* timers;
} ;
 
#endif  /* TIMER_POLL_H */