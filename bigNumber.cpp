#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <string>
#include <algorithm>
#include <iomanip>

using namespace std;

typedef unsigned char BASE;
typedef unsigned int DBASE;

class BigNumber {
private:
    BASE* coef;
    int len;
    int maxLen;

public:
    // 1) Конструкторы
    BigNumber(int mLen = 1, int mode = 0) : len(1), maxLen(mLen) {
        coef = new BASE[maxLen]();
        if (mode == 1) {
            for (int i = 0; i < maxLen; i++) coef[i] = rand() % 256;
            len = maxLen;
            normalize();
        }
    }

    BigNumber(const BigNumber& bn) : len(bn.len), maxLen(bn.len) {
        coef = new BASE[maxLen];
        memcpy(coef, bn.coef, len * sizeof(BASE));
    }

    ~BigNumber() { delete[] coef; }

    // operator =
    BigNumber& operator=(const BigNumber& bn) {
        if (this != &bn) {
            delete[] coef;
            len = bn.len; maxLen = bn.len;
            coef = new BASE[maxLen];
            memcpy(coef, bn.coef, len * sizeof(BASE));
        }
        return *this;
    }

    void normalize() {
        while (len > 1 && coef[len - 1] == 0) len--;
    }

    // 3) Сравнения
    bool operator<(const BigNumber& bn) const {
        if (len != bn.len) return len < bn.len;
        for (int i = len - 1; i >= 0; i--) {
            if (coef[i] < bn.coef[i]) return true;
            if (coef[i] > bn.coef[i]) return false;
        }
        return false;
    }
    bool operator==(const BigNumber& bn) const {
        if (len != bn.len) return false;
        return memcmp(coef, bn.coef, len) == 0;
    }
    bool operator!=(const BigNumber& bn) const { return !(*this == bn); }
    bool operator>(const BigNumber& bn) const { return bn < *this; }
    bool operator<=(const BigNumber& bn) const { return !(*this > bn); }
    bool operator>=(const BigNumber& bn) const { return !(*this < bn); }

    // 4) Сложение
    BigNumber operator+(const BigNumber& bn) const {
        int rL = max(len, bn.len) + 1;
        BigNumber res(rL, 0);
        DBASE k = 0;
        for (int j = 0; j < rL; j++) {
            DBASE t = (DBASE)((j < len) ? coef[j] : 0) + ((j < bn.len) ? bn.coef[j] : 0) + k;
            res.coef[j] = t & 255; k = t >> 8;
        }
        res.len = rL; res.normalize();
        return res;
    }
    BigNumber& operator+=(const BigNumber& bn) { return *this = *this + bn; }
    BigNumber operator+(BASE v) const { BigNumber tmp(1,0); tmp.coef[0]=v; return *this + tmp; }
    BigNumber& operator+=(BASE v) { return *this = *this + v; }

    // 5) Вычитание
    BigNumber operator-(const BigNumber& bn) const {
        if (*this < bn) return BigNumber(1, 0);
        BigNumber res(*this);
        int k = 0;
        for (int i = 0; i < len; i++) {
            int v = (i < bn.len) ? bn.coef[i] : 0;
            int t = (int)res.coef[i] - v - k;
            if (t < 0) { t += 256; k = 1; } else k = 0;
            res.coef[i] = (BASE)t;
        }
        res.normalize(); return res;
    }
    BigNumber& operator-=(const BigNumber& bn) { return *this = *this - bn; }
    BigNumber operator-(BASE v) const { BigNumber tmp(1,0); tmp.coef[0]=v; return *this - tmp; }
    BigNumber& operator-=(BASE v) { return *this = *this - v; }

    // 6) Умножение
    BigNumber operator*(const BigNumber& bn) const {
        if ((len == 1 && coef[0] == 0) || (bn.len == 1 && bn.coef[0] == 0)) return BigNumber(1, 0);
        BigNumber w(len + bn.len, 0); w.len = len + bn.len;
        for (int j = 0; j < bn.len; j++) {
            DBASE k = 0;
            for (int i = 0; i < len; i++) {
                DBASE t = (DBASE)coef[i] * bn.coef[j] + w.coef[i + j] + k;
                w.coef[i + j] = t & 255; k = t >> 8;
            }
            w.coef[len + j] = (BASE)k;
        }
        w.normalize(); return w;
    }
    BigNumber& operator*=(const BigNumber& bn) { return *this = *this * bn; }
    BigNumber operator*(BASE v) const { BigNumber tmp(1,0); tmp.coef[0]=v; return *this * tmp; }
    BigNumber& operator*=(BASE v) { return *this = *this * v; }

