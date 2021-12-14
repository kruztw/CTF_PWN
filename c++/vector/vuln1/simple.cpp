#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>

using namespace std;

class foo{
	public:
		foo(){
			cout << "constructor" << endl;
			cmd = new char[0x10];
			cout << "cmd @ " << (void *)cmd << endl;
			strcpy(cmd, "echo hello");			
		}
		~foo(){
			cout << "destructor" << endl;
			cout << "delete cmd @ " << (void *)cmd << endl;
			delete cmd;
		}

	private:
		char *cmd;
};

int main()
{
	foo foo1;
	vector<foo> vec;                         
	vec.push_back(foo1);                          // copy constructor => shallow copy
	return 0;									  // destructor (vec[0]) destructor (foo1) => (double free)
}









