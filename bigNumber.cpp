#include <iostream>
#include <cstring> 
#include <cstdlib>
#include <ctime>
using namespace std;
typedef unsigned int BASE;
typedef unsigned long int DBASE;
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
bool operator==(const BigNumber& bn) const;
bool operator!=(const BigNumber& bn) const;
bool operator<(const BigNumber& bn) const;
bool operator>(const BigNumber& bn) const;
bool operator<=(const BigNumber& bn) const;
bool operator>=(const BigNumber& bn) const;

friend istream &operator>> (istream &in, BigNumber &bn);
friend ostream &operator<< (ostream &, const BigNumber &);
};
istream &operator>>(istream &in, BigNumber &bn){
    string s;
    in >> s;
    bn.len = (s.size()-1)/(sizeof(BASE)*2)+1;
    bn.maxLen = bn.len ;

    int j = 0;
    int i = strlen(s.c_str()) -1; 
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
ostream &operator<<(ostream &out, const BigNumber &bn){
    bool started = false;
    for (int j = bn.len - 1; j >= 0; j--) {
        unsigned char byte = bn.coef[j];
        if (byte != 0 || started) {
            started = true;
            out << hex << (int)byte;
        }
    }
    if (!started) out << "0";
    return out;
}
BigNumber::BigNumber(int maxlen, int mode){
        len = maxlen;
        maxLen = maxlen;
        coef = new BASE[len];
        if(mode == 0){
        for (int i = 0; i < len; ++i) coef[i] = 0;
        }else if (mode=1){
            for (int i= 0; i<len;i++){
                coef[i] = rand()%100;
            }
        }
    }
BigNumber::BigNumber(const BigNumber &bn){
    len = bn.len;
    maxLen = bn.maxLen;
    coef = new BASE[maxLen];
    for (int i = 0; i < len; i++){
        coef[i] = bn.coef[i];
    }
}
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

BigNumber& BigNumber::operator+=(const BigNumber&bn){
    int carry = 0;
    int maxlen = max(len, bn.len);
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
        DBASE sum = (DBASE)coef[i] + (i < bn.len ? bn.coef[i] : 0) + carry;
        coef[i] = (BASE)sum;
        carry = (BASE)(sum >> BASE_SIZE);
    }
    return *this;
}
BigNumber BigNumber::operator+(const BigNumber& bn) const {
    BigNumber result(max(len, bn.len) + 1, 0);  
    int carry = 0;
    int i = 0;
    for (; i < len && i < bn.len; ++i) {
        DBASE sum = (DBASE)coef[i] + bn.coef[i] + carry;
        result.coef[i] = (BASE)sum;
        carry = (BASE)(sum >> BASE_SIZE);
    }
    for (; i < len; ++i) {
        DBASE sum = (DBASE)coef[i] + carry;
        result.coef[i] = (BASE)sum;
        carry = (BASE)(sum >> BASE_SIZE);
    }
    for (; i < bn.len; ++i) {
        DBASE sum = (DBASE)bn.coef[i] + carry;
        result.coef[i] = (BASE)sum;
        carry = (BASE)(sum >> BASE_SIZE);
    }
    if (carry) result.coef[i++] = carry;
    result.len = i;
    return result;
}
bool BigNumber::operator==(const BigNumber& bn) const {
    if (len != bn.len) return false;
    for (int i = 0; i < len; ++i)
        if (coef[i] != bn.coef[i]) return false;
    return true;
}

bool BigNumber::operator!=(const BigNumber& bn) const {
    return !(*this == bn);                     
}

bool BigNumber::operator<(const BigNumber& bn) const {
    if (len != bn.len)
        return len < bn.len;                   

    
    for (int i = len - 1; i >= 0; --i) {
        if (coef[i] < bn.coef[i]) return true;
        if (coef[i] > bn.coef[i]) return false;
    }
    return false; 
}

bool BigNumber::operator>(const BigNumber& bn) const {
    return bn < *this;
}

bool BigNumber::operator<=(const BigNumber& bn) const {
    return !(*this > bn);
}

bool BigNumber::operator>=(const BigNumber& bn) const {
    return !(*this < bn);
}




int main(){
    srand(time(NULL));
        BigNumber bn1(4,1); 
        BigNumber bn2(4,1);
        cout << "BigNumber 1: " << bn1 << endl;
        cout << "BigNumber 2: " << bn2 << endl;
        BigNumber sum = bn1 + bn2;
        BigNumber Bn3 = bn1 +=bn2;
        cout << "Sum: " << sum << endl;
        cout << "Sum: " << Bn3 << endl;
return 0;
}