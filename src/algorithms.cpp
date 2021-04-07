#include <iostream>
#include <memory>
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"
#include "minisat/mtl/IntTypes.h"
#include "minisat/mtl/Vec.h"
#include <sstream>
#include <vector>
#include <string>
#include <bits/stdc++.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>
#include <fstream>
#define handle_error(msg)                                                      \
  do {                                                                         \
    perror(msg);                                                               \
    std::exit(EXIT_FAILURE);                                                   \
  } while (0)

using namespace std;
typedef struct graph {
    int V;
    vector<int> edg;
    string vc;
    unsigned long int time;
    int size;
} graph_t;

graph_t gph[3];
int vertices(string line){
    stringstream input(line);
    char cmd;
    input >> cmd;
    int V;
    input >> V;
    return V;
}

vector<int> edges(string line){
    int found;
    string e1;
    for(char &s1 : line){
        if((s1 != '{') && (s1 != '}') && (s1 != '<') && (s1 != '>') && (s1 != ',')){
            e1 = e1 + s1;
        }
        else if((s1 == '{') || (s1 == '}') || (s1 == '<') || (s1 == '>') || (s1 == ',')){
            e1 = e1 + ' ';
        }
    }
    stringstream ss(e1);
    char cmd;
    ss >> cmd;
    vector<int> conv;
    while (ss >> found){
        conv.push_back(found);
    }
    return conv;
}

// CPU clock time in microseconds
unsigned long int pclock(char *msg, clockid_t cid) {
    struct timespec ts;
    if (clock_gettime(cid, &ts) == -1){
        perror("clock_gettime");
        std::exit(0);
    }
    unsigned long int t = ts.tv_sec * 1000 * 1000 + ts.tv_nsec / 1000;
    return t;
}

void errp(char *s, int code) {
  std::cerr << "Error: " << s << " -- " << strerror(code) << std::endl;
}

// Thread sleep
void thr_sleep(time_t sec, long nsec) {
  struct timeval now;
  struct timezone tz;
  struct timespec ts;
  int retcode;

  pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

  retcode = pthread_mutex_lock(&m);
  if (retcode) {
    std::cerr << "Error: mutex_lock" << std::endl;
    std::exit(0);
  }

  gettimeofday(&now, &tz);
  ts.tv_sec = now.tv_sec + sec + (nsec / 1000000000L);
  ts.tv_nsec = now.tv_usec * 1000 + (nsec % 1000000000L);
  if (ts.tv_nsec > 1000000000L) {
    (ts.tv_sec)++;
    (ts.tv_nsec) -= 1000000000L;
  }

  retcode = pthread_cond_timedwait(&cond, &m, &ts);

  if (retcode != ETIMEDOUT) {
    if (retcode == 0) {
      std::cerr <<  "pthread_cond_timedwait, (zero)" << std::endl;
    } else {
      std::cerr <<  "pthread_cond_timedwait, " << strerror(retcode) << std::endl;
      std::exit(0);
    }
  }

  retcode = pthread_mutex_unlock(&m);
  if (retcode) {
    std::cerr <<  "mutex_unlock, " << strerror(retcode) << std::endl;
    std::exit(0);
  }

  retcode = pthread_cond_destroy(&cond);
  if (retcode) {
    std::cerr <<  "mutex_cond_destroy, " << strerror(retcode) << std::endl;
    fflush(stdout);
  }

  retcode = pthread_mutex_destroy(&m);
  if (retcode) {
    std::cerr <<  "mutex_destroy, " << strerror(retcode) << std::endl;
    std::exit(0);
  }
}

//Minisat solver
void *Vertex_Cover(void *data){
    stringstream sat_out;
    graph_t *gph = static_cast<graph_t *>(data);
    int V = gph->V;
    gph->time = 0;
    gph->size = 0;
    clockid_t cid;
    int retcode;
    // gph->tid = tid1;
    vector<int> edg = gph->edg;
    gph->vc = "CNF-SAT-VC: Time out\n";
    unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
    solver.reset (new Minisat::Solver());
    int k = 1;
    vector<int> vertex_cover;
    sat_out << "CNF-SAT-VC: ";
    while(k<=V){
        Minisat::Lit x[V][k];
        for(int i=1; i<=V; i++){
            for(int j=1; j<=k; j++){
                x[i][j] = Minisat::mkLit(solver->newVar());
            }
        }
        // clause 1
        for(int i=1; i<=k; i++){
            Minisat::vec<Minisat::Lit> literals;
            for(int j=1; j<=V; j++){
                literals.push(x[j][i]);
            }
            solver->addClause(literals);
            // clause 3
            for(int p=0; p<V; p++){
                for(int q=p+1; q<V; q++){
                    solver->addClause(~literals[p], ~literals[q]);
                }
            }
        }
        // clause 2
        for(int i=1; i<=V; i++){
            for(int j=1; j<=k; j++){
                for(int m=j+1; m<=k; m++)
                {
                    solver->addClause(~x[i][j], ~x[i][m]);
                }
            }
        }
        // clause 4
        int u,v;
        for(size_t i=0; i<edg.size()-1; i=i+2){
            u = edg[i];
            v = edg[i+1];
            Minisat::vec<Minisat::Lit> cover;
            for(int j=1; j<=k; j++){
                cover.push(x[u][j]);
                cover.push(x[v][j]);
            }
            solver->addClause(cover);
        }
        bool res = solver->solve();
        int m;
        if(res){
            for(int i=1; i<=V; i++){
                for(int j=1; j<=k; j++){
                    m = Minisat::toInt(solver->modelValue(x[i][j]));
                    if(m == 0){
                        vertex_cover.push_back(i);
                        sat_out << i << ' ';
                    }
                }
            }
            sat_out << endl;
            gph->vc = sat_out.str();
            retcode = pthread_getcpuclockid(pthread_self(), &cid);

            if (retcode) {
                errp("pthread_getcpuclockid", retcode);
            }
            else {
                gph->time = pclock("Thread t0 cpu time: ", cid);
            }
            gph->size = vertex_cover.size();
            return nullptr;
        }
        k++;
        solver.reset (new Minisat::Solver());
    }
    return nullptr;
}

