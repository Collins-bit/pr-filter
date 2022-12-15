#include <util/time_util.h>

struct timeval TimeUtil::initTime()
{
    gettimeofday(&this->start, NULL);
    return this->start;
}

void TimeUtil::endTime()
{
    struct timeval end;
    gettimeofday(&end, NULL);
    cout << "Time cost: " << ((end.tv_sec - this->start.tv_sec) * 1000000.0 + end.tv_usec - this->start.tv_usec) / 1000.0 << " ms" << endl;
}

void TimeUtil::endTime(string msg)
{
    struct timeval end;
    gettimeofday(&end, NULL);
    cout << msg << ", time cost: " << ((end.tv_sec - this->start.tv_sec) * 1000000.0 + end.tv_usec - this->start.tv_usec) / 1000.0 << " ms" << endl;
}

void TimeUtil::endTime(struct timeval start)
{
    struct timeval end;
    gettimeofday(&end, NULL);
    cout << "Time cost: " << ((end.tv_sec - start.tv_sec) * 1000000.0 + end.tv_usec - start.tv_usec) / 1000.0 << " ms" << endl;
}

void TimeUtil::endTime(struct timeval start, string msg)
{
    struct timeval end;
    gettimeofday(&end, NULL);
    cout << msg << ", time cost: " << ((end.tv_sec - start.tv_sec) * 1000000.0 + end.tv_usec - start.tv_usec) / 1000.0 << " ms" << endl;
}