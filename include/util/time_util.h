#ifndef TIME_UTIL
#define TIME_UTIL

#include <iostream>
#include <sys/time.h>
using namespace std;

class TimeUtil
{
private:
    struct timeval start;
public:
    TimeUtil(){}
    struct timeval initTime();
    void endTime();
    void endTime(string msg);
    void endTime(struct timeval start);
    void endTime(struct timeval start, string msg);
};

#endif // TIME_UTIL