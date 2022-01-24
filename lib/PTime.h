#ifndef PTIME_H
#define PTIME_H

#include <ctime>

tm getTime(){
   time_t tt;
   time( &tt );
   return (tm)*localtime( &tt );
}

// Returns the current day as a value from 1 - 31
int day(){
    return getTime().tm_mday;
}

// Returns the current hour as a value from 0 - 23
int hour(){
    return getTime().tm_hour;
}

// Returns the current minute as a value from 0 - 59
int minute(){
    return getTime().tm_min;
}

// Returns the current month as a value from 1 - 12
int month(){
    return getTime().tm_mon;
}

// Returns the current second as a value from 0 - 59
int second(){
    return getTime().tm_sec;
}

// Returns the current year as an integer (2003, 2004, 2005, etc)
int year(){
    return getTime().tm_year + 1900;
}

#include <GLFW/glfw3.h>

// Returns the number of milliseconds (thousandths of a second) since starting an applet
int millis(){
    return (int)(glfwGetTime() * 1000);
}

#endif
