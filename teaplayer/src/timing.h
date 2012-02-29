#ifndef _TIMING_H_
#define _TIMING_H_

#ifdef LINUX
typedef uint64_t MediaTime;

const MediaTime BAD_TIME = (uint64_t)(-1);
inline MediaTime currentTime() {
   return 0;
}

#elif WIN32
    
#endif


#endif
