#include "bigNumber.h"
#include <cstring>
#include <random>
#include <algorithm>
using namespace std;



BigNumber::BigNumber(int mLen, int mode) : len(1), maxLen(mLen) {
    coef = new BASE[maxLen]();
    if (mode == 1) {
         mt19937_64 rng(random_device{}());
         uniform_int_distribution<BASE> dist(0, ~(BASE)0);
        for (int i = 0; i < maxLen; i++)
            coef[i] = dist(rng);
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
        len = bn.len; maxLen = bn.maxLen;
        coef = new BASE[maxLen];
        memcpy(coef, bn.coef, maxLen * sizeof(BASE));
    }
    return *this;
}

// ─── Normalize ─────────────────────────────────────────────────────────────

void BigNumber::normalize() {
    while (len > 1 && coef[len - 1] == 0) len--;
}

// ─── Comparison ────────────────────────────────────────────────────────────

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
bool BigNumber::operator> (const BigNumber& bn) const { return  bn < *this; }
bool BigNumber::operator<=(const BigNumber& bn) const { return !(*this > bn); }
bool BigNumber::operator>=(const BigNumber& bn) const { return !(*this < bn); }

// ─── Addition ──────────────────────────────────────────────────────────────
BigNumber BigNumber::operator+(const BigNumber& bn) const {
    int rL = max(len, bn.len) + 1;
    BigNumber res(rL, 0);
    DBASE k = 0;
    for (int j = 0; j < rL; j++) {
        DBASE a = (j < len)    ? (DBASE)coef[j]    : 0;
        DBASE b = (j < bn.len) ? (DBASE)bn.coef[j] : 0;
        DBASE t = a + b + k;
        res.coef[j] = (BASE)(t);          
        k           = t >> BASE_SIZE;     
    }
    res.len = rL;
    res.normalize();
    return res;
}
BigNumber& BigNumber::operator+=(const BigNumber& bn) { return *this = *this + bn; }

BigNumber BigNumber::operator+(BASE v) const {
    BigNumber tmp(2, 0); tmp.coef[0] = v; tmp.len = 1;
    return *this + tmp;
}
BigNumber& BigNumber::operator+=(BASE v) { return *this = *this + v; }

// ─── Subtraction ───────────────────────────────────────────────────────────

BigNumber BigNumber::operator-(const BigNumber& bn) const {
    if (*this < bn) return BigNumber(1, 0);
    BigNumber res(*this);
    DBASE k = 0;
    for (int i = 0; i < len; i++) {
        DBASE v   = (i < bn.len) ? (DBASE)bn.coef[i] : 0;
        DBASE tmp = BASE_VAL | (DBASE)res.coef[i];  
        tmp = tmp - v - k;   
        res.coef[i] = (BASE)(tmp);                 
        k           = !(tmp >> BASE_SIZE);             
    }
    res.normalize();
    return res;
}
BigNumber& BigNumber::operator-=(const BigNumber& bn) { return *this = *this - bn; }

BigNumber BigNumber::operator-(BASE v) const {
    BigNumber tmp(1, 0); tmp.coef[0] = v; tmp.len = 1;
    return *this - tmp;
}
BigNumber& BigNumber::operator-=(BASE v) { return *this = *this - v; }

// ─── Multiplication (big × big ) ────────────────────────────────────────────


BigNumber BigNumber::operator*(const BigNumber& bn) const {
    if ((len == 1 && coef[0] == 0) || (bn.len == 1 && bn.coef[0] == 0)) return BigNumber(1, 0);
    BigNumber w(len + bn.len, 0);
    w.len = len + bn.len;
    for (int j = 0; j < bn.len; j++) {
        if (bn.coef[j] == 0) continue;
        DBASE k = 0;
        for (int i = 0; i < len; i++) {
            DBASE t = (DBASE)coef[i] * (DBASE)bn.coef[j]
                    + (DBASE)w.coef[i + j] + k;
            w.coef[i + j] = (BASE)(t);        
            k              = t >> BASE_SIZE;  
        }
        w.coef[len + j] = (BASE)k;
    }
    w.normalize();
    return w;
}
BigNumber& BigNumber::operator*=(const BigNumber& bn) { return *this = *this * bn; }

BigNumber BigNumber::operator*(BASE v) const {
 if (v == 0) return BigNumber(1, 0);

    int len_w = len + 1;
    BigNumber w(len_w, 0);
    w.len = len_w;


    int j = 0;
    DBASE k = 0;

    while (j < len) {
        DBASE tmp = (DBASE)coef[j] * (DBASE)v + k;
        w.coef[j] = (BASE)(tmp);
        k         = tmp >> BASE_SIZE;
        j++;
    }

    if (k != 0) {
        w.coef[j] = (BASE)k;
    } else {
        w.len--;
    }

    return w;
}
BigNumber& BigNumber::operator*=(BASE v) { return *this = *this * v; }

// ─── Division (big / big) ──────────────────────────────

