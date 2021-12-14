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
			delete cmd;
		}
		void Sys(){
			system(cmd);
		}
		void edit(const char *cmd_){
			strncpy(cmd, cmd_, 0x10);
		}
	private:
		char *cmd;
};

int main()
{
	foo foo1, foo2;                               // constructor x2
	vector<foo> vec;                         
	vec.push_back(foo1);                          // copy constructor (vec[0].cmd == foo1.cmd)
	vec.push_back(foo2);                          // copy constructor x2  destructor (vec[0])
	foo1.edit("sh");                              // use after free
	vec[0].Sys();									
	return 0;
}









