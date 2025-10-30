#include <iostream>
#include <cstring>  
#include <windows.h>
using namespace std;

class String
{ char *str;
 int len;

public:
 String(int l = 0){
    if(l >0){
          len = l ;
          str = new char [len+1];
          str[0] = '\0';
    }else{
        len = 0;
        str = NULL;
        cout<< "Error: len isn't be a negative"<<endl;
    }
  
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
    if (index < 0 || index >=len)
    {
        len = 0;
        str = NULL;
        cout << "Error:you write wrong index"<<endl;
    }else{
        return str[index];
    }
    
}
 bool operator == (String &oth){
    return strcmp(this->str , oth.str) == 0;
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
    int textLen = len;
    int helper[256];

    fill_n(helper, 256, substringLen);
    for (int i = 0; i < substringLen - 1; i++) {
        helper[(unsigned char)substring.str[i]] = substringLen - 1 - i;
    }

    int i = substringLen - 1;

    while (i < textLen) {
        int j = substringLen - 1;
        int k = i;

        while (j >= 0 && str[k] == substring.str[j]) {
            j--;
            k--;
        }

        if (j < 0)
            return i + 1 - substringLen;

        i += helper[(unsigned char)str[i]];
    }

    return -1;
}


};
istream& operator>>(istream &is, String &s){
    char buffer[1000];
    is >> buffer;
    s = String(buffer);
    return is;
}

ostream& operator<<(ostream &os, const String &s){
    if (s.str) os << s.str;
    return os;
}


    
int main(){
    SetConsoleOutputCP(CP_UTF8);
  String s1(12);
    cout << "s1 length: " << s1.strLength() << endl;
    
    String s2("Hello");
    String s3(" World");
    String s4 = s2 + s3;
    cout << "s4: " << s4 << endl;
    
    // Тестирование поиска и удаления подстроки
    String text("На дворе трава на траве дрова");
    String pattern("траве");
    
    cout << "Original: " << text << endl;
    cout << "Pattern: " << pattern << endl;
    
    int pos = text.BMSearch(pattern);
    
    if (pos != -1) {
        cout << "Pattern found at position: " << pos << endl;
    } else {
        cout << "Pattern not found" << endl;
    }

    return 0;
}