BigNumber BigNumber::operator/(const BigNumber& bn) const {
    if (bn.len == 1 && bn.coef[0] == 0)
        return BigNumber(1, 0);

    if (*this < bn)
        return BigNumber(1, 0);

    if (bn.len == 1)
        return *this / bn.coef[0];

    int n = bn.len;       // length of v
    int m = len - n;      // u has length m + n
    DBASE mask = BASE_VAL - 1;

    // D1. Normalization.
    DBASE d = BASE_VAL / ((DBASE)bn.coef[n - 1] + 1);
    BigNumber v = bn * (BASE)d;
    BigNumber uNorm = *this * (BASE)d;

    // Add an extra high zero digit: u = (u_{m+n} ... u_0), u_{m+n} = 0.
    BigNumber u(m + n + 1, 0);
    for (int i = 0; i < uNorm.len; i++) {
        u.coef[i] = uNorm.coef[i];
    }
    u.len = m + n + 1;

    BigNumber q(m + 1, 0);
    q.len = m + 1;

    // D2. Initial j.
    int j = m;
    while (j >= 0) {
        // D3. Compute q' and r'.
        DBASE numerator = ((DBASE)u.coef[j + n] << BASE_SIZE) + (DBASE)u.coef[j + n - 1];
        DBASE q_ = numerator / (DBASE)v.coef[n - 1];
        DBASE r_ = numerator % (DBASE)v.coef[n - 1];

        if (q_ == BASE_VAL) {
            q_ = BASE_VAL - 1;
            r_ += (DBASE)v.coef[n - 1];
        }

        while (n > 1) {
            DBASE left = q_ * (DBASE)v.coef[n - 2];
            DBASE right = (r_ << BASE_SIZE) + (DBASE)u.coef[j + n - 2];
            if (left <= right) break;

            q_--;
            r_ += (DBASE)v.coef[n - 1];
            if (r_ >= BASE_VAL) break;
        }

        // D4. (u_{j+n} ... u_j) -= q' * (v_{n-1} ... v_0).
        // carryBorrow is the high part of q' * v_i plus borrow from subtraction.
        DBASE k = 0;
        for (int i = 0; i < n; i++) {
            DBASE p = q_ * (DBASE)v.coef[i];
            DBASE sub = (p & mask) + k;
            DBASE borrow = sub >> BASE_SIZE;
            sub &= mask;

            if ((DBASE)u.coef[j + i] < sub) {
                u.coef[j + i] = (BASE)((DBASE)u.coef[j + i] + BASE_VAL - sub);
                borrow++;
            } else {
                u.coef[j + i] = (BASE)((DBASE)u.coef[j + i] - sub);
            }

            k = (p >> BASE_SIZE) + borrow;
        }

        bool kBorrow = false;
        if ((DBASE)u.coef[j + n] < k) {
            u.coef[j + n] = (BASE)((DBASE)u.coef[j + n] + BASE_VAL - k);
            kBorrow = true;
        } else {
            u.coef[j + n] = (BASE)((DBASE)u.coef[j + n] - k);
        }

        // D5-D6. q_j = q'. If k = 1, compensate by q_j-- and add v back.
        if (kBorrow) {
            q_--;
            DBASE carry = 0;
            for (int i = 0; i < n; i++) {
                DBASE sum = (DBASE)u.coef[j + i] + (DBASE)v.coef[i] + carry;
                u.coef[j + i] = (BASE)(sum & mask);
                carry = sum >> BASE_SIZE;
            }
            u.coef[j + n] = (BASE)((DBASE)u.coef[j + n] + carry);
        }

        q.coef[j] = (BASE)q_;

        // D7.
        j--;
    }

    // D8 is needed for the normalized remainder. operator/ returns only q.
    q.normalize();
    return q;
}
BigNumber& BigNumber::operator/=(const BigNumber& bn) { return *this = *this / bn; }

// ─── Division by single BASE digit ─────────────────────────────────────────

BigNumber BigNumber::operator/(BASE v) const {
    if (v == 0) return BigNumber(1, 0);

    BigNumber res(*this);
    DBASE r = 0;
    for (int j = len - 1; j >= 0; j--) {
        DBASE tmp = (r << BASE_SIZE) + (DBASE)coef[j];
        res.coef[j] = (BASE)(tmp / (DBASE)v);
        r = tmp % (DBASE)v;
    }
    res.normalize();
    return res;
}
BigNumber& BigNumber::operator/=(BASE v) { return *this = *this / v; }

// ─── Modulo ────────────────────────────────────────────────────────────────

BigNumber BigNumber::operator%(const BigNumber& bn) const {
    return *this - (*this / bn) * bn;
}
BigNumber& BigNumber::operator%=(const BigNumber& bn) { return *this = *this % bn; }


BASE BigNumber::operator%(BASE v) const {
    DBASE r = 0;
    for (int j = len - 1; j >= 0; j--)
        r = ((r << BASE_SIZE) + (DBASE)coef[j]) % (DBASE)v;
    return (BASE)r;
}

