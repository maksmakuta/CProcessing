#ifndef ARRAYLIST_H
#define ARRAYLIST_H
/**
 * @brief The ArrayList class
 */ beta 1.0
template<class T>
class ArrayList{
private:
    T* arr = nullptr;
    int _size,_length;
public:
    ArrayList() : ArrayList(1){

    }

    ArrayList(int s){
        this->arr = new T[s];
        this->_size = s;
        this->_length = 0;
    }

    ArrayList(T* array, int arrSize){
        this->arr = array;
        this->_size = arrSize;
        this->_length = 0;
    }

    void add(T elem){
        arr[_length] = elem;
        _length++;
    }

    void add(int index, T elem){
        T *tmp = new T[_size+1];
        for(int a = 0;a < _size+1;a++){
            if(a < index){
                tmp[a] = arr[a];
            }else if(a == index){
                tmp[a] = elem;
            }else{
                tmp[a] = arr[a-1];
            }
        }
        this->arr = tmp;
        this->_length = 0;
        this->_size++;
    }

    void addAll(T* array,int s){
        T* tmp = new T[_size + s];
        for(int a = 0;a < _size + s;a++){
            if(a<= _size){
                tmp[a] = arr[a];
            }else{
                tmp[a] = array[a - _size];
            }
        }
        arr = tmp;
    }
    void addAll(int p,T* array,int s){

    }

    void clear(){
        for(int a = 0;a < this->_size();a++){
            arr[a] = T();
        }
    }

    ArrayList clone(){
        return this;
    }

    bool contains(T obj){
        for(T o : arr){
            if(o == obj)
                return true;
        }
        return false;
    }
    void ensureCapacity(int s){
        this->_length += s;
    }
    T get(int pos){
        return arr[pos];
    }
    int indexOf(T obj){
        for(int a = 0;a < _size;a++){
            if(arr[a] == obj)
                return a;
        }
        return -1;
    }

    bool isEmpty(){
        if(_size == 0) return true;
        return false;
    }

    int lastIndexOf(T obj){
        int p = -1;
        for(int a = 0;a < _size;a++){
            if(arr[a] == obj)
                p = a;
        }
        return p;
    }

    T remove(int p){
        T item;
        T* tmp = new T[_size - 1];
        for(int a = 0;a < _size;a++){
            if(a == p){
                item = arr[a];
            }else{
                if(a > p)
                    tmp[a] = arr[a-1];
                else
                    tmp[a] = arr[a];
            }
        }
        arr = tmp;
        return item;
    }

    bool remove(T obj){
        bool x = false;
        T* tmp = new T[_size - 1];
        for(int a = 0;a < _size;a++){
            if(arr[a] == obj){
                x = true;
            }else{
                if(a > p)
                    tmp[a] = arr[a-1];
                else
                    tmp[a] = arr[a];
            }
        }
        arr = tmp;
        return x;
    }

    void removeRange(int start, int end){
        bool x = false;
        T* tmp = new T[_size - 1];
        for(int a = 0;a < _size;a++){
            if(a >= start && a < end){
                x = true;
            }else{
                if(a > p)
                    tmp[a] = arr[a-1];
                else
                    tmp[a] = arr[a];
            }
        }
        arr = tmp;
        return x;
    }

    T set(int p, T obj){
        if(p >= 0 && p < _size){
            T item = arr[p];
            arr[p] = obj;
            return item;x
        }else{
            return obj;
        }
    }

    int size(){return this->_size;}

    T* subList(int start, int end,int &subSize){
        int s = end - start;
        T *sub = new T[s];
        for(int a = start; a < end;a++){
            sub[a - start] = arr[a];
        }
        subSize = s;
        return sub;
    }
};

#endif
