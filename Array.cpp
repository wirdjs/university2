#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
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
    int &operator[](int index) const;
    int getSize(){
        return len;
    }
    bool isSorted();
    bool operator==(Array oth);
    void Shell_sort();
    void Heapsort();
    void Heapify(int i, int n);
    void Hoar_sort();
    void BitSort(int a[], int left, int right, int k);
    void Bit_Sort();
    friend istream &operator>>(istream &in, Array &obj);
    friend ostream &operator<<(ostream &os, const Array &arr);
};

Array::Array(int l, int mode, int range)
{
    if (l > 0 && (mode == 1 || mode == 2 || mode == 3 || mode == 4) && range > 0)
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
            arr[len - 1] = rand() % (range);
            for (int i = len - 2; i >= 0; i--)
                arr[i] = arr[i + 1] + rand() % (range);
        }
        else if (mode ==4){
            for (int i = 0;i<len;i++){
                cin>>arr[i];
            }
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

int &Array::operator[](int index) const 
{
   if (index < 0 || index >= len)
    {
        cout << "Error: index is out of bounds" << endl;
        exit(1);
    }
    return arr[index];
}

bool Array::operator==(Array oth)
{
    if (len != oth.len) return false;
    
    for (int i = 0; i < oth.len; i++) {
        bool found = false;
        for (int j = 0; j < oth.len; j++) {
            if (arr[i] == oth.arr[j]) {
                found = true;
                oth.arr[j] = oth.arr[oth.len - 1];
                oth.len--;
                break;  
            }           
        }
        if (!found) return false;
    }
    return true;
}

bool Array::isSorted()
{
    for (int i = 0; i < len - 1; i++)
        if (arr[i] > arr[i + 1])
            return false;
    return true;
}

ostream& operator<<(ostream &os, const Array &arr)
{
   os << "[";
    for (int i = 0; i < arr.len; i++) {
        os << arr[i] << " ";
    }
    os << "]";
    return os;
}

istream &operator>>(istream &in, Array &obj)
{
      int len;
    cout << "len: ";
    in >> len;
    cout << "\n array: ";
    int *arr;
    arr = new int[len]; 
    for (int i = 0; i < len; i++) {
        in >> arr[i];
    }
    obj = Array(arr, len); 
    delete[] arr;
    return in;
}

////////////////////////////////////////////////////
//1)h0 = n/2, hi = hi-1/2,…, ht=1
//2)hm = 2^m -1 при m=1,2,3...,t= log2 (n)
//3)Kнут: h1 =1;h(j+1)= 3h(j)+1 для j = 1, 2, ..., t – 1
//
//4)hj=9*2^j-9*2^(j/2)+1, если четно j; 
//  hj=8*2^j-6*2^(j+1/2)+1,если нечетно j;
// Сэджвик
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

//////////////////////////////////////////////////////////
void Array::Heapsort()
{
    int tmp;
    for (int i = len / 2 - 1; i >= 0; i--)
        Heapify(len, i);
    for (int i = len - 1; i > 0; i--)
    {
        tmp = arr[0];
        arr[0] = arr[i];
        arr[i] = tmp;
        Heapify(i, 0);
    }
}
 
void Array::Heapify(int n, int i)
{
    int j = 2 * i + 1;
    int x = arr[i];
    bool f = true;
 
    while (j < n && f == 1)
    {
        if (j + 1 < n && arr[j + 1] > arr[j]) j++;
        if (arr[j] > x)
        {
            arr[i] = arr[j];
            i = j;
            j = 2 * i + 1;
        }
        else f = false;
    }
    arr[i] = x;
}

////////////////////////////////////////////////////////////

void hoar3(int *array, int left, int right)
{
    if (left >= right) return;

    int i = left;
    int j = right;
    int pivot = array[(left + right) / 2]; 

    while (i <= j)
    {
        while (array[i] < pivot) i++;
        while (array[j] > pivot) j--;
        if (i <= j)
        {
            int t = array[i];
            array[i] = array[j];
            array[j] = t;
            i++;
            j--;
        }
    }

    if (left < j) hoar3(array, left, j);
    if (i < right) hoar3(array, i, right);
}

void Array::Hoar_sort()
{
    if (len <= 1) return;
    hoar3(arr, 0, len - 1);
}

//////////////////////////////////////////////////////////////////////
void  Array::BitSort(int arr[], int left, int right, int k) {
    if (left >= right || k < 0) {
        return;
    }
    int i = left, j = right;
    while (i <= j) {
        while (i <= j && ((arr[i] >> k) & 1) == 0) {
            i++;
        }
        while (i <= j && ((arr[j] >> k) & 1) == 1) {
            j--;
        }
        if (i < j) {
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i++;
            j--;
        }
    }
    // Рекурсивная сортировка подмассива [l, j] по (k-1)-ому биту
    BitSort(arr, left, j, k - 1);
    // Рекурсивная сортировка подмассива [i, r] по (k-1)-ому биту
    BitSort(arr, i, right, k - 1);
}
void Array::Bit_Sort() {
    BitSort(arr, 0, len - 1, sizeof(int) * 8 - 1);
}
void Test(){
    
    using chrono::high_resolution_clock;
    using chrono::duration_cast;
    using chrono::duration;
    using chrono::milliseconds;

    Array array1(100000, 1, 100000);
    Array array2 = array1;
    Array array3 = array1;
    Array array4 = array1;

    auto beg1 = chrono::steady_clock::now();
    array1.Shell_sort();
    auto etime1 = chrono::steady_clock::now();
    auto time1 = chrono::duration_cast<std::chrono::milliseconds>(etime1 - beg1);
    cout << array1.isSorted() << endl;


    auto beg2 = chrono::steady_clock::now();
    array2.Hoar_sort();
    auto etime2 = chrono::steady_clock::now();
    auto time2 = chrono::duration_cast<std::chrono::milliseconds>(etime2 - beg2);
    array2.isSorted();


    auto beg3 = chrono::steady_clock::now();
    array3.Heapsort();
    auto etime3 = std::chrono::steady_clock::now();
    auto time3 = chrono::duration_cast<std::chrono::milliseconds>(etime3 - beg3);
    array3.isSorted();


    auto beg4 = std::chrono::steady_clock::now();
    array4.Bit_Sort();
    auto etime4 = std::chrono::steady_clock::now();
    auto time4 = std::chrono::duration_cast<std::chrono::milliseconds>(etime4 - beg4);
    array4.isSorted();


    cout << endl;

    cout << "The time for Hoar Sort: " << time2.count() << " ms\n";
    cout << array1.isSorted() << endl;
    cout << "The time for Bit Sort: " << time4.count() << " ms\n";
    cout << array2.isSorted() << endl;
    cout << "The time for HeapSort: " << time3.count() << " ms\n";
    cout << array3.isSorted() << endl;
    cout << "The time for Shell Sort: " << time1.count() << " ms\n"; 
    cout << array4.isSorted() << endl;

    cout<< "Do all arrays have the same elements"<<endl;
    cout << "array1 == array2: " << (array1 == array2) << endl;
    cout << "array3 == array2: " << (array3 == array2) << endl;
    cout << "array4 == array2: " << (array4 == array2) << endl;
}


int main()
{
    srand(time(0));

    Array a(7, 1, 20);
    cout << "a = " << a << endl;

    a.Shell_sort();
    cout << "a = " << a << endl;
    cout << "Sorted?: " << a.isSorted() << endl;
    
    Array b(7,1,20); 
    cout << "b = " << b << endl;
    b.Heapsort();
    cout << "b = " << b << endl;
    cout << "Sorted?: " << b.isSorted() << endl;
     
    Array z(7,1,20);
    cout << "z = " << z << endl;   
    z.Hoar_sort();
    cout << "z = " << z << endl;
    cout << "Sorted?: " << z.isSorted() <<endl;

    Array c(7,1,20);
    cout<<"c= "<< c<<endl;
    c.Bit_Sort();
    cout<<"c= "<< c<<endl;
    cout<<"Sorted?: "<<c.isSorted()<<endl;
//    Array x1(5,4,1);
//    cout<< "________"<<endl;
//    Array x2(5,4,1);
//    if (x1 == x2){
//     cout<< "true";
//    }else{cout<<"false";}

    Test();

    return 0;
}