// ─── Hex input/output ───────────────────────────────────────────────────

istream& operator>>(istream& in, BigNumber& bn) {
    string s; in >> s;

    int n = (strlen(s.c_str()) - 1) / (sizeof(BASE) * 2) + 1;
    BigNumber res(n, 0);
    res.len = n;
    int j = 0;
    int k = 0;
    int i = (int)s.size() - 1;

    while (i >= 0) {
        int tmp = 0;
        if ('0' <= s[i] && s[i] <= '9') tmp = s[i] - '0';
        else if ('a' <= s[i] && s[i] <= 'f') tmp = s[i] - 'a' + 10;
        else if ('A' <= s[i] && s[i] <= 'F') tmp = s[i] - 'A' + 10;
        else {
            bn = BigNumber(1, 0);
            return in;
        }

        res.coef[j] |= (BASE)(tmp << k);
        k += 4;
        if (k >= (int)BASE_SIZE) { k = 0; j++; }
        i--;
    }

    res.normalize();
    bn = res;
    return in;
}

ostream& operator<<(ostream& out, const BigNumber& bn) {

    out << hex;
    int j = bn.len - 1;
    out << (unsigned long long)bn.coef[j--];
    while (j >= 0) {
        out.width(BASE_SIZE / 4);
        out.fill('0');
        out << (unsigned long long)bn.coef[j];
        j--;
    }
    return out;
}

// ─── Decimal input ──────────────────────────────────────────────────────

void BigNumber::inputDecimal(istream& in) {
    string s;
    in >> s;

    BigNumber res(1, 0);
    for (int i = 0; i < (int)s.size(); i++) {
        if (s[i] < '0' || s[i] > '9') {
            *this = BigNumber(1, 0);
            return;
        }
        res *= (BASE)10;
        res += (BASE)(s[i] - '0');
    }

    res.normalize();
    *this = res;
}

// ─── Decimal conversion ────────────────────────────────────────────────────

string BigNumber::toDecimal() const {
    if (len == 1 && coef[0] == 0) return "0";
    BigNumber tmp(*this);
    string s;
    while (!(tmp.len == 1 && tmp.coef[0] == 0)) {
        s += to_string(tmp % (BASE)10);
        tmp /= (BASE)10;
    }
    reverse(s.begin(), s.end());
    return s;
}

// ─── Test ──────────────────────────────────────────────────────────────────

void runTest() {
    cout << "Running 1000 random division tests (base 2^" << BASE_SIZE << ")..." << endl;
        mt19937_64 rng(random_device{}());
        uniform_int_distribution<int> distA(1, 6);
        uniform_int_distribution<int> distD(1,3 );
    int passed = 0, N = 1000;
    while (passed < N) {
        BigNumber A(distA(rng), 1);
        BigNumber D(distD(rng), 1);
        if (D.len == 1 && D.coef[0] == 0) continue;
        BigNumber Q = A / D;
        BigNumber R = A % D;
        BigNumber check = Q * D + R;
        if (!(check == A && R < D)) {
            cout << "FAILED on test " << passed << endl;
            cout << "A = " << A.toDecimal() << endl;
            cout << "D = " << D.toDecimal() << endl;
            cout << "Q = " << Q.toDecimal() << endl;
            cout << "R = " << R.toDecimal() << endl;
            cout << "Q*D+R = " << check.toDecimal() << endl;
            return;
        }
        passed++;
    }
    cout << "PASSED all " << N << " tests!" << endl;
}

// ─── Main ──────────────────────────────────────────────────────────────────

int main() {
    runTest();

    cout << "\n--- DEMONSTRATION ---" << endl;
    BigNumber n1(3, 1), n2(5, 1);

    cout << "n1 hex: " << n1 << endl;
    cout << "n1 dec: " << n1.toDecimal() << endl;
    cout << "n2 hex: " << n2 << endl;
    cout << "n2 dec: " << n2.toDecimal() << endl;

   
cout<< (n2 /n1).toDecimal();
BigNumber n ;
n.inputDecimal(cin);
cout<<n.toDecimal();


    // cout<< (n1 == n2);
    // cout<< (n1 != n2);
    // cout<< (n1 <= n2);
    // cout<< (n1 >= n2);
    // cout<< (n1 > n2);
    // cout<< (n1 < n2);
    // BigNumber prod = n1 * n2;
    // BigNumber quot = n1 / n2;
    // BigNumber rem  = n1 % n2;

    // cout << "\nSum:  " << sum.toDecimal()  << endl;
    // cout << "Diff: " << diff.toDecimal() << endl;
    // cout << "Prod: " << prod.toDecimal() << endl;
    // cout << "Quot: " << quot.toDecimal() << endl;
    // cout << "Rem:  " << rem.toDecimal()  << endl;
 

   

    cout << "\nDONE." << endl;
    return 0;
}
