/*******************************************************************************
 * Name        : waterjugpuzzle.cpp
 * Author      : Jesse Knuckles
 * Date        : 10/15/22
 * Description : Solves the water jug puzzle
 * Pledge      : "I pledge my honor that I have abided by the Stevens Honor System"
 ******************************************************************************/

#include <iostream>
#include <sstream>
#include <queue>
#include <vector> 
#include <string>
#include <iomanip>
#include <stack>
#include <unordered_map>
using namespace std;
// Struct to represent state of water in the jugs.
struct State {
    int a, b, c;
    string directions;
    State *parent;
    
    State(int _a, int _b, int _c, string _directions) : 
        a{_a}, b{_b}, c{_c}, directions{_directions}, parent{nullptr} { }
    
    // String representation of state in tuple form.
    string to_string() {
        ostringstream oss;
        oss << "(" << a << ", " << b << ", " << c << ")";
        return oss.str();
    }
    //sees if one fonem equal to other fonem
    bool equals(State state) {
        if ((a == state.a) && (b == state.b) && (c == state.c)) {
            return 1;
        }
        else {
            return 0;
        }
    }
};
//pours water from one cup to another, returns true if pour is possible,
//returns false otherwise
stack<State*> global;
bool bpour(int &pourer, int &pouree, int cap) {
    if ((pouree == cap) || (pourer == 0)) {
       return false;
    }
    else {
        return true;
    }
}
//pours water from one cup to another and saves the value of the new pourer in the 0 index of the result vector, the new value of the pouree 
//in the 1 index, and the amount of water poured in the 2 index.
vector<int> pour(int pourer, int pouree, int cap) {
    vector<int> ans;
    if (cap - pouree > pourer) {
        int temp = pourer;
            pouree += pourer;
            pourer = 0;
            ans.push_back(pourer);
            ans.push_back(pouree);
            ans.push_back(temp);
        }
    else {
        int temp = cap-pouree;
        pourer -= cap - pouree;
        pouree = cap;
        ans.push_back(pourer);
        ans.push_back(pouree);
        ans.push_back(temp);
    }
    return ans;
}
//prints the directions, and the states.
void print(State* target) {
            stack<State> st;
            State* current;
            current = target;
            while (current != nullptr) {
                st.push(*current);
                current = (*current).parent;
            }
            while (!(st.empty())) {
                cout << (st.top()).directions << (st.top()).to_string() << endl;
                st.pop();
            }
}

