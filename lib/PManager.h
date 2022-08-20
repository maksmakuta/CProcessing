#ifndef PMANAGER_H
#define PMANAGER_H

#include <vector>
#include <utility>

template<typename T> class Manager{
private:
    std::vector<T> images;
public:

    void add(T img){
        images.push_back(img);
    }

    T get(int id){
        return images[id];
    }

    T getLast(){
        return get(images.size() - 1);
    }

    void load(){
        for(T &img : images){
            img.load();
        }
    }

    void done(){
        images.clear();
    }
};

#endif // PMANAGER_H
