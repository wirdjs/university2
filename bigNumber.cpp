#include "bigNumber.h"
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iomanip>

using namespace std;

// --- реализация методов ---

BigNumber::BigNumber(int mLen, int mode) : len(1), maxLen(mLen) {
    coef = new BASE[maxLen]();
    if (mode == 1) {
        for (int i = 0; i < maxLen; i++) coef[i] = (BASE)(rand() % 256);
        len = maxLen;
        normalize();
    }
}

BigNumber::BigNumber(const BigNumber& bn) : len(bn.len), maxLen(bn.maxLen) {
    coef = new BASE[maxLen];
    memcpy(coef, bn.coef, maxLen * sizeof(BASE));
}

BigNumber::~BigNumber() { delete[] coef; }

BigNumber& BigNumber::operator=(const BigNumber& bn) {
    if (this != &bn) {
        delete[] coef;
        len = bn.len;
        maxLen = bn.maxLen;
        coef = new BASE[maxLen];
        memcpy(coef, bn.coef, maxLen * sizeof(BASE));
    }
    return *this;
}

void BigNumber::normalize() {
    while (len > 1 && coef[len - 1] == 0) len--;
}

bool BigNumber::operator<(const BigNumber& bn) const {
    if (len != bn.len) return len < bn.len;
    for (int i = len - 1; i >= 0; i--) {
        if (coef[i] < bn.coef[i]) return true;
        if (coef[i] > bn.coef[i]) return false;
    }
    return false;
}

bool BigNumber::operator==(const BigNumber& bn) const {
    if (len != bn.len) return false;
    return memcmp(coef, bn.coef, len * sizeof(BASE)) == 0;
}

bool BigNumber::operator!=(const BigNumber& bn) const { return !(*this == bn); }
bool BigNumber::operator>(const BigNumber& bn) const { return bn < *this; }
bool BigNumber::operator<=(const BigNumber& bn) const { return !(*this > bn); }
bool BigNumber::operator>=(const BigNumber& bn) const { return !(*this < bn); }

BigNumber BigNumber::operator+(const BigNumber& bn) const {
    int rL = max(len, bn.len) + 1;
    BigNumber res(rL, 0);
    DBASE k = 0;
    for (int j = 0; j < rL; j++) {
        DBASE t = (DBASE)((j < len) ? coef[j] : 0) + ((j < bn.len) ? bn.coef[j] : 0) + k;
        res.coef[j] = (BASE)(t & 0xFFu); k = t >> 8;
    }
    res.len = rL; res.normalize();
    return res;
}
BigNumber& BigNumber::operator+=(const BigNumber& bn) { return *this = *this + bn; }
BigNumber BigNumber::operator+(BASE v) const { BigNumber tmp(1,0); tmp.coef[0]=v; tmp.len = 1; return *this + tmp; }
BigNumber& BigNumber::operator+=(BASE v) { return *this = *this + v; }

