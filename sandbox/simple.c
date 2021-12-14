//////////////////////////////////////////////////////////
//						                                //
//  fork fail : Resource temporarily unavailable        //
//  ulimit -u  or use sudo ./simple                     //
//  							                        //
//////////////////////////////////////////////////////////

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <sys/resource.h>
#include <sys/prctl.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <sys/user.h>
#include <sys/mman.h>


void limit_max_resource()
{
    struct rlimit r;
    r.rlim_cur = 0x6400000;
    r.rlim_max = 0x6400000;
    setrlimit(RLIMIT_AS, &r);
    r.rlim_cur = 10;
    r.rlim_max = 10;
    setrlimit(RLIMIT_CPU, &r);
    r.rlim_cur = 1000;
    r.rlim_max = 1000;
    setrlimit(RLIMIT_FSIZE, &r);
    r.rlim_cur = 100;
    r.rlim_max = 100;
    setrlimit(RLIMIT_NOFILE, &r);
    r.rlim_cur = 40;
    r.rlim_max = 40;
    setrlimit(__RLIMIT_NPROC, &r);
}

void load_shellcode()
{
	const size_t psize = getpagesize();
    char *mem = (char *)mmap(NULL, psize, PROT_EXEC|PROT_WRITE|PROT_READ, MAP_ANON|MAP_PRIVATE, -1, 0);
    char *p = mem;

    dprintf(STDOUT_FILENO, "> ");
	read(0, p, psize-1);
    ((void (*)(void))mem)();
}

int filter_syscall(pid_t pid, struct user_regs_struct *regs)
{    
    uint64_t rax = regs->orig_rax;
    if(rax == SYS_execve || rax == SYS_open || rax == SYS_execveat || rax == SYS_fork)
    	return 1;
    return 0;
}


int main()
{
    pid_t pid;
    int status;
    unsigned int signo = 0;
    struct user_regs_struct regs;

    limit_max_resource();

    pid = fork();
    if ( pid < 0 ) {
        dprintf(STDOUT_FILENO, "fork fail : %s\n", strerror(errno));
        return 1;
    }
    if ( !pid ) { // child process
        prctl(PR_SET_PDEATHSIG, SIGKILL);
        if ( getppid() != 1 ) {
            if ( ptrace(PTRACE_TRACEME, 0, NULL, NULL) ) {
                dprintf(STDOUT_FILENO, "child traceme : %s\n", strerror(errno));
                return 1;
            }
            pid_t p = getpid();
            kill(p, SIGSTOP);
            load_shellcode();
            _exit(0);
        }
        dprintf(STDOUT_FILENO, "child is orphaned\n");
        _exit(1);
    }

    if ( waitpid(pid, &status, __WALL) < 0 || !WIFSTOPPED(status) || WSTOPSIG(status) != SIGSTOP ) {
        dprintf(STDOUT_FILENO, "initial waitpid fail 0x%x : %s\n", (unsigned int)status, strerror(errno));
        return 1;
    }

    ptrace(PTRACE_SETOPTIONS, pid, NULL, PTRACE_O_EXITKILL);
    while ( 1 ) {
        while ( 1 ) {
            if ( ptrace(PTRACE_SYSCALL, pid, NULL, signo) ) {
                if ( errno != ECHILD ) {
                    dprintf(STDOUT_FILENO, "ptrace syscall1 : %s\n", strerror(errno));
                    kill(pid, SIGKILL);
                    return 1;
                }
                return 0;
            }
            if ( waitpid(pid, &status, __WALL) < 0 ) {
                if ( errno != ECHILD ) {
                    dprintf(STDOUT_FILENO, "waitpid fail : %s\n", strerror(errno));
                    return 1;
                }
                return 0;
            }
            if ( !WIFSTOPPED(status) )
                return 0;

            if ( (signo = WSTOPSIG(status)) == SIGTRAP )
                break;
            dprintf(STDERR_FILENO, "child signal %d\n", signo);
        }

        if ( ptrace(PTRACE_GETREGS, pid, 0, &regs) ) {
            dprintf(STDOUT_FILENO, "ptrace getregs %s\n", strerror(errno));
            kill(pid, SIGKILL);
            return 1;
        }

        if ( !filter_syscall(pid, &regs) ) {
            dprintf(STDERR_FILENO, "allowed syscall %lld(%lld, %lld, %lld, %lld)\n", regs.orig_rax, regs.rdi, regs.rsi, regs.rdx, regs.r10);
        } else {
            char ban[] = "not support!";
            dprintf(STDERR_FILENO, "blocked syscall %lld\n", regs.orig_rax);
            regs.orig_rax = SYS_write;
            regs.rdi = 1;
            regs.rdx = strlen(ban);
            regs.rsi = regs.rsp;
            if ( ptrace(PTRACE_SETREGS, pid, 0, &regs) ){
			    dprintf(STDOUT_FILENO, "ptrace setregs : %s\n", strerror(errno));
			    kill(pid, SIGKILL);
			    return 1;
            }

            for ( int i = 0; i < strlen(ban); i += 8 ) {
                unsigned long data = *(int64_t *)&ban[i];
                unsigned long addr = i + regs.rsp;
                ptrace(PTRACE_POKEDATA, pid, addr, data); 
            }
        }
        if ( ptrace(PTRACE_SYSCALL, pid, 0, 0) ) {
            if ( errno != ECHILD ) {
                dprintf(STDOUT_FILENO, "ptrace syscall2 %s\n", strerror(errno));
                kill(pid, SIGKILL);
                return 1;
            }
            return 0;
        }

        if ( waitpid(pid, &status, __WALL) < 0 ) {
            if ( errno != ECHILD ) {
                dprintf(STDOUT_FILENO, "waitpid fail %s\n", strerror(errno));
                return 1;
            }
            return 0;
        }
        if ( !WIFSTOPPED(status) ) {
            dprintf(STDOUT_FILENO, "so long, sucker. 0x%x\n", status);
            return 0;
        }
        signo = 0;
    }

    return 0;
}
