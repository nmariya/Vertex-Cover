#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <string>
#include <sys/wait.h>
#include <vector>

using namespace std;
void sig_usr(int signo){
    if(signo == SIGCHLD)
    exit(0);
}

int main (int argc, char **argv) {
    vector<pid_t> kids;
    pid_t pid1;
    pid_t pid2;
    pid_t pid3;
    int RtoA1[2];
    int A1toA2[2];
    // Pipe to communicate between Rgen and ece650-a1.py
    pipe(RtoA1);
    // Pipe to communicate between ece650-a1and ece650-a2.cpp
    pipe(A1toA2);
    // fork the driver program to two
    pid1 = fork();
    // child process to excecute rgen.cpp
    if(pid1 < 0){
        cerr << "Error: Could not fork\n";
		return 1;
    }
    else if(pid1 == 0){
        dup2(RtoA1[1], STDOUT_FILENO);
        close(RtoA1[1]);
        close(RtoA1[0]);
        close(A1toA2[0]);
        close(A1toA2[1]);
        char* cmd = (char*)"./rgen";
        argv[0] = (char*)"./rgen";
        execv(cmd, argv);
        cerr << "Error: Failed excecution of rgen\n";
        exit(0);
    }
    kids.push_back(pid1);
    // cout << pid1 << endl;
    pid2 = fork();
    if(pid2 < 0){
        cerr << "Error: Could not fork\n";
        return 1;
    }
    else if(pid2 == 0){
        dup2(RtoA1[0], STDIN_FILENO);
        close(RtoA1[1]);
        close(RtoA1[0]);

        dup2(A1toA2[1], STDOUT_FILENO);
        close(A1toA2[1]);
        close(A1toA2[0]);
        char* strng[4];
		strng[0] = (char*)"/usr/bin/python3";
        strng[1] = (char*)"-u";
		strng[2] = (char*)"./ece650-a1.py";
		strng[3] = NULL;
		char* cmd = (char*)"/usr/bin/python3";
		execvp(cmd, strng);
        cerr << "Error : Failed excecution of ece650-a1\n";
        return 1;
    }
    kids.push_back(pid2);
    // cout << pid2 << endl;
    pid3 = fork();
    if(pid3 < 0){
        cerr << "Error : Could not fork\n";
        return 1;
    }
    else if(pid3 == 0){
        close(RtoA1[0]);
        close(RtoA1[1]);
        dup2(A1toA2[0], STDIN_FILENO);
        close(A1toA2[0]);
        close(A1toA2[1]);
        char *cmd = (char*)"./ece650-a2";
        char *strng[1];
        strng[0] = NULL;
        execvp(cmd, strng);
        cerr << "Error : Failed excecution of ece650-a2\n";
    }
    // cout << pid3 << endl;
    close(RtoA1[0]);
    close(RtoA1[1]);
    dup2(A1toA2[1], STDOUT_FILENO);
    close(A1toA2[0]);
    close(A1toA2[1]); 
    if(signal(SIGCHLD, sig_usr)){
        kill(getpid(), 0);
    }
    while (!cin.eof())
    {
        // int sta;
        // waitpid(pid3, &sta, -1);
        string s;
        getline(cin, s);
        if(s == "") continue;
        cout << s << endl;
    }
    
    for (pid_t k : kids) {
        int status;
        kill(k, SIGTERM);
        waitpid(pid1, &status, 0);
        cerr << "Child terminated with status: " << status << endl;
    }
    exit(0);
}
