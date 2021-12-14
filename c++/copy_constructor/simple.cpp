#include <iostream>
#include <cstring>

using namespace std;

class foo {
    public:
        foo() {
            cout << "call constructor" << endl;
            bar = new char[0x80];
        }

        // 正確做法
        /*foo(const foo &rhs) {
            cout << "call copy constructor" << endl;
            bar = new char[0x80];
            memcpy(bar, rhs.bar, 0x80);
        }*/

        ~foo() {
            delete[] bar;
        }

    private:
        char *bar;
};


// 因為 obj 是 pass by value, 所以會呼叫 copy constructor 而不是 constructor
// foo 沒實做 copy constructor 所以預設使用 shallow copy  => obj.bar == obj1.bar
void vuln(foo obj)
{
    // 呼叫 obj 的 destrutor => delete[] bar;
}

int main()
{
    foo obj1;
    vuln(obj1);

    // 呼叫 obj1 的 destructor => delete[] bar => double free !!
}

