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
			strcpy(cmd, "echo hello");			
		}
		~foo(){
			delete cmd;
		}
		foo(const foo& rhs){
			cmd = new char[0x10];
			cout << "cmd @ " << (void *)cmd << endl;
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
	vec.erase(it);				 				  // copy assignment (vec[0] = vec[1])  destructor (vec[1])
	vec.erase(it);								  // destructor (vec[0]) => double free 
	return 0;									  
}









