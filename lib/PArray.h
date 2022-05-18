#ifndef PARRAY_H
#define PARRAY_H

#include <vector>

template<class T> std::vector<T> append(const std::vector<T> arr,T obj){
    arr.push_back(obj);
    return arr;
}
template<class T> void arrayCopy(const std::vector<T>& src,int srcPosition,
                                 const std::vector<T>& dst,int dstPosition,int length)	{

    std::copy(src.begin()+srcPosition,src.begin()+length,dst.begin() + dstPosition);

}
template<class T> void arrayCopy( const std::vector<T>&  src,const std::vector<T>&  dst, int length){
    arrayCopy(src,0,dst,0,length);
}
template<class T> void arrayCopy(const std::vector<T>& src, const std::vector<T>&  dst){
    arrayCopy(src,dst,src.size());
}
template<class T> std::vector<T> concat(const std::vector<T>& src, const std::vector<T>&  dst){
    src.insert(src.end(),dst.begin(),dst.end());
    return src;
}
template<class T> std::vector<T> expand(const std::vector<T>& list,int newSize = -1){
    int s = (int)(newSize == -1 ? list.size() * 2 : newSize);
    std::vector<T> tmp(s);
    std::copy(list.begin(),list.end(),tmp.begin());
    return tmp;
}
template<class T> std::vector<T> reverse(const std::vector<T>& list){
    std::vector<T> tmp(list.size());
    std::copy(list.end(),list.begin(),tmp.begin());
    return tmp;
}
template<class T> std::vector<T> shorten(const std::vector<T>& list){
    std::vector<T> tmp(list.size() - 1);
    std::copy(list.begin(),list.end()-1,tmp.begin());
    return tmp;
}
template<class T> std::vector<T> sort(const std::vector<T>& list,int count = 0){
    qsort(list,count == 0 ? list.size() : count,sizeof(T),
          [](const void * a, const void * b){
            return ( *(T*)a - *(T*)b );
          }
    );
    return list;
}
template<class T> std::vector<T> splice(const std::vector<T>& list,const std::vector<T>& val,int index){
    list.insert(list.begin() + index,val.begin(),val.end());
    return list;
}
template<class T> std::vector<T> splice(const std::vector<T>& list,T val,int index){
    list.insert(list.begin() + index,val);
    return list;
}
template<class T> std::vector<T> subset(const std::vector<T>& list,int start,int count = 0){
    return std::vector<T>(list.begin() + start,count == 0 ? list.end() : list.begin()+ start + count);
}

#endif // PARRAY_H
