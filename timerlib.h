#ifndef TIMERLIB_H
#define TIMERLIB_H

class TIMER
{
    private:
      long long StartTime;
      
    public:
      TIMER();
      ~TIMER();
      
      long long GetCount();
};

#endif
