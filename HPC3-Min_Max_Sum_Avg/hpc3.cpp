/*
Problem Statement :-
Implement Min, Max, Sum and Average operations using Parallel Reduction.
Measure the performance of sequential and parallel algorithms.
*/

/* Code */
#include<numeric>
#include<climits>
#include<iostream>
#include<vector>
#include<algorithm>
#include<omp.h>
#include<chrono>

using namespace std;
using namespace std::chrono;

void sequential_min(vector<int> &v,int &min_value)
{
    min_value= *min_element(v.begin(),v.end());
}

void sequential_max(vector<int> &v,int &max_value)
{
    max_value= *max_element(v.begin(),v.end());
}

long long sequential_sum(vector<int> &v)
{
    long long sum=0;
    sum=accumulate(v.begin(),v.end(),0ll);

    return sum;
}

void sequential_avg(vector<int> &v,double &avg)
{
    long long sum=sequential_sum(v);
    avg=double(sum)/v.size();
}

void parallel_min(vector<int> &v,int &min_value)
{
    min_value=INT_MAX;

    #pragma omp parallel for reduction(min:min_value)
    for(int i=0;i<v.size();i++)
    {
        min_value=min(min_value,v[i]);
    }
}

void parallel_max(vector<int> &v,int &max_value)
{
    max_value=INT_MIN;

    #pragma omp parallel for reduction(max:max_value)
    for(int i=0;i<v.size();i++)
    {
        max_value=max(max_value,v[i]);
    }
}

long long parallel_sum(vector<int> &v)
{
    long long sum=0;
    #pragma omp parallel for reduction(+:sum)
    for(int i=0;i<v.size();i++)
    {
        sum+=v[i];
    }

    return sum;
}

void parallel_avg(vector<int> &v,double &avg)
{
    long long sum=parallel_sum(v);
    avg=double(sum)/v.size();
}

int main()
{
    // vector<int> v={10,9,8,7,6,5,4,3,2,1};

    int size=1000000;
    vector<int>v(size);
    for(int i=0;i<size;i++)
    {
        v[i]=size-i;
    }

    int min_value, max_value;
    long long sum;
    double avg;

    cout<<"--------------------Sequential Algorithms-----------------------"<<endl;

    auto start=high_resolution_clock::now();
    sequential_min(v,min_value);
    auto end=high_resolution_clock::now();
    auto time_min=duration_cast<milliseconds>(end-start).count();

    start=high_resolution_clock::now();
    sequential_max(v,max_value);
    end=high_resolution_clock::now();
    auto time_max=duration_cast<milliseconds>(end-start).count();

    start=high_resolution_clock::now();
    sum=sequential_sum(v);
    end=high_resolution_clock::now();
    auto time_sum=duration_cast<milliseconds>(end-start).count();

    start=high_resolution_clock::now();
    sequential_avg(v,avg);
    end=high_resolution_clock::now();
    auto time_avg=duration_cast<milliseconds>(end-start).count();

    cout<<"Sequential Minimum: "<<min_value<<"\t Time: "<<time_min<<" ms"<<endl;
    cout<<"Sequential Maximum: "<<max_value<<"\t Time: "<<time_max<<" ms"<<endl;
    cout<<"Sequential Sum: "<<sum<<"\t Time: "<<time_sum<<" ms"<<endl;
    cout<<"Sequential Average: "<<avg<<"\t Time: "<<time_avg<<" ms"<<endl;
    cout<<"----------------------------------------------------------------"<<endl;
    




    min_value=INT_MAX;
    max_value=INT_MIN;
    sum=0;
    avg=0.0;




    cout<<"--------------------Parallel Algorithms-------------------------"<<endl;

    start=high_resolution_clock::now();
    parallel_min(v,min_value);
    end=high_resolution_clock::now();
    time_min=duration_cast<milliseconds>(end-start).count();

    start=high_resolution_clock::now();
    parallel_max(v,max_value);
    end=high_resolution_clock::now();
    time_max=duration_cast<milliseconds>(end-start).count();

    start=high_resolution_clock::now();
    sum=parallel_sum(v);
    end=high_resolution_clock::now();
    time_sum=duration_cast<milliseconds>(end-start).count();

    start=high_resolution_clock::now();
    parallel_avg(v,avg);
    end=high_resolution_clock::now();
    time_avg=duration_cast<milliseconds>(end-start).count();

    cout<<"Parallel Minimum: "<<min_value<<"\t Time: "<<time_min<<" ms"<<endl;
    cout<<"Parallel Maximum: "<<max_value<<"\t Time: "<<time_max<<" ms"<<endl;
    cout<<"Parallel Sum: "<<sum<<"\t Time: "<<time_sum<<" ms"<<endl;
    cout<<"Parallel Average: "<<avg<<"\t Time: "<<time_avg<<" ms"<<endl;
    cout<<"----------------------------------------------------------------"<<endl;

    return 0;
}




/*
Output-1 :-
g++-14 -fopenmp -o output3 hpc3.cpp
./output3                       
--------------------Sequential Algorithms-----------------------
Sequential Minimum: 1    Time: 0 ms
Sequential Maximum: 10   Time: 0 ms
Sequential Sum: 55       Time: 0 ms
Sequential Average: 5.5  Time: 0 ms
----------------------------------------------------------------
--------------------Parallel Algorithms-------------------------
Parallel Minimum: 1      Time: 0 ms
Parallel Maximum: 10     Time: 0 ms
Parallel Sum: 55         Time: 0 ms
Parallel Average: 5.5    Time: 0 ms
----------------------------------------------------------------








Output-2 :-
g++-14 -fopenmp -o output3 hpc3.cpp
./output3                       
--------------------Sequential Algorithms-----------------------
Sequential Minimum: 1    Time: 13 ms
Sequential Maximum: 1000000      Time: 11 ms
Sequential Sum: 500000500000     Time: 7 ms
Sequential Average: 500000       Time: 6 ms
----------------------------------------------------------------
--------------------Parallel Algorithms-------------------------
Parallel Minimum: 1      Time: 0 ms
Parallel Maximum: 1000000        Time: 0 ms
Parallel Sum: 500000500000       Time: 0 ms
Parallel Average: 500000         Time: 0 ms
----------------------------------------------------------------
*/


/*
Performance Analysis:-
| Algorithm          | Time Complexity                    | Space Complexity | Reason                                                             |
| ------------------ | ---------------------------------- | ---------------- | ------------------------------------------------------------------ |
| Sequential Minimum | O(n)                               | O(1)             | Scans entire array once to find min; in-place, constant space.     |
| Parallel Minimum   | O(n/p) per thread, O(n) total work | O(1)             | Parallel reduction divides work among threads; min reduced at end. |
| Sequential Maximum | O(n)                               | O(1)             | Scans entire array once to find max; constant space.               |
| Parallel Maximum   | O(n/p) per thread, O(n) total work | O(1)             | Similar to parallel min; parallel reduction to get max.            |
| Sequential Sum     | O(n)                               | O(1)             | Traverses array once and accumulates sum; constant space.          |
| Parallel Sum       | O(n/p) per thread, O(n) total work | O(1)             | Parallel reduction adds chunks of sum; combines at end.            |
| Sequential Average | O(n)                               | O(1)             | Computes sum (O(n)) and divides by n; constant space.              |
| Parallel Average   | O(n/p) per thread, O(n) total work | O(1)             | Uses parallel sum (O(n)) and divides by n; constant space.         |
*/