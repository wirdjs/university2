#include <iostream>
#include <cstring> 
#include <cstdlib>
#include <ctime>
#include <string>
#include <algorithm>
using namespace std;
typedef unsigned int BASE;
typedef unsigned long long DBASE;
#define BASE_SIZE (sizeof(BASE)*8)
const DBASE BASE_MASK = ((DBASE)1 << BASE_SIZE) - 1;
const DBASE BASE_MAX = (DBASE)1 << BASE_SIZE;
class BigNumber
{
private:
    BASE*coef;
    int len;
    int maxLen;
public:
    BigNumber(int maxlen=1,int mode=0);
    BigNumber(const BigNumber &bn);
    ~BigNumber(){
        delete[] coef;
        coef = nullptr;
    };
    BigNumber& operator=(const BigNumber &bn);
    BigNumber& operator+=(const BigNumber&bn);
    BigNumber operator+(const BigNumber&bn) const;
    BigNumber& operator-=(const BigNumber&bn);
    BigNumber operator-(const BigNumber&bn) const;
    string toDecimal() const;
    BigNumber& operator*=(const BigNumber& bn);
    BigNumber operator*(const BigNumber& bn) const;
    BigNumber& operator/=(BASE v);
    BigNumber operator/(BASE v) const;
    BASE operator%(BASE v) const;
    BigNumber& operator/=(const BigNumber& bn);
    BigNumber operator/(const BigNumber& bn) const;
    BigNumber& operator%=(const BigNumber& bn);
    BigNumber operator%(const BigNumber& bn) const;
    void normalize();
bool operator==(const BigNumber& bn) const;
bool operator!=(const BigNumber& bn) const;
bool operator<(const BigNumber& bn) const;
bool operator>(const BigNumber& bn) const;
bool operator<=(const BigNumber& bn) const;
bool operator>=(const BigNumber& bn) const;

friend istream &operator>> (istream &in, BigNumber &bn);
friend ostream &operator<< (ostream &, const BigNumber &);
};

// Оператор ввода: считывает шестнадцатеричную строку и конвертирует её в массив BASE
istream &operator>>(istream &in, BigNumber &bn){
    string s;
    in >> s;
    if (bn.coef) delete[] bn.coef;

    bn.len = (s.size()-1)/(sizeof(BASE)*2)+1;
    bn.maxLen = bn.len ;
    bn.coef = new BASE[bn.maxLen]();

    int j = 0;
    // Идем с конца строки (младшие разряды)
    int i = s.size() - 1; 
    int k = 0;
    while(i>=0){
        int tmp ;
        if ('0'<=s[i] && s[i]<='9')
        {
            tmp= s[i]-'0';
        }
        else if ('a'<=s[i]&& s[i]<='f')
        {
            tmp = s[i]-'a'+10;
        }
        else if ('A'<=s[i]&& s[i]<='F'){
            tmp = s[i]-'A'+10;
        }        
        // Записываем 4 бита в нужную позицию текущего коэффициента
        bn.coef[j]|=tmp<<k;
        k+=4;
        if (k>=BASE_SIZE)
        {
           k =0;
           j++;
        }
        i--;
    }
    return in;
} 

// Оператор вывода: печатает число в шестнадцатеричном виде
ostream &operator<<(ostream &out, const BigNumber &bn){
    bool started = false;
    for (int j = bn.len - 1; j >= 0; j--) {
        // Пропускаем ведущие нули, пока не встретим первое значащее число
        if (!started) {
            if (bn.coef[j] != 0 || j == 0) {
                out << hex << bn.coef[j]; 
                started = true;
            }
        } else {
            out.fill('0');             // Устанавливаем символ '0' для заполнения
            out.width(sizeof(BASE) * 2); // Устанавливаем ширину поля (8 символов для 32 бит)
            out << hex << bn.coef[j];
        }
    }
    return out;
}

// Конструктор: выделяет память и инициализирует нулями или случайными числами
BigNumber::BigNumber(int maxlen, int mode){
        len = maxlen;
        maxLen = maxlen;
        coef = new BASE[len];
        if(mode == 0){
        for (int i = 0; i < len; ++i) coef[i] = 0;
        }else if (mode == 1){
            for (int i= 0; i<len;i++){
                coef[i] = rand()%100;
            }
        }
    }

// Конструктор копирования: выполняет глубокое копирование массива коэффициентов
BigNumber::BigNumber(const BigNumber &bn){
    len = bn.len;
    maxLen = bn.maxLen;
    coef = new BASE[maxLen];
    for (int i = 0; i < len; i++){
        coef[i] = bn.coef[i];
    }
}