//Approx-VC-1 solver
void *Vertex_Cover_1(void *data){
    stringstream out;
    graph_t *gph = static_cast<graph_t *>(data);
    vector<int> cover;
    while(gph->edg.size()>0){
        vector<int> new_edg;
        int max = 0;
        int k;
        for(size_t i=0; i<gph->edg.size(); i++){
            int count = 0;
            for(size_t j=0; j<gph->edg.size(); j++){
                if(gph->edg[i] == gph->edg[j]) count++;
            }
            if(count > max){
                max = count;
                k = gph->edg[i];
            }
        }
        cover.push_back(k);
        for(size_t p=0; p<gph->edg.size(); p=p+2){
            if(p%2 == 0 || p == 0){
                if(gph->edg[p] == k);
                else{
                    if(gph->edg[p+1] == k);
                    else{
                        new_edg.push_back(gph->edg[p]);
                        new_edg.push_back(gph->edg[p+1]);
                    }
                }
            }
        }
        if(gph->edg == new_edg) break;
        else  gph->edg = new_edg;
    }
    sort(cover.begin(), cover.end());
    out << "APPROX-VC-1: ";
    for(size_t i =0; i< cover.size(); i++){
        out << cover[i] << ' ';
    }  
    out << endl;
    gph->vc = out.str();
    clockid_t cid;
    int retcode;
    retcode = pthread_getcpuclockid(pthread_self(), &cid);

    if (retcode) {
        errp("pthread_getcpuclockid", retcode);
    }
    else {
        gph->time = pclock("Thread t1 cpu time: ", cid);
    }
    gph->size = cover.size();
    return nullptr; 
}

//Approx-VC-2 solver
void *Vertex_Cover_2(void *data){
    graph_t *gph = static_cast<graph_t *>(data);
    stringstream out2;
    //if no edges are present
    vector<int> edg = gph->edg;
    out2 << "APPROX-VC-2: ";
    vector<int> cv;
    //computing vertex cover
    while(edg.size() != 0){
        int x = 0;
        vector<int> new_edg;
        for(size_t y=0; y<edg.size()-1; y=y+2){
            if(edg[x] == edg[y] || edg[x] == edg[y+1] || edg[x+1] == edg[y] || edg[x+1] == edg[y+1]){
                ;
            }
            else{
                new_edg.push_back(edg[y]);
                new_edg.push_back(edg[y+1]);
            }
        }
        cv.push_back(edg[x]);
        cv.push_back(edg[x+1]);
        edg = new_edg;
    }

    //sorting the vertex cover
    sort(cv.begin(),cv.end());
    for(size_t i=0; i<cv.size(); i++){
        out2 << cv[i] << " ";
    }
    out2 << endl;
    gph->vc = out2.str();
    clockid_t cid;
    int retcode;
    retcode = pthread_getcpuclockid(pthread_self(), &cid);

    if (retcode) {
        errp("pthread_getcpuclockid", retcode);
    }
    else {
        gph->time = pclock("Thread t2 cpu time: ", cid);
    }
    gph->size = cv.size();
    return nullptr;
}


int main(void) {
    int V = -1;
    vector<int> edg;
    vector<int> vertex_cover;
    while(!cin.eof()){ 
        string line;
        getline(cin, line);
        if(line.length() == 0) continue;
        stringstream st(line);
        char cmd;
        st >> cmd;
        if(cmd == 'V'){
            V = vertices(line);
            edg.clear();
        }
        if(cmd == 'E'){
            edg = edges(line);
            int ret = 0;
            pthread_t t1,t2,t3;
            clockid_t cid;
            int retcode;
            if(edg.empty()){
                cout << "CNF-SAT-VC: " << endl;
                cout << "APPROX-VC-1: " << endl;
                cout << "APPROX-VC-2: " << endl;
            }
            else{
                for(int i=0; i<3; i++){
                    gph[i].V = V;
                    gph[i].edg = edg;
                } 
                ret = pthread_create(&t1, nullptr, &Vertex_Cover, &gph[0]);
                if (ret != 0)
                    handle_error("create");

                thr_sleep(5, 0);

                if (ret = pthread_kill(t1, 0)){
                    fflush(stdout);
                } 
                else {
                    ret = pthread_cancel(t1);
                    gph[0].time = 0;
                    if (!ret) {
                        fflush(stdout);
                    }
                    else {
                        fflush(stdout);
                    }
                    retcode = pthread_getcpuclockid(t1, &cid);

                    if (retcode) {
                        errp("pthread_getcpuclockid", retcode);
                    }
                    else {
                        gph[0].time = pclock("Thread t1 cpu time: ", cid);
                    }
                }
                ret = pthread_create(&t2, nullptr, &Vertex_Cover_1, &gph[1]);
                if (ret != 0)
                    handle_error("create");
                ret = pthread_create(&t3, nullptr, &Vertex_Cover_2, &gph[2]);
                if (ret != 0)
                    handle_error("create");
                pthread_detach(t1);
                ret = pthread_join(t2, nullptr);
                if (ret != 0)
                    handle_error("join");
                ret = pthread_join(t3, nullptr);
                if (ret != 0)
                    handle_error("join");
                for(int i=0; i<3; i++){
                    cout << gph[i].vc;
                }
            }     
        }       
    }
    return 0;
}