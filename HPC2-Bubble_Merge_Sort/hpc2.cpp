/*
Problem Statement :-
Write a program to implement Parallel Bubble Sort and Merge sort using OpenMP. Use
existing algorithms and measure the performance of sequential and parallel algorithms.
*/

/* Code */
#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include <omp.h>

using namespace std;
using namespace std::chrono;

void sequentialBubbleSort(vector<int> &v)
{
    int n=v.size();

    for(int i=0;i<n-1;i++)
    {
        for(int j=0;j<n-i-1;j++)
        {
            if(v[j]>v[j+1])
            {
                swap(v[j],v[j+1]);
            }
        }
    }
}

void parallelBubbleSort(vector<int> &v)
{
    int n=v.size();

    for(int i=0;i<n;i++)
    {
        if(i%2==0)
        {
            #pragma omp parallel for
            for(int j=0;j<n-1;j+=2)
            {
                if(v[j]>v[j+1])
                {
                    swap(v[j],v[j+1]);
                }
            }
        }
        else
        {
            #pragma omp parallel for
            for(int j=1;j<n-1;j+=2)
            {
                if(v[j]>v[j+1])
                {
                    swap(v[j],v[j+1]);
                }
            }
        }
    }
}


void merge(vector<int> &v,int low,int mid,int high)
{
    vector<int> temp;

    int left=low;
    int right=mid+1;

    while(left<=mid && right<=high)
    {
        if(v[left]<=v[right])
        {
            temp.push_back(v[left]);
            left++;
        }
        else
        {
            temp.push_back(v[right]);
            right++;
        }
    }

    while(left<=mid)
    {
        temp.push_back(v[left]);
        left++;
    }

    while(right<=high)
    {
        temp.push_back(v[right]);
        right++;
    }

    for(int i=low;i<=high;i++)
    {
        v[i]=temp[i-low];
    }
}

void sequentialMergeSort(vector<int> &v,int low, int high)
{
    if(low>=high)
    {
        return;
    }

    int mid=(low+high)/2;
    sequentialMergeSort(v,low,mid);
    sequentialMergeSort(v,mid+1,high);
    merge(v,low,mid,high);
}

void parallelMergeSort(vector<int> &v, int low, int high, int depth=0)
{
    if(low>=high)
    {
        return;
    }

    int mid=(low+high)/2;

    if(depth<=3)
    {
        #pragma omp parallel sections
        {
            #pragma omp section
            {
                parallelMergeSort(v,low,mid,depth+1);
            }

            #pragma omp section
            {
                parallelMergeSort(v,mid+1,high,depth+1);
            }
        }
    }
    else
    {
        sequentialMergeSort(v,low,mid);
        sequentialMergeSort(v,mid+1,high);
    }

    merge(v,low,mid,high);
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



int main()
{
    int size=10000;
    vector<int> temp;

    // vector<int> v={10,9,8,7,6,5,4,3,2,1,0};

    vector<int> v(size);
    for(int i=0;i<size;i++)
    {
        v[i]=size-i;
    }

    temp=v;
    auto start=high_resolution_clock::now();
    sequentialBubbleSort(temp);
    auto end=high_resolution_clock::now();
    cout<<"Sequential Bubble Sort Time: "<<duration_cast<milliseconds>(end-start).count()<<" ms\n";
    cout<<"Sequential Bubble Sort Array: "<<endl;
    print(temp);
    cout<<endl;
    cout<<endl;

    temp=v;
    start=high_resolution_clock::now();
    parallelBubbleSort(temp);
    end=high_resolution_clock::now();
    cout<<"Parallel Bubble Sort Time: "<<duration_cast<milliseconds>(end-start).count()<<" ms\n";
    cout<<"Parallel Bubble Sort Array: "<<endl;
    print(temp);
    cout<<endl;
    cout<<endl;

    temp=v;
    start=high_resolution_clock::now();
    sequentialMergeSort(temp,0,temp.size()-1);
    end=high_resolution_clock::now();
    cout<<"Sequential Merge Sort Time: "<<duration_cast<milliseconds>(end-start).count()<<" ms\n";
    cout<<"Sequential Merge Sort Array: "<<endl;
    print(temp);
    cout<<endl;
    cout<<endl;

    temp=v;
    start=high_resolution_clock::now();
    parallelMergeSort(temp,0,temp.size()-1);
    end=high_resolution_clock::now();
    cout<<"Parallel Merge Sort Time: "<<duration_cast<milliseconds>(end-start).count()<<" ms\n";
    cout<<"Parallel Merge Sort Array: "<<endl;
    print(temp);
    cout<<endl;
    cout<<endl;

    return 0;
}





/*
Output-1:-
g++-14 -fopenmp -o output2 hpc2.cpp
./output2                       
Sequential Bubble Sort Time: 0 ms
Sequential Bubble Sort Array: 
First 11 elements of Array: 
0 1 2 3 4 5 6 7 8 9 10 

Parallel Bubble Sort Time: 0 ms
Parallel Bubble Sort Array: 
First 11 elements of Array: 
0 1 2 3 4 5 6 7 8 9 10 

Sequential Merge Sort Time: 0 ms
Sequential Merge Sort Array: 
First 11 elements of Array: 
0 1 2 3 4 5 6 7 8 9 10 

Parallel Merge Sort Time: 0 ms
Parallel Merge Sort Array: 
First 11 elements of Array: 
0 1 2 3 4 5 6 7 8 9 10 









Output-2:-
g++-14 -fopenmp -o output2 hpc2.cpp
./output2                       
Sequential Bubble Sort Time: 432 ms
Sequential Bubble Sort Array: 
First 50 elements of Array: 
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 

Parallel Bubble Sort Time: 587 ms
Parallel Bubble Sort Array: 
First 50 elements of Array: 
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 

Sequential Merge Sort Time: 4 ms
Sequential Merge Sort Array: 
First 50 elements of Array: 
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 

Parallel Merge Sort Time: 2 ms
Parallel Merge Sort Array: 
First 50 elements of Array: 
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 
*/



/*
Performance Analysis :-
|   Algorithm                              |   Time Complexity                                            |   Space Complexity   |   Reason                                                                                                         |
| ---------------------------------------- | ------------------------------------------------------------ | -------------------- | ---------------------------------------------------------------------------------------------------------------- |
|   Sequential Bubble Sort                 | O(n²)                                                        | O(1)                 | Two nested loops compare & swap adjacent elements; constant space in-place sort.                                 |
|   Parallel Bubble Sort (Odd-Even Sort)   | O(n²) (total work)    Better wall-time with multiple threads | O(1)                 | Parallelizes comparisons at each phase, but still O(n) phases; limited speedup due to dependency between phases. |
|   Sequential Merge Sort                  | O(n log n)                                                   | O(n)                 | Divides array log n times and merges in O(n); auxiliary array used for merging.                                  |
|   Parallel Merge Sort                    | O(n log n) (total work)    O(log n) parallel depth           | O(n)                 | Parallelizes recursive splits using OpenMP sections; merges still require O(n) extra space.                      |
*/