#include <iostream>
#include <cstring>  
#include <windows.h>
using namespace std;

class String
{ char *str;
 int len;

public:
 String(int l = 0){
          len = l ;
          str = new char [len+1];
          str[0] = '\0';
 }
 String(const char *s){
    if(s){
        len = strlen(s);
        str= new char[len +1];
        strcpy(str,s);
    }else{
        len = 0;
        str = nullptr;
    }
 }
 String(const String &oth){
    len = oth.len;
    if (len>0){
        str = new char[len +1];
        strcpy(str,oth.str);
    }else{
        str = NULL; 
    }
 }
 ~String() {
 if(str) delete []str; str = NULL;
 }
 int strLength(){
    return len;
 }
 String & operator = (const String &oth){
    if (this != &oth){
        if(str) delete []str;
        len = oth.len;
        if(len >0){
            str = new char [len+1];
            strcpy(str,oth.str);
        }else{
            str = nullptr;
        }
    }
    return *this;
 }
char &operator [](int index){
    if (index < 0 || index > len) {
        cout << "Error: index is out of bounds";
        exit(1);
    }
    return str[index];
    
}
 bool operator == (String &oth){
     return strcmp(str, oth.str) == 0;
 }
 bool operator != (String &oth){
    return !(*this == oth);
 }
 String operator + (const String &oth){
    String res(len+oth.len);
    strcpy(res.str,str);
    strcat(res.str,oth.str);
    return res;

 }
 String & operator += (const String &oth){
    *this = *this + oth;
    return *this;
 }
friend istream & operator>>(istream &is, String &s);
friend ostream & operator<<(ostream &os, const String &s);
   
    String operator()(int i, int j) {
        if (i < 0 || j >= len || i > j) {
            cout << "Error: Invalid substring indices" << endl;
        }
        int subLen = j - i + 1;
        String subStr(subLen);
        strncpy(subStr.str, str + i, subLen);
        subStr.str[subLen] = '\0';
        return subStr;
    }
int BMSearch(String &substring) {
    int substringLen = substring.len;

    int text = len; 
    int helper[256]; 

    
    fill_n(helper, 256, substringLen);

    for (int i = 0; i < substringLen - 1; i++){
        helper[substring.str[i]] = substringLen - 1 - i;
    }

    int i = substringLen - 1; 
    int j = substringLen - 1; 

    
    while (i < text && j >= 0) { 
        int k = i;
        j = substringLen - 1; 
        while (j >= 0) { 
            if (str[k] != substring.str[j]) {
                i += helper[str[i]];
                j = substringLen - 1;
                break;
            }
            j--;
            k--;
        }
    }


    if (j >= 0) {
        return -1;
    } else {
        return i + 1 - substringLen;
    }

}


};
istream& operator>>(istream &is, String &s){
  const int massSize = 1000;
    char mass[massSize];

    is.getline(mass, massSize);

    s.len = strlen(mass);

    delete[] s.str;
    s.str = new char[s.len + 1];
    strcpy(s.str, mass);

    return is;
}

ostream& operator<<(ostream &os, const String &s){
    if (s.str) os << s.str;
    return os;
}


    
int main(){
    SetConsoleOutputCP(CP_UTF8);
 String s1, s2, s3;
    

    cout << "enter the string s1: ";
    cin >> s1; 
    cout << endl;

    cout << "enter a substring b: ";
    cin >> s2;
    cout << endl;


    cout << "String: " << s1 << "\n" << "Substring: " << s2 << endl;
    cout << "Length of s1: " << s1.strLength() << "\n" << "Length of s2: " << s2.strLength() << endl;

    cout << "match at symbol number: " << s1.BMSearch(s2) << "\n";

    if (s1 == s2) {
        cout << "s1 == s2" << endl;
    } else {
        cout << "s1 != s3" << endl;
    }

    s3 = s1 + s2;
    cout << "s1 + s2: " << s3 << endl;

    s1 += s2;
    cout << "s3 += s2: " << s3 << endl;
    return 0;
}
