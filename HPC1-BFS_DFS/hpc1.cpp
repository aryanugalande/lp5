/*
Problem Statement :-
Design and implement Parallel Breadth First Search and Depth First Search based on
existing algorithms using OpenMP. Use a Tree or an undirected graph for BFS and DFS .
Measure the performance of sequential and parallel algorithms.
*/

/* Code */
#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <queue>
#include <chrono>
#include <omp.h>

using namespace std;
using namespace std::chrono;

class graph
{
    int v;
    vector<vector<int>> adj;

    public:

    graph(int v)
    {
        this->v=v;
        adj.resize(v);
    }

    void addEdge(int u,int v)
    {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void sequentialBFS(int start, vector<int> &sbfsSequence)
    {
        vector<bool> visited(v,false);
        queue<int> q;

        q.push(start);
        visited[start]=true;

        while(!q.empty())
        {
            int node=q.front();
            q.pop();
            sbfsSequence.push_back(node);

            for(auto neighbor:adj[node])
            {
                if(!visited[neighbor])
                {
                    q.push(neighbor);
                    visited[neighbor]=true;
                }
            }
        }
    }

    void parallelBFS(int start, vector<int> &pbfsSequence)
    {
        vector<bool> visited(v,false);
        queue<int> q;

        q.push(start);
        visited[start]=true;

        while(!q.empty())
        {
            int size=q.size();
            vector<int> current_level;

            for(int i=0;i<size;i++)
            {  
                int node=q.front();
                q.pop();
                pbfsSequence.push_back(node);
                current_level.push_back(node);
            }

            #pragma omp parallel for
            for(int i=0;i<current_level.size();i++)
            {
                int node=current_level[i];
                for(auto neighbor : adj[node])
                {
                    if(!visited[neighbor])
                    {
                        #pragma omp critical
                        {
                            if(!visited[neighbor])
                            {
                                q.push(neighbor);
                                visited[neighbor]=true;
                            }
                        }
                    }
                }
            }
        }
    }

    void sequentialDFS(int start, vector<int> &sdfsSequence)
    {
        vector<bool> visited(v,false);
        stack<int> s;

        s.push(start);
        visited[start]=true;

        while(!s.empty())
        {
            int node=s.top();
            s.pop();
            sdfsSequence.push_back(node);

            for(auto neighbor:adj[node])
            {
                if(!visited[neighbor])
                {
                    s.push(neighbor);
                    visited[neighbor]=true;
                }
            }                      

            // for(auto it=adj[node].rbegin();it!=adj[node].rend();it++)
            // {
            //     if(!visited[*it])
            //     {
            //         s.push(*it);
            //         visited[*it]=true;
            //     }
            // }
        }
    }

    void parallelDFS(int start, vector<int> &pdfsSequence)
    {
        vector<bool> visited(v,false);
        stack<int> s;

        s.push(start);
        visited[start]=true;

        while(!s.empty())
        {
            int node=s.top();
            s.pop();
            pdfsSequence.push_back(node);

            #pragma omp parallel for
            for(auto neighbor:adj[node])
            {
                if(!visited[neighbor])
                {
                    #pragma omp critical
                    {
                        if(!visited[neighbor])
                        {
                            s.push(neighbor);
                            visited[neighbor]=true;
                        }
                    }
                }
            }
        }
    }

    void print(vector<int> &v)
    {
        int limit=min((int)v.size(),50);

        cout<<"First "<<limit<<" elements of Array: \n";

        for(int i=0;i< limit;i++)
        {
            cout<<v[i]<<" ";
        }
    }
};

int main()
{
    int v=100000;
    graph g(v);
    for(int i=0;i<v;i++)
    {
        g.addEdge(i,i+1);

        if(i%100==0 && i+100<v)
        {
            g.addEdge(i,i+100);
        }
    }

    // int v=7;
    // graph g(v);
    // g.addEdge(0,1);
    // g.addEdge(0,2);
    // g.addEdge(1,3);
    // g.addEdge(1,4);
    // g.addEdge(2,5);
    // g.addEdge(2,6);

    vector<int> sbfsSequence;
    vector<int> pbfsSequence;
    vector<int> sdfsSequence;
    vector<int> pdfsSequence;


    auto start=high_resolution_clock::now();
    g.sequentialBFS(0,sbfsSequence);
    auto end=high_resolution_clock::now();
    cout<<"Sequential BFS Time: "<<duration_cast<milliseconds>(end-start).count()<<" ms \n";
    cout<<"Sequential BFS Traversal: "<<endl;
    g.print(sbfsSequence);
    cout<<endl;

    cout<<endl;

    start=high_resolution_clock::now();
    g.parallelBFS(0,pbfsSequence);
    end=high_resolution_clock::now();
    cout<<"Parallel BFS Time: "<<duration_cast<milliseconds>(end-start).count()<<" ms \n";
    cout<<"Parallel BFS Traversal: "<<endl;
    g.print(pbfsSequence);
    cout<<endl;

    cout<<endl;

    start=high_resolution_clock::now();
    g.sequentialDFS(0,sdfsSequence);
    end=high_resolution_clock::now();
    cout<<"Sequential DFS Time: "<<duration_cast<milliseconds>(end-start).count()<<" ms \n";
    cout<<"Sequential DFS Traversal: "<<endl;
    g.print(sdfsSequence);
    cout<<endl;

    cout<<endl;

    start=high_resolution_clock::now();
    g.parallelDFS(0,pdfsSequence);
    end=high_resolution_clock::now();
    cout<<"Parallel DFS Time: "<<duration_cast<milliseconds>(end-start).count()<<" ms \n";
    cout<<"Parallel DFS Traversal: "<<endl;
    g.print(pbfsSequence);
    cout<<endl;


    return 0;
}


/*
Output 1:-
g++-14 -fopenmp -o output1 hpc1.cpp
./output1                          
Sequential BFS Time: 0 ms 
Sequential BFS Traversal: 
First 7 elements of Array: 
0 1 2 3 4 5 6 

Parallel BFS Time: 0 ms 
Parallel BFS Traversal: 
First 7 elements of Array: 
0 1 2 5 6 3 4 

Sequential DFS Time: 0 ms 
Sequential DFS Traversal: 
First 7 elements of Array: 
0 2 6 5 1 4 3 

Parallel DFS Time: 0 ms 
Parallel DFS Traversal: 
First 7 elements of Array: 
0 1 2 5 6 3 4 









Output 2:-
g++-14 -fopenmp -o output1 hpc1.cpp
./output1                          
Sequential BFS Time: 17 ms 
Sequential BFS Traversal: 
First 50 elements of Array: 
0 1 100 2 99 101 200 3 98 102 199 201 300 4 97 103 198 202 299 301 400 5 96 104 197 203 298 302 399 401 500 6 95 105 196 204 297 303 398 402 499 501 600 7 94 106 195 205 296 304 

Parallel BFS Time: 74 ms 
Parallel BFS Traversal: 
First 50 elements of Array: 
0 1 100 99 101 200 2 199 201 300 102 3 98 299 301 400 202 103 4 97 198 399 401 500 203 302 104 96 197 5 298 303 499 501 600 204 95 6 105 297 196 398 402 205 94 502 599 601 700 296 

Sequential DFS Time: 11 ms 
Sequential DFS Traversal: 
First 50 elements of Array: 
0 100 200 300 400 500 600 700 800 900 1000 1100 1200 1300 1400 1500 1600 1700 1800 1900 2000 2100 2200 2300 2400 2500 2600 2700 2800 2900 3000 3100 3200 3300 3400 3500 3600 3700 3800 3900 4000 4100 4200 4300 4400 4500 4600 4700 4800 4900 

Parallel DFS Time: 3686 ms 
Parallel DFS Traversal: 
First 50 elements of Array: 
0 1 100 99 101 200 2 199 201 300 102 3 98 299 301 400 202 103 4 97 198 399 401 500 203 302 104 96 197 5 298 303 499 501 600 204 95 6 105 297 196 398 402 205 94 502 599 601 700 296 
*/


/*
Performance Analysis:-
| Algorithm          | Time Complexity                                                   | Space Complexity     | Reason                                                                                                 |
| ------------------ | ----------------------------------------------------------------- | -------------------- | ------------------------------------------------------------------------------------------------------ |
|   Sequential BFS   | O(V + E)                                                          | O(V)                 | Visits each node and edge once using a queue.                                                          |
|   Parallel BFS     | O(V + E) (total work)    Parallel depth ≈ O(log V) (best case)    | O(V)                 | Parallelizes nodes at the same level; efficient if many neighbors (wide frontier).                     |
|   Sequential DFS   | O(V + E)                                                          | O(V)                 | Visits each node and edge once using a stack.                                                          |
|   Parallel DFS     | O(V + E) (total work)    High overhead, low practical parallelism | O(V)                 | DFS is inherently sequential (deep exploration); parallelism limited + critical sections slow it down. |
*/




