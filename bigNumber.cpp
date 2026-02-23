#include <iostream>
#include <cstring> 
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
    BigNumber();
    ~BigNumber(){
        delete[] coef;
    };
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

BigNumber::BigNumber(/* args */)
{
}



int main(){
return 0;
}