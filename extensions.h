/**
 *  Extensions for CProcessing
 *  @author  : Maks Makuta
 *  @version : 1.0 beta 1
 * 
 *  Note: include AFTER CProcessing header
 */

#include <iostream>
#include <cstdio>
#include <memory>
#include <string>
#include <stdexcept>

bool in(float a, float b, float val){
    return val >= a && val <= b;
}

bool inRect(float x,float y,float w,float h,float a,float b){
    return in(x,x+w,a) && in(y,y+h,b);
}

bool inRect(float x,float y,float w,float h,float r,float a,float b){
    PVector m(a,b);
    
    PVector c1(x+r  ,y+r  );
    PVector c2(x+r  ,y+h-r);
    PVector c3(x+w-r,y+h-r);
    PVector c4(x+w-r,y+r  );

    return c1.dist(m) <= r |  c2.dist(m) <= r | c3.dist(m) <= r | c4.dist(m) <= r | inRect(x+r,y,w-2*r,h,a,b) | inRect(x,y+r,w,h-2*r,m.x,m.y);
}
bool inRect(float r1x, float r1y, float r1w, float r1h, float r2x, float r2y, float r2w, float r2h) {
    if (r1x + r1w >= r2x &&
        r1x <= r2x + r2w &&
        r1y + r1h >= r2y &&
        r1y <= r2y + r2h) {
        return true;
    }
    return false;
}

template<typename ... Args>
std::string stringf( const std::string& format, Args ... args ){
    int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    std::unique_ptr<char[]> buf( new char[ size ] );
    std::snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}