BigNumber BigNumber::operator-(const BigNumber& bn) const {
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
BigNumber& BigNumber::operator-=(const BigNumber& bn) { return *this = *this - bn; }
BigNumber BigNumber::operator-(BASE v) const { BigNumber tmp(1,0); tmp.coef[0]=v; tmp.len = 1; return *this - tmp; }
BigNumber& BigNumber::operator-=(BASE v) { return *this = *this - v; }

BigNumber BigNumber::operator*(const BigNumber& bn) const {
    if ((len == 1 && coef[0] == 0) || (bn.len == 1 && bn.coef[0] == 0)) return BigNumber(1, 0);
    BigNumber w(len + bn.len, 0); w.len = len + bn.len;
    for (int j = 0; j < bn.len; j++) {
        DBASE k = 0;
        for (int i = 0; i < len; i++) {
            DBASE t = (DBASE)coef[i] * bn.coef[j] + w.coef[i + j] + k;
            w.coef[i + j] = (BASE)(t & 0xFFu); k = t >> 8;
        }
        w.coef[len + j] = (BASE)k;
    }
    w.normalize(); return w;
}
BigNumber& BigNumber::operator*=(const BigNumber& bn) { return *this = *this * bn; }
BigNumber BigNumber::operator*(BASE v) const { BigNumber tmp(1,0); tmp.coef[0]=v; tmp.len = 1; return *this * tmp; }
BigNumber& BigNumber::operator*=(BASE v) { return *this = *this * v; }

BigNumber BigNumber::operator/(const BigNumber& bn) const {
    bool isZero = (bn.len == 1 && bn.coef[0] == 0);
    if (isZero) return BigNumber(1, 0);
    if (*this < bn) return BigNumber(1, 0);
    if (bn.len == 1) {
        BigNumber res(*this); DBASE r = 0;
        for (int j = len - 1; j >= 0; j--) {
            DBASE t = (r << 8) + res.coef[j];
            res.coef[j] = (BASE)(t / bn.coef[0]); r = t % bn.coef[0];
        }
        res.normalize(); return res;
    }
    DBASE d = 256u / ((DBASE)bn.coef[bn.len - 1] + 1u);
    BigNumber u_p = (*this).mul_scalar(d);
    BigNumber v = bn.mul_scalar(d);
    BigNumber u(u_p.len + 1, 0); memcpy(u.coef, u_p.coef, u_p.len * sizeof(BASE)); u.len = u_p.len + 1;
    int n = v.len, m = u.len - n;
    BigNumber q(m, 0); q.len = m;
    for (int j = m - 1; j >= 0; j--) {
        DBASE ujn = (j + n < u.len) ? (DBASE)u.coef[j + n] : 0;
        DBASE ujn1 = (j + n - 1 < u.len) ? (DBASE)u.coef[j + n - 1] : 0;
        DBASE num = (ujn << 8) + ujn1;
        DBASE den = (DBASE)v.coef[n - 1];
        DBASE qh = num / den;
        DBASE rh = num % den;
        DBASE v_n2 = (n >= 2) ? (DBASE)v.coef[n - 2] : 0;
        while (qh >= 256 || (qh * v_n2 > ((rh << 8) + ((j + n - 2 < u.len) ? u.coef[j + n - 2] : 0)))) {
            qh--; rh += v.coef[n - 1]; if (rh >= 256) break;
        }
        DBASE k = 0;
        for (int i = 0; i < n; i++) {
            DBASE p = qh * (DBASE)v.coef[i] + k;
            DBASE cp = p & 0xFFu, nk = p >> 8;
            DBASE uij = (j + i < u.len) ? (DBASE)u.coef[j + i] : 0;
            if (uij < cp) { u.coef[j + i] = (BASE)(uij + 256 - cp); nk++; }
            else u.coef[j + i] = (BASE)(uij - cp);
            k = nk;
        }
        if ((j + n) < u.len) {
            if ((DBASE)u.coef[j + n] < k) {
                qh--; DBASE ak = 0;
                for (int i = 0; i < n; i++) {
                    DBASE t = (DBASE)u.coef[i + j] + (DBASE)v.coef[i] + ak;
                    u.coef[i + j] = (BASE)(t & 0xFFu); ak = t >> 8;
                }
                u.coef[j + n] = (BASE)ak;
            } else u.coef[j + n] = (BASE)((DBASE)u.coef[j + n] - k);
        }
        q.coef[j] = (BASE)(qh & 0xFFu);
    }
    q.normalize(); return q;
}
BigNumber& BigNumber::operator/=(const BigNumber& bn) { return *this = *this / bn; }
BigNumber BigNumber::operator%(const BigNumber& bn) const { return *this - ((*this / bn) * bn); }
BigNumber& BigNumber::operator%=(const BigNumber& bn) { return *this = *this % bn; }

BigNumber BigNumber::operator/(BASE v) const {
    BigNumber divisor(1,0); divisor.coef[0] = (BASE)v; divisor.len = 1;
    return *this / divisor;
}
BigNumber& BigNumber::operator/=(BASE v) { return *this = *this / v; }
BASE BigNumber::operator%(BASE v) const {
    DBASE r = 0;
    for (int j = len - 1; j >= 0; j--) r = ((r << 8) + coef[j]) % v;
    return (BASE)r;
}

BigNumber BigNumber::mul_scalar(DBASE v) const {
    if (v == 0) return BigNumber(1, 0);
    BigNumber res(len + 2, 0);
    DBASE carry = 0;
    for (int i = 0; i < len; i++) {
        DBASE t = (DBASE)coef[i] * v + carry;
        res.coef[i] = (BASE)(t & 0xFFu);
        carry = t >> 8;
    }
    int idx = len;
    while (carry) {
        res.coef[idx++] = (BASE)(carry & 0xFFu);
        carry >>= 8;
    }
    res.len = idx;
    res.normalize();
    return res;
}
istream& operator>>(istream& in, BigNumber& bn) {
    string s; in >> s;
    int n = s.length();
    BigNumber res((n + 1) / 2, 0);
    for (int i = 0; i < n; i++) {
        char c = s[n - 1 - i];
        int v = (c >= '0' && c <= '9') ? (c - '0') : ((c >= 'A' && c <= 'F') ? (c - 'A' + 10) : 0);
        if (i % 2 == 0) res.coef[i / 2] |= v;
        else res.coef[i / 2] |= (v << 4);
    }
    res.len = (n + 1) / 2; res.normalize();
    bn = res;
    return in;
}
ostream& operator<<(ostream& out, const BigNumber& bn) {
    out << hex << (int)bn.coef[bn.len - 1];
    for (int i = bn.len - 2; i >= 0; i--) out << setfill('0') << setw(2) << (int)bn.coef[i];
    return out << dec;
}

string BigNumber::toDecimal() const {
    if (len == 1 && coef[0] == 0) return "0";
    BigNumber tmp(*this); string s = "";
    while (!(tmp.len == 1 && tmp.coef[0] == 0)) {
        s += to_string(tmp % (BASE)10);
        tmp /= (BASE)10;
    }
    reverse(s.begin(), s.end()); return s;
}

// тест и демонстрация
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
    srand((unsigned)time(0));
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