void bfs(int a, int b, int c, int A, int B) {
    queue<State*> q; // init queue q
    State* s = new State(0, 0, c, "Initial state. "); // initialize state object s to State(0,0,8)
    global.push(s);
    if ((A == s->a) && (B == s->b)) {
        cout << s->directions << s->to_string();
        return;
    }
    bool solution = false;
    q.push(s); // add s to q
    bool **graph = new bool*[a+1];
    for (int i = 0; i < a+1; i++) {
        graph[i] = new bool[b+1];
        fill(graph[i], graph[i] + b+1, false);
    }
    graph[0][0] = true; 
    while (!(q.empty())) {
        State* curr = q.front(); // peek element at front of q
//         // try the 6 ways to pour water, pushing new States to the queue
        
        if (bpour(curr->c, curr->a, a)) {
            vector<int> vec = pour(curr->c, curr->a, a);
            string dir;
            if (vec[2] > 1) {
                dir = "Pour " + to_string(vec[2]) + " gallons from C to A. ";
            }
            else {
                dir = "Pour " + to_string(vec[2]) + " gallon from C to A. "; 
            }
            State* new_state = new State(vec[1], c - (vec[0] + vec[1]), vec[0], dir);
            new_state->parent = curr;
            global.push(new_state);
            if ((new_state->a == A) && (new_state->b == B)) {
                for (int i = 0; i < a+1; i ++) {
                delete[] graph[i];
                    }
                delete[] graph; 
                solution = true;
                print(new_state);
                break;
            }
            if (!graph[new_state->a][new_state->b]) {
                q.push(new_state);
                graph[curr->a][curr->b] = true;
            }
        }
        if (bpour(curr->b, curr->a, a)) {
            vector<int> vec = pour(curr->b, curr->a, a);
            string dir;
            if (vec[2] > 1) {
                dir = "Pour " + to_string(vec[2]) + " gallons from B to A. ";
            }
            else {
                dir = "Pour " + to_string(vec[2]) + " gallon from B to A. "; 
            }
            State* new_state = new State(vec[1], vec[0], c - (vec[0]+vec[1]), dir);
            new_state->parent = curr;
            global.push(new_state);
            if ((new_state->a == A) && (new_state->b == B)) {
               for (int i = 0; i < a+1; i ++) {
                delete[] graph[i];
                    }
                delete[] graph; 
                solution = true;
                print(new_state);
                break;
            }
            if (!graph[new_state->a][new_state->b]) {
                q.push(new_state);
                graph[new_state->a][new_state->b] = true;
            }
        }
        if (bpour(curr->c, curr->b, b)) {
            vector<int> vec = pour(curr->c, curr->b, b);
            string dir;
            if (vec[2] > 1) {
                dir = "Pour " + to_string(vec[2]) + " gallons from C to B. ";
            }
            else {
                dir = "Pour " + to_string(vec[2]) + " gallon from C to B. "; 
            }
            State* new_state = new State(c -(vec[0]+vec[1]), vec[1], vec[0], dir);
            new_state->parent = curr;
            global.push(new_state);
            if ((new_state->a == A) && (new_state->b == B)) {
                for (int i = 0; i < a+1; i ++) {
                delete[] graph[i];
                    }
                delete[] graph; 
                solution = true;
                print(new_state);
                break;
            }
            if (!graph[new_state->a][new_state->b]) {
                q.push(new_state);
                graph[new_state->a][new_state->b] = true;
            }
        }
         if (bpour(curr->a, curr->b, b)) {
            vector<int> vec = pour(curr->a, curr->b, b);
            string dir;
            if (vec[2] > 1) {
                dir = "Pour " + to_string(vec[2]) + " gallons from A to B. ";
            }
            else {
                dir = "Pour " + to_string(vec[2]) + " gallon from A to B. "; 
            }
            State* new_state = new State(vec[0], vec[1], c - (vec[0]+vec[1]), dir);
            new_state->parent = curr;
            global.push(new_state);
            if ((new_state->a == A) && (new_state->b == B)) {
                for (int i = 0; i < a+1; i ++) {
                delete[] graph[i];
                    }
                delete[] graph; 
                solution = true;
                print(new_state);
                break;
            }
            if (!graph[new_state->a][new_state->b]) {
                q.push(new_state);
                graph[new_state->a][new_state->b] = true;
            }
        }
       if (bpour(curr->b, curr->c, c)) {
            vector<int> vec = pour(curr->b, curr->c, c);
            string dir;
            if (vec[2] > 1) {
                dir = "Pour " + to_string(vec[2]) + " gallons from B to C. ";
            }
            else {
                dir = "Pour " + to_string(vec[2]) + " gallon from B to C. "; 
            }
            State* new_state = new State(c - (vec[1]+vec[0]), vec[0], vec[1], dir);
            new_state->parent = curr;
            global.push(new_state);
            if ((new_state->a == A) && (new_state->b == B)) {
               for (int i = 0; i < a+1; i ++) {
                delete[] graph[i];
                    }
                delete[] graph; 
                solution = true;
                print(new_state);
                break;
            }
            if (!graph[new_state->a][new_state->b]) {
                q.push(new_state);
                graph[new_state->a][new_state->b] = true;
            }
        }
       if (bpour(curr->a, curr->c, c)) {
            vector<int> vec = pour(curr->a, curr->c, c);
            string dir;
            if (vec[2] > 1) {
                dir = "Pour " + to_string(vec[2]) + " gallons from A to C. ";
            }
            else {
                dir = "Pour " + to_string(vec[2]) + " gallon from A to C. "; 
            }
            State* new_state = new State(vec[0], c - (vec[1]+vec[0]), vec[1], dir);
            new_state->parent = curr;
            global.push(new_state);
            if ((new_state->a == A) && (new_state->b == B)) {
                for (int i = 0; i < a+1; i ++) {
                delete[] graph[i];
                    }
                delete[] graph; 
                solution = true;
                print(new_state);
                break;
            }
            if (!graph[new_state->a][new_state->b]) {
                q.push(new_state);
                graph[new_state->a][new_state->b] = true;
            }
        }
        q.pop(); // remove that element from q
    }
    //prints no solution if an answer wasn't found
    if (!solution) {
        for (int i = 0; i < a+1; i ++) {
                delete[] graph[i];
                    }
                delete[] graph; 
    cout << "No solution.";
    }
} 
//print bfs(0, 0, 8)

int main(int argc, char * const argv[]) {
    int total = 0; // total goal
    vector<int> vec;
    if (argc != 7) {
        cerr << "Usage: " << argv[0] << " <cap A> <cap B> <cap C> <goal A> <goal B> <goal C>" << endl;
        return 1;
    }
    istringstream iss;
    int x;
    char jugNum;
    for (int i = 1; i < argc; i++ ) {
        iss.str(argv[i]);
        if (!(iss >> x) || (x < 0) || (i <= 3 && x == 0)) {
            string s;
            s = (i <= 3) ? "capacity" : "goal"; // if i less than 3 prints capacity, else prints goal
            if (i % 3 == 1) {
                jugNum = 'A';
            }
            else if (i % 3 == 2) {
                jugNum = 'B';
            }
            else {
                jugNum = 'C';
            }
            cerr << "Error: Invalid " << s << " \'" << argv[i] << "\' for jug " << jugNum << "." << endl;
            return 1;
        }
        vec.push_back(x);
        if (i > 3) {
            total += vec[i-1];
        }
        iss.clear();
    }

    /* Compare capacity with goal individually */
    if (vec[3] > vec[0]) {
        cerr << "Error: Goal cannot exceed capacity of jug A." << endl;
        return 1;
    }
    if (vec[4] > vec[1]) {
        cerr << "Error: Goal cannot exceed capacity of jug B." << endl;
        return 1;
    }
    if (vec[5] > vec[2]) {
        cerr << "Error: Goal cannot exceed capacity of jug C." << endl;
        return 1;
    }

    /* Make sure capacity C is equal to the total of goal_state */
    if (vec[2] != total) {
        cerr << "Error: Total gallons in goal state must be equal to the capacity of jug C." << endl;
        return 1;
    }
    
    //State cap(vec[0], vec[1], vec[2], ""); // capacity state object called cap
    //State* targ = new State(vec[3], vec[4], vec[5], ""); // goal state object called targ
    bfs(vec[0], vec[1], vec[2], vec[3], vec[4]);
    // State s(0, 0, 8, "Initial state.");
    // cout << s.to_string() << endl;
    // s.a += 3;
    // s.c -= 3;
    // cout << s.to_string() << endl;
    while (!global.empty()) {
        delete global.top();
        global.pop(); 
    }
    return 0;
}