    // 7, 8) Деление и Остаток
    BigNumber operator/(const BigNumber& bn) const {
        if (bn == BigNumber(1, 0)) return BigNumber(1, 0);
        if (*this < bn) return BigNumber(1, 0);
        if (bn.len == 1) {
            BigNumber res(*this); DBASE r = 0;
            for (int j = len - 1; j >= 0; j--) {
                DBASE t = (r << 8) + res.coef[j];
                res.coef[j] = t / bn.coef[0]; r = t % bn.coef[0];
            }
            res.normalize(); return res;
        }
        DBASE d = 256 / ((DBASE)bn.coef[bn.len - 1] + 1);
        BigNumber u_p = (*this) * (BASE)d;
        BigNumber v = bn * (BASE)d;
        BigNumber u(u_p.len + 1, 0); memcpy(u.coef, u_p.coef, u_p.len); u.len = u_p.len + 1;
        int n = v.len, m = u.len - n;
        BigNumber q(m, 0); q.len = m;
        for (int j = m - 1; j >= 0; j--) {
            DBASE qh = (((DBASE)u.coef[j+n] << 8) + u.coef[j+n-1]) / v.coef[n-1];
            DBASE rh = (((DBASE)u.coef[j+n] << 8) + u.coef[j+n-1]) % v.coef[n-1];
            while (qh >= 256 || (qh * v.coef[n-2] > (rh << 8) + u.coef[j+n-2])) {
                qh--; rh += v.coef[n-1]; if (rh >= 256) break;
            }
            DBASE k = 0;
            for (int i = 0; i < n; i++) {
                DBASE p = qh * v.coef[i] + k;
                DBASE cp = p & 255, nk = p >> 8;
                if (u.coef[i+j] < cp) { u.coef[i+j] = (BASE)(u.coef[i+j] + 256 - cp); nk++; }
                else u.coef[i+j] -= (BASE)cp;
                k = nk;
            }
            if (u.coef[j+n] < k) {
                qh--; DBASE ak = 0;
                for (int i = 0; i < n; i++) {
                    DBASE t = (DBASE)u.coef[i+j] + v.coef[i] + ak;
                    u.coef[i+j] = t & 255; ak = t >> 8;
                }
                u.coef[j+n] = 0;
            } else u.coef[j+n] -= (BASE)k;
            q.coef[j] = (BASE)qh;
        }
        q.normalize(); return q;
    }
    BigNumber& operator/=(const BigNumber& bn) { return *this = *this / bn; }
    BigNumber operator%(const BigNumber& bn) const { return *this - ((*this / bn) * bn); }
    BigNumber& operator%=(const BigNumber& bn) { return *this = *this % bn; }

    // Операторы для BASE
    BigNumber operator/(BASE v) const { return *this / (BigNumber(1,0) + v); }
    BigNumber& operator/=(BASE v) { return *this = *this / (BASE)v; }
    BASE operator%(BASE v) const {
        DBASE r = 0;
        for (int j = len - 1; j >= 0; j--) r = ((r << 8) + coef[j]) % v;
        return (BASE)r;
    }

    // 2, 7) Ввод/Вывод
    friend ostream& operator<<(ostream& out, const BigNumber& bn) {
        out << hex << (int)bn.coef[bn.len - 1];
        for (int i = bn.len - 2; i >= 0; i--) out << setfill('0') << setw(2) << (int)bn.coef[i];
        return out << dec;
    }

    string toDecimal() const {
        if (len == 1 && coef[0] == 0) return "0";
        BigNumber tmp(*this); string s = "";
        while (!(tmp.len == 1 && tmp.coef[0] == 0)) { 
            s += to_string(tmp % (BASE)10); 
            tmp /= (BASE)10; 
        }
        reverse(s.begin(), s.end()); return s;
    }
};

void runTest9() {
    cout << "[TEST 9] Running 1000 iterations..." << endl;
    int N = 1000;
    do {
        BigNumber A(rand() % 100 + 1, 1);
        BigNumber D(rand() % 100 + 1, 1);
        if (D == BigNumber(1,0)) continue;
        BigNumber Q = A / D;
        BigNumber R = A % D;
        if (!(A == Q * D + R && A - R == Q * D && R < D)) {
            cout << "FAILED" << endl; return;
        }
    } while (--N);
    cout << "[TEST 9] PASSED!" << endl;
}

int main() {
    srand(time(0));
    runTest9();

    cout << "\n--- DEMONSTRATION ---" << endl;
    BigNumber n1(4, 1); 
    BigNumber n2(2, 1);
    if (n2 == BigNumber(1,0)) n2 += (BASE)7;

    cout << "n1 hex: " << n1 << " | dec: " << n1.toDecimal() << endl;
    cout << "n2 hex: " << n2 << " | dec: " << n2.toDecimal() << endl;

    BigNumber sum = n1 + n2;
    cout << "Sum: " << sum.toDecimal() << endl;
    
    BigNumber diff = n1 - n2;
    cout << "Diff: " << diff.toDecimal() << endl;

    BigNumber prod = n1 * n2;
    cout << "Prod: " << prod.toDecimal() << endl;

    BigNumber quot = n1 / n2;
    BigNumber rem = n1 % n2;
    cout << "Quot: " << quot.toDecimal() << endl;
    cout << "Rem:  " << rem.toDecimal() << endl;

    cout << "\nDONE." << endl;
    return 0;
}