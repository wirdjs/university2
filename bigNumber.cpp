#include <iostream>
#include <cstring> 
#include <cstdlib>
#include <ctime>
using namespace std;
typedef unsigned int BASE;
#define BASE_SiZE (sizeof(BASE)*8)
class BigNumber
{
private:
    unsigned int*coef;
    int len;
    int maxLen;
public:
    BigNumber(int maxlen=1,int mode=0);
    ~BigNumber(){
        delete[] coef;
        coef = nullptr;
    };
    BigNumber& operator=(const BigNumber &bn);
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
        if (k>=BASE_SiZE)
        {
           k =0;
           j++;
        }
        i--;
    }
    return in;
} 
ostream &operator<<(ostream &out, const BigNumber &bn){
    int j= bn.len-1;
    int i =0;
    int k = BASE_SiZE-4;
     while (j>=0){
        out.width(BASE_SiZE/4);
        out.fill('0');
        out<<hex<<bn.coef[j];
        j--;
     }
    return out;
 
}
BigNumber::BigNumber(int maxlen=1,int mode=0){
        len = maxlen;
        maxLen = maxlen;
        coef = new unsigned int[len];
        if(mode == 0){
        coef ={0};
        }else if (mode=1){
            for (int i= 0; i<len;i++){
                coef[i] = rand()%100;
            }
        }
    }
 BigNumber& BigNumber::operator=(const BigNumber &bn){
    if (this != &bn){
    len = bn.len;
    maxLen = bn.maxLen;
   if(coef) delete[] coef;
   coef = new unsigned int[len];
    for (int i = 0; i <len ; i++){
        coef[i] = bn.coef[i];
    }
    }
    return *this;
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
return 0;
}