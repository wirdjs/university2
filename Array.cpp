#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;
class Array
{ int *arr, len;
 public:

Array(int l = 1, int mode = 1, int range = 10);
Array(int *arr, int l);
Array(const Array &oth);
 ~Array(){
    if(arr) delete []arr; arr = NULL;
 }
Array& operator = (const Array &oth); 
int &operator [](int index);

 bool isSorted(); // проверка на упорядоченность по неубыванию
 bool operator == (Array&oth); // функция возвращает true, если массивы состоят из одинаковых
// чисел, при этом их порядок в массивах может различаться

 void Shell_sort();
 void Heapsort();
 void Hoar_sort();
 void Bit_sort();
 friend istream & operator >> (istream &, Array &);
 friend ostream & operator << (ostream &, const Array &);
};
Array::Array(int l = 1, int mode = 1, int range = 10){
     if(l > 0 && (mode ==1 || mode == 2 || mode ==3) && range > 0){
        len = l;
        arr = new int [len];
        
        srand(time(0));
        if (mode ==1){
            for(int i=0; i<len; i++){
                arr[i] = rand() % range;
            }
        }else if (mode == 2){
            arr[0] = rand() % (range -1);
            for(int i=1; i<len; i++){
                arr[i] = arr[i-1] + rand() % (range -1);
            }
        }else if (mode ==3){
            arr[len-1] = rand() % (range +1);
            for (int i = len-2; i >=0 ; i--)
            {
                arr[i] = arr[i+1] - rand() % (range +1);
            }
    }
    else{
        len = 0;
        arr = {0};
        cout<< "Error: len isn't be a negative or wrong mode"<<endl;
    }
}
}
Array:: Array(int *arr, int l){
     if(l>0){
        srand(time(0));
        len = l;
        arr = new int [len];

        for(int i=0; i<len; i++){
            
            arr[i] = rand() % 100;
        }
    }else{
        len = 0;
        arr = {0};
        cout<< "Error: len isn't be a negative"<<endl;
    }
}
Array::Array(const Array &oth){
        len = oth.len;
    arr = new int [len];
    for(int i=0; i<len; i++){
        arr[i] = oth.arr[i];
    }
}
Array& Array:: operator = (const Array &oth){
    if(this != &oth){
        if(arr) delete []arr;
        len = oth.len;
        arr = new int [len];
        for(int i=0; i<len; i++){
            arr[i] = oth.arr[i];
        }
    }
    return *this;
}
bool Array::operator==(Array&oth){
     if (len != oth.len){
        return false;
    }
    int l = len;
    for (int i =0 ; i < len ; i++){
        bool check = false;
        for (int j =0 ; j < l ; j++){
            if (arr[i] == oth.arr[j]){
                check = true;
                oth.arr[j] = oth.arr[l-1];
                l--;
                break;
            }
        }
        if (!check){
            return false;
        }
    }
    return true;
}
int &Array:: operator [](int index){
    if (index < 0 || index >= len) {
        cout << "Error: index is out of bounds";
        exit(1);
    }
    return arr[index];
}
bool Array::isSorted(){
for (int i =0 ; i < len -1 ; i++){
        if (arr[i]> arr[i+1]){
            return false;
            break;
        }
        return true;
    }
}
//Дополнительно (не обязательно).
//1. Конструктор, который формирует арифметическую прогрессию.
//2. Перегрузка операции +: слить два упорядоченных массива в третий упорядоченный.
//3. void operator - (int k); // удалить все вхожден
 // len – число элементов в массиве
 // mode = 1 – неупорядоченный массив
 // mode = 2 – массив, упорядоченный по неубыванию
 // mode = 3 – массив, упорядоченный по невозрастанию
 // range – диапазон псевдослучайных чисел для неупорядоченного массива (при mode = 1)