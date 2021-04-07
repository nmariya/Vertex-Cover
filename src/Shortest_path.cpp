// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <list>

using namespace std;

class DB {
private:
	vector<vector<int>> neighbour;
public:
	void set_neighbour(vector<vector<int>> nbr) {
		neighbour = nbr;
	}
	vector<vector<int>> get_neighbour() {
		return neighbour;
	}
};

class CheckVertex{
    public:
    static int check_vertex(int v){
        if(v > 1){
            return v;
        }
        else if(v <= 1){
            std::cout << "Error: Vertex has to be greater than 1" << endl;
            v = -1;
            return v;
        }
        return v;
    }
};

class CheckSrcDest{
    public:
    static bool check_src_dest(int s, int d, int chk){
        int src=0;
        int dst=0;
        if(s < 0 || d < 0){
            std::cout << "Error: Vertex has to be a positive number" << endl;
            src = -1;
            dst = -1;
            return false;
        }
        else if(s > chk || d > chk){
            std::cout << "Error: Vertex does not exist" << endl;
            src = -1;
            dst = -1;
            return false;
        }
        else{
            src = s;
            dst = d;
            return true;
        }
    } 
};

class CheckEdges{
    public:
    static string check_edges(string edge, int chk){
        int found;
        string e;
        string e1;
        for(char &s1 : edge){
            if((s1 != '{') && (s1 != '}') && (s1 != '<') && (s1 != '>') && (s1 != ',')){
                e1 = e1 + s1;
            }
            else if((s1 == '{') || (s1 == '}') || (s1 == '<') || (s1 == '>') || (s1 == ',')){
                e1 = e1 + ' ';
            }
        }
        stringstream ss(e1);
        vector<int> conv;
        while (ss >> found){
            conv.push_back(found);
        }
        for(size_t q =0; q < conv.size(); q++){
            if(conv[q] > chk){
                std::cout << "Error: Vertex ID ouside the specified range" << endl;
                e = "";
                break;
            }
            if(conv[q] <= 0){
                std::cout << "Error: Vertex ID has to be non-zero and positive" << endl;
                e = "";
                break;
            }
            e = e + to_string(conv[q])+ ' ';
        }
        return e;
    }
};

class AdjacencyList{
    public:
    static vector<vector<int>> adj_list(DB &db, string e, int chk){
        stringstream iss(e);
        int number;
        vector<int> uv;
        while (iss >> number){
            uv.push_back(number);
        }
        vector<vector<int>> neighbour;
        for (int i = 0; i <= chk; i++) {
			vector<int> vtr;
			neighbour.push_back(vtr);
		}
        for(unsigned int pos = 0; pos < uv.size(); pos = pos+2){
            neighbour[uv[pos]].push_back(uv[pos+1]);
            neighbour[uv[pos+1]].push_back(uv[pos]);
        }
        db.set_neighbour(neighbour);
        return neighbour;
    }
};

class BFS{
    public:
    static bool bfs(int s, int d, vector<vector<int>> neighbour, int chk, int parent[]){
        list<int> stack;
        bool *visited = new bool[chk];
        for(int i=0; i<=chk; i++){
            visited[i] = false;
            parent[i] = -1;
        }
        stack.push_back(s);
        visited[s] = true;
        while(!stack.empty()){
            int u = stack.front();
            stack.pop_front();
            for(std::size_t j=0; j < neighbour[u].size(); j++){
                if(!visited[neighbour[u][j]]){
                    visited[neighbour[u][j]] = true;
                    parent[neighbour[u][j]] = u;
                    stack.push_back(neighbour[u][j]);
                    if(neighbour[u][j] == d){
                        return true;
                    }
                }
            }
        }
    return false;
    }
    static bool shortest_path(int s, int d, vector<vector<int>> neighbour, int chk){
        int parent[chk];
        if(bfs(s,d,neighbour,chk, parent) == false){
            std::cout << "Error: Path does not exist between the vertices" << std::endl;
            return false;
        }
        vector<int> path;
        int root = d;
        path.push_back(root);
        while(parent[root] != -1){
            path.push_back(parent[root]);
            root = parent[root];
        }
        for(size_t i = path.size()-1; i > 0; i--){
            cout << path[i] << '-';
        }
        cout << path[0] << endl;
        return true;

        return false;
    }
};

int main(int argc, char** argv) {
    DB db;
    // read from stdin until EOF
    while (!std::cin.eof()) {
        std::string line;
        std::getline(std::cin, line);
        if(line.length() == 0){
            continue;
        }
        std::istringstream input(line);
        int num;
        int source;
        int dest;
        int vertex;
        string e;
        vector<vector<int>> neighbour;
        while (!input.eof()) {
            char cmd;
            // parse an character
            input >> cmd;
            ws(input);
            if (input.fail()) {
                std::cout << "Error: Parsing a command" << endl;
                break;
            }
            if(cmd == 'V'){
                vertex = -1;
                vector<vector<int>> reset;
                db.set_neighbour(reset);
                std::ws(input);
                if(input.eof()){
                    std::cout << "Error: Missing argument" << endl;
                    break;
                }
                num=0;
                input >> num;
                if(input.fail()){
                    std::cout << "Error: Unknown argument" << endl;
                    break;
                }
                std::ws(input);
                if(!input.eof()){
                    std::cout << "Error: Unexpected argument" << endl;
                    break;
                }
                cout << line << endl;
                vertex = CheckVertex::check_vertex(num);
            }
            else if(cmd == 'E'){
                if(vertex == -1){
                    break;
                }
                std::ws(input);
                if(input.eof()){
                    vertex = -1;
                    std::cout << "Error: Missing argument" << endl;
                    break;
                }
                string edge;
                input >> edge;
                std::ws(input);
                if(!(input.eof())){
                    vertex = -1;
                    std::cout << "Error: Unexpected argument" << endl;
                    break;
                }
                cout << line << endl;
                e = CheckEdges::check_edges(edge, vertex);
                if(e == ""){
                    vertex = -1;
                }
                neighbour = AdjacencyList::adj_list(db, e, vertex);
            }      
            else if(cmd == 's'){
                if(vertex == -1){
                    break;
                }
                std::ws(input);
                if(input.eof()){
                    std::cout << "Error: Missing argument" << endl;
                    break;
                }
                source=0;
                input >> source;
                if(input.fail()){
                    std::cout << "Error: Unknown argument 1" << endl;
                    break;
                }
                dest=0;
                input >> dest;
                if(input.fail()){
                    std::cout << "Error: Unknown argument 2" << endl;
                    break;
                }
                std::ws(input);
                if(!input.eof()){
                    std::cout << "Error: Unexpected argument" << endl;
                    break;
                }
                if(CheckSrcDest::check_src_dest(source, dest, vertex)){
                    BFS::shortest_path(source, dest, db.get_neighbour(), vertex);
                }
            }
            else{
                break;;
            }
            
            if (input.eof())
                break;
        }
    }
}
