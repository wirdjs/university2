#ifndef BIGNUMBER_H
#define BIGNUMBER_H

#include <iostream>
#include <string>
using namespace std;
typedef unsigned int BASE;   // 1 byte limb
typedef unsigned long DBASE;  // accumulator wide enough for BASE*BASE + carry


typedef unsigned int BASE;   
typedef unsigned long DBASE;  

class BigNumber {
private:
    BASE* coef;
    int len;
    int maxLen;

public:
    BigNumber(int mLen = 1, int mode = 0);
    BigNumber(const BigNumber& bn);
    ~BigNumber();
    BigNumber& operator=(const BigNumber& bn);

    void normalize();

    bool operator<(const BigNumber& bn) const;
    bool operator==(const BigNumber& bn) const;
    bool operator!=(const BigNumber& bn) const;
    bool operator>(const BigNumber& bn) const;
    bool operator<=(const BigNumber& bn) const;
    bool operator>=(const BigNumber& bn) const;

    BigNumber operator+(const BigNumber& bn) const;
    BigNumber& operator+=(const BigNumber& bn);
    BigNumber operator+(BASE v) const;
    BigNumber& operator+=(BASE v);

    BigNumber operator-(const BigNumber& bn) const;
    BigNumber& operator-=(const BigNumber& bn);
    BigNumber operator-(BASE v) const;
    BigNumber& operator-=(BASE v);

    BigNumber operator*(const BigNumber& bn) const;
    BigNumber& operator*=(const BigNumber& bn);
    BigNumber operator*(BASE v) const;
    BigNumber& operator*=(BASE v);

    BigNumber operator/(const BigNumber& bn) const;
    BigNumber& operator/=(const BigNumber& bn);
    BigNumber operator%(const BigNumber& bn) const;
    BigNumber& operator%=(const BigNumber& bn);

    BigNumber operator/(BASE v) const;
    BigNumber& operator/=(BASE v);
    BASE operator%(BASE v) const;

    BigNumber mul_scalar(DBASE v) const;

    string toDecimal() const;

    friend ostream& operator<<(std::ostream& out, const BigNumber& bn);
    friend istream& operator>>(std::istream& in, BigNumber& bn);
};

#endif // BIGNUMBER_H
