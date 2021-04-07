#include <iostream>
#include <memory>
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"
#include "minisat/mtl/IntTypes.h"
#include "minisat/mtl/Vec.h"
#include <sstream>
#include <vector>
#include <string>

using namespace std;
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

vector<int> Vertex_Cover(vector<int> edg, int V){
    unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
    int k = 1;
    vector<int> vertex_cover;
    while(k<=V){
        // cout << "k = " << k << endl;
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
            // cout << u << ' ' << v << endl;
            Minisat::vec<Minisat::Lit> cover;
            for(int j=1; j<=k; j++){
                cover.push(x[u][j]);
                cover.push(x[v][j]);
            }
            solver->addClause(cover);
        }
        bool res = solver->solve();
        // cout << "The result is: " << res << "\n";
        int m;
        if(res){
            for(int i=1; i<=V; i++){
                for(int j=1; j<=k; j++){
                    m = Minisat::toInt(solver->modelValue(x[i][j]));
                    if(m == 0){
                        vertex_cover.push_back(i);
                    }
                }
            }
            return vertex_cover;
        }
        k++;
        solver.reset (new Minisat::Solver());
    }
}

int main(int argc, char **argv) {
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
        // cout << V << endl;
        if(cmd == 'E'){
            edg = edges(line);
            if(edg.empty()) cout << endl;
            else{
                vertex_cover = Vertex_Cover(edg, V);
                for(size_t i =0; i< vertex_cover.size(); i++){
                    cout << vertex_cover[i] << ' ';
                }  
                cout << endl; 
            }     
        }        
    }

    return 0;
}
