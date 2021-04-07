#include <unistd.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>

using namespace std;

int random(int l, int u){
    unsigned random_value = 0; 
    int random_number = 0;
	size_t size = sizeof(random_value);
    ifstream urandom("/dev/urandom");
    if(urandom){
        urandom.read(reinterpret_cast<char*>(&random_value), size);
		if (urandom)
		{
			random_number = l + random_value % (u-l+1);
		}
		else
		{
			std::cerr << "Error: Failed to read from /dev/urandom" << std::endl;
		}
		urandom.close(); 
	}
	else 
	{
		std::cerr << "Error: Failed to open /dev/urandom" << std::endl;
	}
	return random_number;
}

string street_name(){
    string alp = "abcdefghijklmnopqrstuvwxyz";
    string name = "";
    for(int i=0; i<8; i++){
        int val = random(1,26);
        name += alp[val];
    }
    name += " Street";
    return name;
}

struct Point 
{ 
    int x; 
    int y; 
};

bool onSegment(Point p, Point q, Point r) 
{ 
    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) && 
        q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y)) 
       return true; 
  
    return false; 
} 

int orientation(Point p, Point q, Point r) 
{  
    int val = (q.y - p.y) * (r.x - q.x) - 
              (q.x - p.x) * (r.y - q.y); 
  
    if (val == 0) return 0;  // colinear 
  
    return (val > 0)? 1: 2; // clock or counterclock wise 
} 

bool doIntersect(Point p1, Point q1, Point p2, Point q2) 
{ 
    // Find the four orientations needed for general and 
    // special cases 
    int o1 = orientation(p1, q1, p2); 
    int o2 = orientation(p1, q1, q2); 
    int o3 = orientation(p2, q2, p1); 
    int o4 = orientation(p2, q2, q1); 
  
    // General case 
    if (o1 != o2 && o3 != o4) 
        return true; 
  
    // Special Cases 
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1 
    if (o1 == 0 && onSegment(p1, p2, q1)) return true; 
  
    // p1, q1 and q2 are colinear and q2 lies on segment p1q1 
    if (o2 == 0 && onSegment(p1, q2, q1)) return true; 
  
    // p2, q2 and p1 are colinear and p1 lies on segment p2q2 
    if (o3 == 0 && onSegment(p2, p1, q2)) return true; 
  
     // p2, q2 and q1 are colinear and q1 lies on segment p2q2 
    if (o4 == 0 && onSegment(p2, q1, q2)) return true; 
  
    return false; // Doesn't fall in any of the above cases 
} 

bool same_coordinates(Point n, Point m){
    if(n.x == m.x && n.y == m.y) return true;
    else return false;
}

bool overlap(Point n1, Point n2, Point m1, Point m2){
    int col1 = orientation(n1, n2, m1);
    int col2 = orientation(n1, n2, m2);
    if((same_coordinates(n1,m1)||same_coordinates(n1,m2))&&(same_coordinates(n2,m1)||same_coordinates(n2,m2))){
        return true;
    }
    else{
        if(col1 == 0 && col2 == 0){
            if(same_coordinates(n1,m1)){
                if(onSegment(m1,n2,m2)||onSegment(m1,m2,n2)) return true;
            }
            else if(same_coordinates(n1,m2)){
                if(onSegment(m2,n2,m1)||onSegment(m2,m1,n2)) return true;
            }
            else if(same_coordinates(n2,m1)){
                if(onSegment(m1,n1,m2)||onSegment(m1,m2,n1)) return true;
            }
            else if(same_coordinates(n2,m2)){
                if(onSegment(m2,n1,m1)||onSegment(m2,m1,n1)) return true;
            }
            if(doIntersect(n1,n2,m1,m2)) return true;
            else return false;
        }
        else return false;
    }
    return false;
}

bool new_coord_self_intersect_valid(vector<Point> coord_list, Point p2){
    int count = 0;
    struct Point p1;
    struct Point w1;
    struct Point w2;
    if(coord_list.size() > 3){
        p1 = coord_list.at(coord_list.size()-1);
        for(unsigned i=0; i<coord_list.size()-2; i++){
            w1 = coord_list.at(i);
            w2 = coord_list.at(i+1);
            if(doIntersect(w1, w2, p1, p2)){
                count += 1;
            }
        }
        if(count > 0) return false;
        else return true;
    }
    else return true;
}