// Оператор присваивания: освобождает старую память и копирует данные из другого объекта
BigNumber& BigNumber::operator=(const BigNumber &bn){
    if (this != &bn){
    len = bn.len;
    maxLen = bn.maxLen;
   if(coef) delete[] coef;
   coef = new BASE[len];
    for (int i = 0; i <len ; i++){
        coef[i] = bn.coef[i];
    }
    }
    return *this;
 }

// Сложение: прибавляет к текущему числу другое, учитывая перенос (carry)
BigNumber& BigNumber::operator+=(const BigNumber&bn){
    int carry = 0;
    int maxlen = max(len, bn.len);
    // Если текущей емкости не хватает, расширяем массив
    if (maxlen > maxLen) {
        maxLen = maxlen;
        BASE* newCoef = new BASE[maxLen];
        memset(newCoef, 0, maxLen * sizeof(BASE));
        for (int i = 0; i < len; ++i)
            newCoef[i] = coef[i];
        delete[] coef;
        coef = newCoef;
    }
    for (int i = 0; i < maxlen || carry; ++i) {
        if (i == len) len++;
        // Используем 64-битное число для хранения суммы и переноса
        DBASE sum = (DBASE)coef[i] + (i < bn.len ? bn.coef[i] : 0) + carry;
        coef[i] = (BASE)sum;
        carry = (BASE)(sum >> BASE_SIZE);
    }
    normalize();
    return *this;
}

// Оператор сложения: возвращает новый объект-сумму
BigNumber BigNumber::operator+(const BigNumber& bn) const {
    BigNumber result(*this);
    result += bn;
    return result;
}

// Вычитание: вычитает из текущего числа меньшее (или равное), учитывая заем (borrow)
BigNumber& BigNumber::operator-=(const BigNumber& bn) {
    if (*this < bn) {
        memset(coef, 0, maxLen * sizeof(BASE));
        len = 1;
        return *this;
    }
    DBASE borrow = 0;
    for (int i = 0; i < len; ++i) {
        // Вычитаем из 64-битного числа, чтобы отследить отрицательный результат
        DBASE sub = (DBASE)coef[i] - (i < bn.len ? bn.coef[i] : 0) - borrow;
        if (sub >= BASE_MAX) { // Произошел заем
            coef[i] = (BASE)(sub + BASE_MAX);
            borrow = 1;
        } else {
            coef[i] = (BASE)sub;
            borrow = 0;
        }
    }
    normalize();
    return *this;
}

// Оператор вычитания: возвращает разность
BigNumber BigNumber::operator-(const BigNumber& bn) const {
    BigNumber result(*this);
    result -= bn;
    return result;
}

// Преобразование в десятичную строку (алгоритм деления на 10)
string BigNumber::toDecimal() const {
    if (len == 1 && coef[0] == 0) return "0";
    BigNumber temp(*this);
    string res = "";
    while (!(temp.len == 1 && temp.coef[0] == 0)) {
        res += (char)(temp % 10 + '0');
        temp /= 10;
    }
    reverse(res.begin(), res.end());
    return res;
}

// Умножение: реализует школьный алгоритм умножения "столбиком"
BigNumber& BigNumber::operator*=(const BigNumber& bn) {
    BigNumber res(len + bn.len, 0);
    for (int i = 0; i < len; ++i) {
        DBASE carry = 0;
        for (int j = 0; j < bn.len || carry; ++j) {
            DBASE cur = res.coef[i + j] +
                        (DBASE)coef[i] * (j < bn.len ? bn.coef[j] : 0) + carry;
            res.coef[i + j] = (BASE)(cur & BASE_MASK);
            carry = cur >> BASE_SIZE;
        }
    }
    res.normalize();
    *this = res;
    return *this;
}

// Оператор умножения: возвращает произведение
BigNumber BigNumber::operator*(const BigNumber& bn) const {
    BigNumber result(*this);
    result *= bn;
    return result;
}

// Деление на короткое число (BASE): проход от старших разрядов к младшим
BigNumber& BigNumber::operator/=(BASE v) {
    if (v == 0) throw "Division by zero";
    DBASE rem = 0;
    for (int j = len - 1; j >= 0; j--) {
        DBASE cur = coef[j] + rem * BASE_MAX;
        coef[j] = (BASE)(cur / v);
        rem = cur % v;
    }
    normalize();
    return *this;
}

// Оператор деления на BASE
BigNumber BigNumber::operator/(BASE v) const {
    BigNumber res(*this);
    res /= v;
    return res;
}

