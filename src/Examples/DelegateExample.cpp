#include <iostream>
#include <vector>
#include <functional>

struct Delegate {
    std::vector<std::function<void(int)>> funcs;
    //template<class T> Delegate& operator+=(T mFunc) { funcs.push_back(mFunc); return *this; }
    Delegate& operator+=(std::function<void(int)> mFunc) { funcs.push_back(mFunc); return *this; }
    void operator()(int num) { for(auto& f : funcs) f(num); }
};

class SomeClass
{
    public:
        void Func(int num){std::cout << num <<" someClass" << std::endl;}
};

int main() {
    Delegate delegate;
    delegate += [](int num){ std::cout << num << " d1" << std::endl; };
    delegate += [](int num){ std::cout << num << " d2" << std::endl; };
    
    SomeClass sc = SomeClass();
    
    using namespace std::placeholders;
    delegate += std::bind(&SomeClass::Func, &sc, _1);
    
    
    delegate(1); // prints "hello, world!"
}