/* 
 * File:   main.cpp
 * Author: Administrator
 *
 */
 
#include <cstdlib>
#include <iostream>
#include <time.h>
#include "timers_container.h"
 
using namespace std;
 
int callback(timer &ptimer)
{
    time_t t = time(0); 
    struct tm * timeinfo; 
    timeinfo = localtime(&t);

    printf("[%d:%d]timer id=%d:%s\n", 
        timeinfo->tm_min, timeinfo->tm_sec,ptimer.get_id(), (char *) ptimer.get_userdata());

    if(ptimer.get_userdata() == "timer1") 
    {
        ptimer.stop();
    }
    return 0;
}
 
/*
 * 
 */
int main(int argc, char** argv)
{
    timers_container::get_instance()->init(); 
    timer timer1(1, callback, (void *) "timer1",1);
    timer timer2(2, callback, (void *) "timer2",1);
    timer1.start();
    timer2.start();

    timers_container::get_instance()->run();
    return 0;
}