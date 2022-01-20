#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <initializer_list>

template <typename T> class ArrayList{
private:
    T* arr;
    int mSize,mCapacity;
public:
    ArrayList() : ArrayList(1){}
    ArrayList(int capacity){
        this->arr = new T[capacity];
        this->mSize = 0;
        this->mCapacity = capacity;
    }

    ArrayList(const std::initializer_list<T>& l)
     : ArrayList(l.size()){
        this->addAll(l);
    }

    void add(T element){
        this->arr[mSize] = element;
        this->mSize++;
    }

    void add(int i,T e){
        if(mCapacity > mSize){
            T tmp;
            for(int a = i;a < size() + 1;a++){
                tmp = get(a);
                if(a == i)
                    set(a,e);
                else
                    set(a,tmp);
            }
            this->mSize++;
        }
    }

    void addAll(const std::initializer_list<T>& l){
        this->addAll(0,l);
    }

    void addAll(int i,const std::initializer_list<T>& l){
        if(i + l.size() > mCapacity){
            T *tmp = new T[i + l.size()];
            for(int a = 0;a < mSize;a++){
                tmp[a] = arr[a];
            }
            arr = tmp;
            mSize = i + l.size();
            mCapacity = mSize;
        }
        for(int a = 0;a < l.size();a++){
            arr[i+a] = l[a];
        }
    }


    void clear(){
        delete [] arr;
        this->arr = new T[mCapacity];
    }
    ArrayList clone(){return *this;}

    bool contains(T element){
        for(int a = 0;a < mSize;a++){
            if(arr[a] == element)
                return true;
        }
        return false;
    }

    void ensureCapacity(int minCapacity){
        this->mCapacity = minCapacity;
    }

    T get(int pos){
        if(pos < 0 || pos > mCapacity)
            return 0;
        else{
            return arr[pos];
        }
    }

    int indexOf(T o){
        for(int a = 0; a < mSize;a++)
            if(arr[a] == o) return a;
        return -1;
    }

    bool isEmpty(){
        return mSize == -1;
    }

    T remove(int index){
        T tmp = arr[index];
        arr[index] = (T)0;
        return tmp;
    }

    bool remove(T o){
        int p = indexOf(o);
        if(p != -1){
            remove(p);
            return true;
        }
        return false;
    }

    bool removeAll(const std::initializer_list<T>& c){
        bool res = true;
        ArrayList<T> tmp(c);
        for(int a = 0;a < tmp.size();a++){
            res |= remove(tmp.get(a));
        }
        return res;
    }

    T set(int index, T element){
        if(index < 0 || index > mCapacity){
            return (T)0;
        }else{
            T tmp = arr[index];
            arr[index] = element;
            return tmp;
        }
    }

    int size(){return this->mSize;}

    ArrayList subList(int fromIndex, int toIndex){
        ArrayList<T> tmp;
        for(int a = fromIndex;a < toIndex;a++)
            tmp.add(this->get(a));
        return tmp;
    }

    T* toArray(){return this-arr;}

    void trimToSize(){
        this->mCapacity = mSize;
    }

};

#endif
