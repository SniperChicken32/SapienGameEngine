//
// General purpose timer class

/** General purpose timer abstraction.*/
class Timer {
    
    double TimeFrequency;
    LARGE_INTEGER tLast;
    
    public:
    
    Timer() {
        
        LARGE_INTEGER tFrequency;
        
        // Get precise frequency
        QueryPerformanceFrequency(&tFrequency);
        TimeFrequency = tFrequency.QuadPart / 1000.0;
        
        // Initiate timer
        QueryPerformanceCounter(&tLast);
    }
    ~Timer() {
        
    }
    
    /** Checks the current time.*/
    double Check (bool Reset=false)  {
        LARGE_INTEGER tCurrent;
        
        QueryPerformanceCounter(&tCurrent);
        
        double Delta = (tCurrent.QuadPart - tLast.QuadPart) / TimeFrequency;
        
        if (Reset) {tLast = tCurrent;}
        return Delta;
    }
    
    /** Resets the timer.*/
    void Reset (void) {QueryPerformanceCounter(&tLast);}
    
};

/** Frame timer structure.*/
struct FrameTimer {
    
    double UpdateRate;
    double UpdateRateMax;
    double UpdateRateMin;
    double UpdateRateMs;
    double UpdateRateSec;
    
    double TimeDelta;
    double TimeAlpha;
    double TimeAccumulator;
    double UnitsPerSecond;
    double UpdateComplete;
    double UpdateRemainder;
    
};

