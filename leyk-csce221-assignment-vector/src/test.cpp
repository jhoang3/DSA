#include <iostream>
#include <vector>
#include <ctime>
#include <bits/stdc++.h>
#include <cmath>

using namespace std;

int num_comp = 0;

bool n_square(vector<int>::iterator first, vector<int>::iterator last)
{
    // O(n^2) algorithm, return true if a pair % 12 == 0
    if (first == last || first + 1 == last)
        return false;
    for (auto i = first; i != last - 1; i++) // n times
    {
        for (auto j = i + 1; j != last; j++) // n-i times
        {
            if (*i == 0 || *j == 0)
                continue;
            if ((*i * *j) % 12 == 0)
            {
                return true;
            }
        }
    }
    return false;
}

bool n(vector<int>::iterator first, vector<int>::iterator last)
{
    // O(n) algorithm, return true if a pair % 12 == 0
    // for something to be that, one must have at least two factors of 2 and one factor of 3
    // or we must have a num that has one factor of 2 and one factor of 3 and another num that has one of 2
    bool twoFac2 = false;
    bool oneFac3And2 = false;
    bool otherOneFac3And2 = false;
    bool oneFac2 = false;
    bool oneFac3 = false;
    bool oneFac12 = false;

    if (first == last || first + 1 == last)
        return false;
    for (auto i = first; i != last; i++)
    {

        if (*i == 0)
            continue;
        if (*i % 12 == 0)
            oneFac12 = true;
        else if (*i % 2 == 0 && *i / 2 != 0 && (*i / 2) % 2 == 0)
            twoFac2 = true;
        else if (*i % 2 == 0 && *i % 3 == 0)
        {
            if (oneFac3And2)
                otherOneFac3And2 = true;
            else
                oneFac3And2 = true;
        }
        else if (*i % 2 == 0)
            oneFac2 = true;
        else if (*i % 3 == 0)
            oneFac3 = true;
    }
    if (oneFac12 || (twoFac2 && (oneFac3 || oneFac3And2)) || (oneFac2 && oneFac3And2) || (oneFac3And2 && otherOneFac3And2))
        return true;

    return false;
}

template <typename T>
T Binary_Search(vector<T> &v, T x)
{
    int mid, low = 0;
    int high = (int)v.size() - 1;
    while (low < high)
    {
        mid = (low + high) / 2;
        if (num_comp++, v[mid] < x)
            low = mid + 1;
        else
            high = mid;
    }
    if (num_comp++, x == v[low])
        return low; // OK: found
    return -1;      // not found
}

int Binary_Search_Int(vector<int> &v, int x)
{
    int mid, low = 0;
    int high = (int)v.size() - 1;
    while (low < high)
    {
        mid = (low + high) / 2;
        if (num_comp++, v[mid] < x)
            low = mid + 1;
        else
            high = mid;
    }
    if (num_comp++, x == v[low])
        return low; // OK: found
    return -1;      // not found
}

// modify the binary search using a functional object to compare vector elements
template <typename T, typename Comparator>
T Binary_Search_Obj(vector<T> &v, T x, Comparator cmp)
{
    int mid, low = 0;
    int high = (int)v.size() - 1;
    while (low < high)
    {
        mid = (low + high) / 2;
        if (num_comp++, cmp(v[mid], x))
            low = mid + 1;
        else
            high = mid;
    } 
    if (num_comp++, x == v[low])
        return low; // OK: found
    return -1;      // not found
}

class compareElements {
public:
    bool operator()(int a, int b) const {
        return a > b;
    }
};




int main()
{
    srand(time(NULL));

    // create a templated integer vector with random integers in it
    vector<int> vec = {};

    for (int i = 0; i < pow(2.0,11); i++)
    {
        vec.push_back(i+1);
        cout << vec[i] << " ";
    }
    // bool result = n_square(intVec.begin(), intVec.end());
    // cout << endl
    //      << result << endl;
    // result = n(intVec.begin(), intVec.end());
    // cout << result << endl;
    cout << endl;
    // sort descending
    sort(vec.begin(), vec.end(), greater<int>());
    for (long unsigned int i = 0; i < pow(2.0,11); i++)
    {
        cout << vec[i] << " ";
    }
    // firstIndex + (lastIndex-firstIndex)/2
    cout << endl
         << Binary_Search_Obj(vec, 1, compareElements()) << endl;
    cout << num_comp << endl;
}