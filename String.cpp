#include <iostream>
#include <cstring>
using namespace std;

class String {
    char *str;
    int len;
public:
    String(int l = 0) {
        len = l;
        str = new char[len+1];
       str[l] = '\0';
    }
    String(const char *s);
    String(const String &oth);
    ~String() {
        if(str) delete []str; 
        str = NULL;
    }
    int strLength() {
        return len;
    }       
    String & operator = (const String &oth);
    char &operator [](int index);
    bool operator == (String oth);
    bool operator != (String &oth);
    String operator + (const String &oth);
    String & operator += (const String &oth);
    friend istream & operator>>(istream &is, String &s);
    friend ostream & operator<<(ostream &os, const String &s);
    String operator()(int i, int j);
    int BMSearch(String &substring);
    String & HalfStr();
    
};

istream& operator>>(istream &is, String &s) {
    const int massSize = 1000;
    char mass[massSize];
    is.getline(mass, massSize);
    s.len = strlen(mass);
    delete[] s.str;
    s.str = new char[s.len + 1];
    strcpy(s.str, mass);
    return is;
}

ostream& operator<<(ostream &os, const String &s) {
    if (s.str) os << s.str;
    return os;
}

String::String(const char *s) {
    if(s){
        len = strlen(s);
        str= new char[len + 1];
        strcpy(str, s);
    }
}

String::String(const String &oth) {
    len = oth.len;
    if (len > 0) {
        str = new char[len + 1];
        strcpy(str, oth.str);
    } else {
        str = NULL;
    }
}
String& String::HalfStr(){
    len = len/2;
    char* str1 = new char[len+1];
    for (int i=0 ; i<len;i++){
        str1[i] = str[i];
    }
    str1[len] = '\0';
    delete []str;
    str =str1;
    return *this;
}
String& String::operator = (const String &oth) {
    if (this != &oth) {
        if(str) delete []str;
        len = oth.len;  
        if(len > 0) {
            str = new char[len + 1];
            strcpy(str, oth.str);
        } else {
            str = nullptr;
        }
    }
    return *this;
}

String& String::operator += (const String &oth) {
    *this = *this + oth;
    return *this;
}

String String::operator + (const String &oth) {
    String res(len + oth.len);
    strcpy(res.str, str);
    strcat(res.str, oth.str);
    return res;
}

char& String::operator [](int index) {
    if (index < 0 || index >= len) {
        cout << "Error: index is out of bounds" << endl;
        exit(1);
    }
    return str[index];
}

bool String::operator != (String &oth) {
    return !(*this == oth);
}

bool String::operator == (String oth) {
    return strcmp(str, oth.str) == 0;
}

String String::operator()(int i, int j) {
    if (i < 0 || j >= len || i > j) {
        cout << "Error: Invalid substring indices" << endl;
    }
    int subLen = j - i + 1;
    String subStr(subLen);
    strncpy(subStr.str, str + i, subLen);
    subStr.str[subLen] = '\0';
    return subStr;
}

int String::BMSearch(String &substring) {
    int substringLen = substring.len;
    int textLen = len; 
    int helper[256]; 

    fill_n(helper, 256, substringLen);

    for (int i = 0; i < substringLen - 1; i++){
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
        if (j < 0) {
            return k + 1; 
        }
        i += helper[(unsigned char)str[i]];
    } 
    return -1; 
}
int main() {
    String s1, s2, s3,s4;
    String s5(s4);
    String s6 = s4;
    s6 = s4;
    cout << "enter the string s1: ";
    cin >> s1; 
    cout << endl;

    // cout << "enter a substring b: ";
    // cin >> s2;
    // cout << endl;

//     cout << "String: " << s1 << "\nSubstring: " << s2 << endl;
//     cout << "Length of s1: " << s1.strLength() << "\nLength of s2: " << s2.strLength() << endl;

//     cout << "match at symbol number: " << s1.BMSearch(s2) << "\n";

//     if (s1 == s2) {
//         cout << "s1 == s2" << endl;
//     } else {
//         cout << "s1 != s2" << endl;
//     }

//     s3 = s1 + s2;
//     cout << "s1 + s2: " << s3 << endl;

//     s1 += s2;    
//     cout << "s1 += s2: " << s1 << endl;
//    s4 = s1(2,3);
//    cout<< "slice a worb:";
//    cout<< s4<<endl;
   
   cout<< s1.HalfStr().HalfStr()<<endl;

    return 0;
}
