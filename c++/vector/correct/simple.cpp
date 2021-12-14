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
		foo(const foo& rhs){
			cout << "copy constructor" << endl;
			cmd = new char[0x10];
			cout << "cmd @ " << (void *)cmd << endl;
			strcpy(cmd, rhs.cmd);
		}
		foo& operator=(const foo& rhs){
			cout << "copy assignment" << endl;
			//cout << "cmd @ " << (void *)cmd << endl;
			//cout << "rhs.cmd @ " << (void *)rhs.cmd << endl;
			delete cmd;
			cmd = new char[0x30];
			strcpy(cmd, rhs.cmd);
		}

	private:
		char *cmd;
};

int main()
{
	foo foo1, foo2;                               // constructor x2
	vector<foo> vec;                         
	vec.push_back(foo1);                          // copy constructor
	vec.push_back(foo2);                          // copy constructor x2  destructor 
	vector<foo>::iterator it = vec.begin();
	vec.erase(it);								  // copy assignment (vec[0] = vec[1])  destructor (vec[1])
	return 0;									  // destructor x1 (vec[0]) destructor x2 (for foo1 & foo2)
}









