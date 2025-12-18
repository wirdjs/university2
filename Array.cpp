#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

class Array
{
    int *arr;
    int len;

public:
    Array(int l = 1, int mode = 1, int range = 10);
    Array(int *data, int l);
    Array(const Array &oth);
    ~Array();
    Array& operator=(const Array &oth);
    int &operator[](int index);
    bool isSorted();
    bool operator==(Array &oth);
    void Shell_sort();
    void Heapsort();
    void Hoar_sort();
    void Bit_sort();
    friend istream &operator>>(istream &, Array &);
    friend ostream &operator<<(ostream &, const Array &);
};

Array::Array(int l, int mode, int range)
{
    if (l > 0 && (mode == 1 || mode == 2 || mode == 3) && range > 0)
    {
        len = l;
        arr = new int[len];
        if (mode == 1)
        {
            for (int i = 0; i < len; i++)
                arr[i] = rand() % range;
        }
        else if (mode == 2)
        {
            arr[0] = rand() % (range);
            for (int i = 1; i < len; i++)
                arr[i] = arr[i - 1] + rand() % range;
        }
        else if (mode == 3)
        {
            arr[len - 1] = rand() % (range );
            for (int i = len - 2; i >= 0; i--)
                arr[i] = arr[i + 1] + rand() % (range);
        }
    }
    else
    {
        len = 0;
        arr = NULL;
        throw "Error: Invalid parameters";
    }
}

Array::Array(int *data, int l)
{
    if (l > 0)
    {
        len = l;
        arr = new int[len];
        for (int i = 0; i < len; i++)
            arr[i] = data[i]; 
    }
    else
    {
        len = 0;
        arr = NULL;
    }
}

Array::Array(const Array &oth)
{
    len = oth.len;
    arr = new int[len];
    for (int i = 0; i < len; i++)
        arr[i] = oth.arr[i];
}

Array::~Array()
{
    if (arr)
        delete[] arr;
    arr = NULL;
}

Array& Array::operator=(const Array &oth)
{
    if (this != &oth)
    {
        if (arr)
            delete[] arr;
        len = oth.len;
        arr = new int[len];
        for (int i = 0; i < len; i++)
            arr[i] = oth.arr[i];
    }
    return *this;
}

int &Array::operator[](int index)
{
    if (index < 0 || index >= len)
    {
        cout << "Error: index is out of bounds" << endl;
        exit(1);
    }
    return arr[index];
}

bool Array::operator==(Array &oth)
{
    if (len != oth.len)
        return false;
    for (int i = 0; i < len; i++)
        if (arr[i] != oth.arr[i])
            return false;
    return true;
}

bool Array::isSorted()
{
    for (int i = 0; i < len - 1; i++)
        if (arr[i] > arr[i + 1])
            return false;
    return true;
}

ostream& operator<<(ostream &os, const Array &a)
{
    if (a.len == 0)
    {
        os << "[]";
        return os;
    }
    os << "[";
    for (int i = 0; i < a.len; ++i)
    {
        os << a.arr[i];
        if (i + 1 < a.len)
            os << ", ";
    }
    os << "]";
    return os;
}

istream &operator>>(istream &in, Array &obj)
{
    int len;
    cout << "len: ";
    in >> len;
    if (in.fail())
        return in;
    cout << "\narray: ";
    int *arr = new int[len];
    for (int i = 0; i < len; i++)
    {
        in >> arr[i];
        if (in.fail())
        {
            delete[] arr;
            return in;
        }
    }
    obj = Array(arr, len);
    delete[] arr;
    return in;
}

void Array::Shell_sort()
{
    int size = len;
    for (int step = size / 2; step > 0; step /= 2)
    {
        for (int i = step; i < size; i++)
        {
            int temp = arr[i];
            int j;
            for (j = i; j >= step && arr[j - step] > temp; j -= step)
                arr[j] = arr[j - step];
            arr[j] = temp;
        }
    }
}

int main()
{
    srand(time(0)); 
   
    Array a(10, 1, 20);
    cout << "a = " << a << endl;
    a.Shell_sort();
    cout << "Sorted?: " << a.isSorted() << endl;
    cout << "a = " << a << endl;
    return 0;
}
