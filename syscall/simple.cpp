// g++ simple.cpp -o chall -ggdb
/*
題目簡介:
可以呼叫 10 次 syscall, 但有限制 (check())
只能設定 rax, rdi, rsi 和 rdx

作法:
用 brk leak heap 位址
因為 class 有用 virtual function => class 存在 virtual table
又 class 用 new 配置 => virtual table 在 heap
所以有 heap 位址後, 用 writev 能 leak virtual function 的位址, 進而得到 pie_base
呼叫 mprotect 讓程式碼可改
用 sys_arch_prctl patch 掉 check 讓我們能呼叫 read
再用 read 將 shell code 寫入程式碼
*/

#include <iostream>
#include <sys/syscall.h>

using namespace std;

class Emulator {
private:
  unsigned long rax, rdi, rsi, rdx;
  virtual void set(string, unsigned long&);
  virtual int check();
  virtual void syscall();
public:
  Emulator():rax(0),rdi(0),rsi(0),rdx(0){}
  virtual void emulate();
};

void Emulator::set(string msg, unsigned long &reg) {
  cout << msg;
  cin >> reg;
  if (!cin.good()) exit(1);
}

int Emulator::check() {
  if (this->rax >= 0x40000000)   return 1; // x32 ABI is dangerous!
  if (this->rax == SYS_open)     return 1; // never open files
  if (this->rax == SYS_openat)   return 1;
  if (this->rax == SYS_write)    return 1; // no more leak
  if (this->rax == SYS_read)     return 1; // no more overwrite
  if (this->rax == SYS_sendfile) return 1;
  if (this->rax == SYS_execve)   return 1; // of course not!
  if (this->rax == SYS_execveat) return 1;
  if (this->rax == SYS_ptrace)   return 1; // may ruine the program
  if (this->rax == SYS_fork)     return 1;
  if (this->rax == SYS_vfork)    return 1;
  if (this->rax == SYS_clone)    return 1;
  return 0;
}

void Emulator::syscall() {
  asm volatile ("movq %0, %%rdi":: "a"(this->rdi));
  asm volatile ("movq %0, %%rsi":: "a"(this->rsi));
  asm volatile ("movq %0, %%rdx":: "a"(this->rdx));
  asm volatile ("movq %0, %%rax":: "a"(this->rax));
  asm volatile ("syscall");
  asm volatile ("movq %%rax, %0": "=a"(this->rax));
}

void Emulator::emulate()
{
  for(int i = 0; i < 10; i++) {
    cout << "=========================" << endl;
    this->set("syscall: ", this->rax);
    this->set("arg1: ", this->rdi);
    this->set("arg2: ", this->rsi);
    this->set("arg3: ", this->rdx);
    cout << "=========================" << endl;
    
    if (this->check()) {
      cerr << "syscall=" << this->rax << " is not allowed" << endl;
      continue;
    }
    else {
      this->syscall();
      cout << "retval: " << hex << this->rax << endl;
    }
  }
  cout << "Bye!" << endl;
}

Emulator *m;

void setup()
{
  setbuf(stdin, NULL);
  setbuf(stdout, NULL);
  setbuf(stderr, NULL);
  m = new Emulator();
}

int main(void)
{
  setup();
  m->emulate();
  exit(0);
}