// Остаток от деления на BASE
BASE BigNumber::operator%(BASE v) const {
    if (v == 0) throw "Division by zero";
    DBASE rem = 0;
    for (int j = len - 1; j >= 0; j--) {
        rem = (coef[j] + rem * BASE_MAX) % v;
    }
    return (BASE)rem;
}

// Деление на длинное число: побитовый алгоритм (сдвиг и вычитание)
BigNumber& BigNumber::operator/=(const BigNumber& bn) {
    if (bn == BigNumber(1, 0)) throw "Division by zero";
    if (*this < bn) {
        *this = BigNumber(1, 0);
        return *this;
    }
    BigNumber q(len, 0);
    BigNumber r(1, 0);
    // Проходим по каждому биту делимого
    for (int i = len * BASE_SIZE - 1; i >= 0; i--) {
        DBASE carry = ((coef[i / BASE_SIZE] >> (i % BASE_SIZE)) & 1);
        for (int j = 0; j < r.len || carry; j++) {
            if (j == r.len) {
                if (r.len < r.maxLen) r.len++;
                else break; 
            }
            DBASE cur = ((DBASE)r.coef[j] << 1) + carry;
            r.coef[j] = (BASE)(cur & BASE_MASK);
            carry = cur >> BASE_SIZE;
        }
        r.normalize();

        if (r >= bn) {
            r -= bn;
            q.coef[i / BASE_SIZE] |= (1U << (i % BASE_SIZE));
        }
    }
    q.normalize();
    *this = q;
    return *this;
}

// Оператор деления на BigNumber
BigNumber BigNumber::operator/(const BigNumber& bn) const {
    BigNumber res(*this);
    res /= bn;
    return res;
}

// Остаток от деления на BigNumber: вычисляется как a - (a/b)*b
BigNumber& BigNumber::operator%=(const BigNumber& bn) {
    if (bn == BigNumber(1, 0)) throw "Division by zero";
    if (*this < bn) return *this;
    
    BigNumber q(*this);
    q /= bn;
    *this -= (q * bn);
    normalize();
    return *this;
}

// Оператор получения остатка
BigNumber BigNumber::operator%(const BigNumber& bn) const {
    BigNumber res(*this);
    res %= bn;
    return res;
}

// Убирает ведущие нулевые коэффициенты в массиве
void BigNumber::normalize() {
    while (len > 1 && coef[len - 1] == 0) len--;
}

// Сравнение на равенство
bool BigNumber::operator==(const BigNumber& bn) const {
    if (len != bn.len) return false;
    for (int i = 0; i < len; ++i)
        if (coef[i] != bn.coef[i]) return false;
    return true;
}

// Сравнение на неравенство
bool BigNumber::operator!=(const BigNumber& bn) const {
    return !(*this == bn);                     
}

// Сравнение "меньше": сначала по длине, затем лексикографически со старших разрядов
bool BigNumber::operator<(const BigNumber& bn) const {
    if (len != bn.len)
        return len < bn.len;                   

    for (int i = len - 1; i >= 0; --i) {
        if (coef[i] < bn.coef[i]) return true;
        if (coef[i] > bn.coef[i]) return false;
    }
    return false; 
}

// Сравнение "больше"
bool BigNumber::operator>(const BigNumber& bn) const {
    return bn < *this;
}

// Сравнение "меньше или равно"
bool BigNumber::operator<=(const BigNumber& bn) const {
    return !(*this > bn);
}

// Сравнение "больше или равно"
bool BigNumber::operator>=(const BigNumber& bn) const {
    return !(*this < bn);
}




int main(){
    srand(time(NULL));
        BigNumber bn1(1, 1); 
        BigNumber bn2(1, 1);
    
        
        cout << "BN1 Hex: " << bn1 << " | Dec: " << bn1.toDecimal() << endl;
        cout << "BN2 Hex: " << bn2 << " | Dec: " << bn2.toDecimal() << endl;

        cout << "Sum: " << bn1 + bn2 << endl;
        cout << "Sum (Dec): " << (bn1 + bn2).toDecimal() << endl;

        cout << "Product (Dec): " << (bn1 * bn2).toDecimal() << endl;
        cout << "Quotient (Dec): " << (bn1 / bn2).toDecimal() << endl;
        cout<<"Product (Hex): " << (bn1 * bn2) << endl;
        cout << "Quotient (Hex): " << (bn1 / bn2)<< endl;
        cout << "Quotient (Hex): " << (bn1 / 10)<< endl;
return 0;
}