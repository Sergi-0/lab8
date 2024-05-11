#include <iostream>
using namespace std;

template<class T>
class MyShared {
    T* p = nullptr;
    

public:
    int* n = nullptr;// Счетчик ссылок

    MyShared(T* p = nullptr) : p(p), n(new int(1)) {}// Конструктор

    
    ~MyShared() {// Деструктор
        if ((*n)-1 == 0) {
            delete p;
            delete n;
        }
    }

    
    MyShared(const MyShared& t) : p(t.p), n(t.n) {// Конструктор копирования
        (*n)++;
    }

    
    MyShared& operator=(const MyShared& t) {// Оператор присваивания копированием
        if (this != &t) {
            if ((*n)-1 == 0) {
                delete p;
                delete n;
            }
            p = t.p;
            n = t.n;
            (*n)++;
        }
        return *this;
    }

    
    MyShared(MyShared&& t) : p(t.p), n(t.n) {// Конструктор перемещения
        t.p = nullptr;
        t.n = new int(0); 
    }

    
    MyShared& operator=(MyShared&& t) {// Оператор присваивания перемещением
        if (this != &t) {
            if ((*n)-1 == 0) {
                delete p;
                delete n;
            }
            p = t.p;
            n = t.n;
            t.p = nullptr;
            t.n = new int(0); 
        }
        return *this;
    }

    
    T* get() const { return p; }

    
    T& operator*() { return *p; }

   
    T* operator->() { return p; } 
};

template<typename T, typename... Args>
MyShared<T> Make_MyShared(Args&&... args) {
    return MyShared<T>(new T(forward<Args>(args)...));
}

class MyPoint {
public:
    int x, y;
    MyPoint(int x, int y) : x(x), y(y) {}
    void vivod() const { 
        cout << "Point(" << x << ", " << y << ")" << endl; 
    }
};

int main() {
    auto PtSr = Make_MyShared<MyPoint>(10, 20);
    PtSr->vivod();
    MyShared<MyPoint> PtSr2 = PtSr;
    PtSr2->vivod(); 
    cout << *PtSr2.n << endl;
    cout << *PtSr.n;
}