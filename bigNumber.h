#pragma once
#include <iostream>
#include <string>


typedef unsigned  int BASE;
typedef unsigned  long long DBASE;

#define BASE_SIZE  (sizeof(BASE) * 8)          
#define BASE_VAL   ((DBASE)1 << BASE_SIZE)     

class BigNumber {
public:
    BASE* coef;   
    int   len;
    int   maxLen;

    BigNumber(int mLen = 1, int mode = 0);   
    BigNumber(const BigNumber& bn);
    ~BigNumber();
    BigNumber& operator=(const BigNumber& bn);

    void normalize();

    bool operator< (const BigNumber& bn) const;
    bool operator==(const BigNumber& bn) const;
    bool operator!=(const BigNumber& bn) const;
    bool operator> (const BigNumber& bn) const;
    bool operator<=(const BigNumber& bn) const;
    bool operator>=(const BigNumber& bn) const;

    BigNumber  operator+(const BigNumber& bn) const;
    BigNumber& operator+=(const BigNumber& bn);
    BigNumber  operator+(BASE v) const;
    BigNumber& operator+=(BASE v);

    BigNumber  operator-(const BigNumber& bn) const;
    BigNumber& operator-=(const BigNumber& bn);
    BigNumber  operator-(BASE v) const;
    BigNumber& operator-=(BASE v);

    BigNumber  operator*(const BigNumber& bn) const;
    BigNumber& operator*=(const BigNumber& bn);
    BigNumber  operator*(BASE v) const;
    // BigNumber operator*(DBASE v) const;
    BigNumber& operator*=(BASE v);

    BigNumber  operator/(const BigNumber& bn) const;
    BigNumber& operator/=(const BigNumber& bn);
    BigNumber  operator/(BASE v) const;
    BigNumber& operator/=(BASE v);

    BigNumber  operator%(const BigNumber& bn) const;
    BigNumber& operator%=(const BigNumber& bn);
    BASE       operator%(BASE v) const;

    

    std::string toDecimal() const;

    friend std::istream& operator>>(std::istream& in,  BigNumber& bn);
    friend std::ostream& operator<<(std::ostream& out, const BigNumber& bn);
};