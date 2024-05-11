#include <iostream>
#include <utility>
using namespace std;

template<class T>
class MyUnique
{
    T* p = nullptr;
public:
    MyUnique(T* p) : p(p) {}; // конструктор

    MyUnique(MyUnique&& t) { // конструктор перемещения
        p = t.p; 
        t.p = nullptr;
    } 

    MyUnique(const MyUnique&) = delete; // конструктор копирования (запрещен)

    ~MyUnique() { // деструктор
        delete p;
    }

    T* get() const {
        return p;
    }

    T& operator*() { // перегрузка *
        return *p;
    }

    T* operator->() { // перегрузка ->, хоть код такой же как и в get(), работает это подругому
        return p;
    }

    MyUnique& operator=(const MyUnique&) = delete; // оператор присваивания копированием (запрещен)

    MyUnique& operator=(MyUnique&& t) { // оператор перемещения
        p = t.p;
        t.p = nullptr;
        return *this;
    }

};

template<typename T, typename... Args>
MyUnique<T> Make_MyUnique(Args&&... args) {
    return MyUnique<T>(new T(forward<Args>(args)...));
}

class MyPoint { // класс точка для примера
public:
    int x, y;

    MyPoint(int x, int y) : x(x), y(y) {}

    void vivod() const { 
        cout << "Point(" << x << ", " << y << ")" << endl; 
    }
};

int main() {
    auto PtUq = Make_MyUnique<MyPoint>(10, 20);
    PtUq->vivod(); 
    MyUnique<MyPoint> mvPt = move(PtUq);
    mvPt->vivod(); 
}