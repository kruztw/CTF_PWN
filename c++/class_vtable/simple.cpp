#include <iostream>
#include <cstring>

using namespace std;

class base {
    public:
        void base_func() {
            cout << "base_func" << endl;
        }

        virtual void base_virtual1() {
            cout << "base_virtual1" << endl;
        }

        virtual void base_virtual2() {
            cout << "base_virtual2" << endl;
        }
        
        float base_arg;
};

class foo: public base {
    public:
       void foo_func() {
           cout << "foo_func" << endl;
       }

       void base_virtual2() {
           cout << "foo_virtual2" << endl;
       }

    private:
        int foo_arg;
};


int main()
{
    // 不是用 new 的不管是不是 virtual function 都直接跳到該函式的位址
    foo obj1;
    obj1.base_func();
    obj1.foo_func();
    obj1.base_virtual1();
    obj1.base_virtual2();

    // 用 new 分配的, 在呼叫 virtual function 時, 會查詢 vtable
    /* 0x403d98 <vtable for foo>:	0x0000000000000000	0x0000000000403dd8
       0x403da8 <vtable for foo+16>:	0x0000000000401362	0x0000000000401416
       0x403db8 <vtable for base>:	0x0000000000000000	0x0000000000403df0
       0x403dc8 <vtable for base+16>:	0x0000000000401362	0x000000000040139e
       0x403dd8 <typeinfo for foo>:	0x00007ffff7f86c98	0x0000000000402041

       gef> x/gx 0x0000000000403dd8
       0x403dd8 <typeinfo for foo>:	0x00007ffff7f86c98
       gef> x/gx 0x0000000000401362
       0x401362 <base::base_virtual1()>:	0xe5894855fa1e0ff3
       gef> x/gx 0x0000000000401416
       0x401416 <foo::base_virtual2()>:	0xe5894855fa1e0ff3
       gef> x/gx 0x0000000000403df0
       0x403df0 <typeinfo for base>:	0x00007ffff7f86008
       gef> x/gx 0x0000000000401362
       0x401362 <base::base_virtual1()>:	0xe5894855fa1e0ff3
       gef> x/gx 0x000000000040139e
       0x40139e <base::base_virtual2()>:	0xe5894855fa1e0ff3
    */

    foo *obj2 = new foo();
    obj2->base_func();
    obj2->foo_func();
    obj2->base_virtual1();
    obj2->base_virtual2();

}