bool new_coord_overlap_valid(vector<Point> *full_list, int list_size, Point m1, Point m2){
    Point n1,n2;
    int counter = 0;
    for(int i=0; i<list_size; i++){
        for(unsigned j=0; j<full_list[i].size()-1; j++){
            n1 = full_list[i].at(j);
            n2 = full_list[i].at(j+1);
            if(overlap(n1,n2,m1,m2)){
                counter = counter +1;
            }
        }
    }
    if(counter > 0) return false;
    else return true;
}

bool check_zero_length_edges(Point n, Point m){
    if(same_coordinates(n,m)) return true;
    else return false;
}

struct Point street_generation(int coordinate_range){
    int cor1 = random(-coordinate_range,coordinate_range);
    int cor2 = random(-coordinate_range,coordinate_range);
    struct Point p;
    if(cor1 == 0){
        cor1 = random(-coordinate_range,coordinate_range);
    }
    if(cor2 == 0){
        cor2 = random(-coordinate_range,coordinate_range);
    }
    p.x = cor1;
    p.y = cor2;
    return p;
}

// void terminate_rgen(){
//     cerr << "Error: Failed to generate a valid input after 25 attempts\n";
//     exit(0);
// }

int main(int argc, char **argv){
    // initializing default arguments
    int streets = 10;
    int line_segments = 5;
    int wait_time = 5;
    int coord_range = 20;
    int c;
    opterr = 0;
    string temp;
    while((c = getopt(argc, argv, "s:n:l:c:")) !=-1){
        switch (c)
        {
        case 's':
            temp = optarg;
            streets = atoi(temp.c_str());
            if(streets < 2){
                cerr << "Error: Invalid CLI value.\n";
                exit(0);
            }
            break;
        case 'n':
            temp = optarg;
            line_segments = atoi(temp.c_str());
            if(line_segments < 1){
                cerr << "Error: Invalid CLI value.\n";
                exit(0);
            }
            break;
        case 'l':
            temp = optarg;
            wait_time = atoi(temp.c_str());
            if(wait_time < 5){
                cerr << "Error: Invalid CLI value.\n";
                exit(0);
            }
            break;
        case 'c':
            temp = optarg;
            coord_range = atoi(temp.c_str());
            if(coord_range < 1){
                cerr << "Error: Invalid CLI value.\n";
                exit(0);
            } 
            break;
        default:
            break;
        }
    }
    while(true){
        struct Point p;
        // struct Point p1;
        // struct Point p2;
        // struct Point p3;
        // struct Point p4;
        int counter = 0;
        vector<string> street_names;
        int no_of_streets = random(2,streets);
        int *street_array = new int[no_of_streets];
        for(int i=0; i<no_of_streets; i++){
            street_array[i] = random(1,line_segments);
        }
        vector<Point> *final_coordinates = new vector<Point>[no_of_streets];
        for(int i=0; i<no_of_streets; i++){
            // cout << street_array[i] << endl;
            for(int j=0; j<=street_array[i]; j++){
                // cout <<j <<endl;
                int coordinate_range = random(1,coord_range);
                p = street_generation(coordinate_range);
                if(j == 0) final_coordinates[i].push_back(p);
                if(j>0){
                    if(new_coord_self_intersect_valid(final_coordinates[i], p) && !check_zero_length_edges(final_coordinates[i].at(j-1),p) && new_coord_overlap_valid(final_coordinates, i, final_coordinates[i].at(j-1), p)){
                        final_coordinates[i].push_back(p);
                        // cout << "p =" << p.x << " " << p.y << endl;
                    }
                    else{
                            j = j-1;
                            if(counter < 25){
                                counter = counter+1;
                        }
                        else{
                            cerr << "Error: Failed to generate a valid input after 25 attempts\n";
                            exit(0);
                        }
                    }
                }
            }
        }
        vector<string> street_name_list;
        for(int i =0; i<no_of_streets; i++){
            string name = street_name();
            street_name_list.push_back(name);
            cout << "add \"" << name << "\" ";
            for(int j=0; j<=street_array[i]; j++){
                cout << "(" << (final_coordinates[i].at(j)).x << "," << (final_coordinates[i].at(j)).y << ") ";
            }
            cout << endl;
        }
        cout << "gg" << endl;
        int waaaiit = random(5,wait_time);
        sleep(waaaiit);
        for(unsigned n=0; n<street_name_list.size(); n++){
            cout << "rm \"" << street_name_list[n] << "\"" << endl;
        }
    }
}
