// g++ -std=gnu++11 channel.cpp test_channel.cpp -o test_channel
// ./test_channel 10000 <pid>
#include "channel.h"

int main(int argc, char* argv[])
{
    unsigned long period;
    pid_t pid;
    if(argc != 3 ||
        sscanf(argv[1], "%lu", &period) != 1 ||
        sscanf(argv[2], "%d", &pid) != 1)
    {
        printf("USAGE: %s <period> <pid>\n", argv[0]);
        return 1;
    }
    Channel c;
    int ret = c.bind(pid, Channel::CHANNEL_STORE);
    if(ret)
        return ret;
    ret = c.setPeriod(period);
    if(ret)
        return ret;
    while(true)
    {
        Channel::Sample sample;
        ret = c.readSample(&sample);
        if(ret == -EAGAIN)
        {
            usleep(10000);
            continue;
        }
        else if(ret < 0)
            return ret;
        else
            printf("type: %x, cpu: %u, pid: %u, tid: %u, address: %lx\n",
                sample.type, sample.cpu, sample.pid, sample.tid, sample.address);
    }
    return 0;
}