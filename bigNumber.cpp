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
    BigNumber tmp(1, 0); tmp.coef[0] = v; tmp.len = 1;
    return *this * tmp;
}
BigNumber& BigNumber::operator*=(BASE v) { return *this = *this * v; }

// ─── mul_scalar ────────────────────────────────────────────────────────────
BigNumber BigNumber::mul_scalar(DBASE v) const {
    if (v == 0) return BigNumber(1, 0);
    BigNumber res(len + 4, 0);
    DBASE carry = 0;
    for (int i = 0; i < len; i++) {
        DBASE t     = (DBASE)coef[i] * v + carry;
        res.coef[i] = (BASE)(t);         
        carry        = t >> BASE_SIZE;    
    }
    int idx = len;
    while (carry) {
        res.coef[idx++] = (BASE)(carry);
        carry >>= BASE_SIZE;
    }
    res.len = max(1, idx);
    res.normalize();
    return res;
}

// ─── Division (big / big) ──────────────────────────────

BigNumber BigNumber::operator/(const BigNumber& bn) const {
    if (bn.len == 1 && bn.coef[0] == 0) return BigNumber(1, 0); // div by 0
    if (*this < bn)                       return BigNumber(1, 0); // u < v
    if (bn.len == 1) {
        // Single-digit divisor: simple loop
        BigNumber res(*this);
        DBASE r = 0;
        for (int j = len - 1; j >= 0; j--) {
            DBASE t     = (r << BASE_SIZE) + (DBASE)coef[j];  // r*b + u_j
            res.coef[j] = (BASE)(t / (DBASE)bn.coef[0]);
            r            = t % (DBASE)bn.coef[0];
        }
        res.normalize();
        return res;
    }

    int n = bn.len;
    int m = len - n;  

    // D1. Normalization

    DBASE d = BASE_VAL / ((DBASE)bn.coef[n - 1] + 1);

    BigNumber v   = bn.mul_scalar(d);
    BigNumber u_p = this->mul_scalar(d);


    BigNumber u(u_p.len + 1, 0);
    memcpy(u.coef, u_p.coef, u_p.len * sizeof(BASE));
    u.len = u_p.len + 1;

    BigNumber q(m + 1, 0);
    q.len = m + 1;

    // D2-D7 main loop
    for (int j = m; j >= 0; j--) {
        // D3. Compute q̂
        DBASE ujn  = (j + n     < u.len) ? (DBASE)u.coef[j + n]     : 0;
        DBASE ujn1 = (j + n - 1 < u.len) ? (DBASE)u.coef[j + n - 1] : 0;
        DBASE ujn2 = (j + n - 2 >= 0 && j + n - 2 < u.len)
                        ? (DBASE)u.coef[j + n - 2] : 0;

        DBASE vn1 = (DBASE)v.coef[n - 1];
        DBASE vn2 = (n >= 2) ? (DBASE)v.coef[n - 2] : 0;

        
        DBASE num = (ujn << BASE_SIZE) | ujn1;
        DBASE qh  = num / vn1;
        DBASE rh  = num % vn1;

        
        if (qh >= BASE_VAL) { qh = BASE_VAL - 1; rh = num - qh * vn1; }

        
        while (rh < BASE_VAL) {
            
            DBASE lhs = qh * vn2;
            DBASE rhs = (rh << BASE_SIZE) | ujn2;
            if (lhs <= rhs) break;
            qh--;
            rh += vn1;
        }

        // D4.
        DBASE k = 0;
        for (int i = 0; i < n; i++) {
            DBASE p   = qh * (DBASE)v.coef[i] + k;
            BASE  lo  = (BASE)(p);           // p % 2^32
            DBASE nk  = p >> BASE_SIZE;      // p / 2^32
            int   idx = j + i;
            DBASE uij = (idx < u.len) ? (DBASE)u.coef[idx] : 0;
            if (uij < (DBASE)lo) {
                u.coef[idx] = (BASE)(uij + BASE_VAL - (DBASE)lo);
                nk++;
            } else {
                u.coef[idx] = (BASE)(uij - (DBASE)lo);
            }
            k = nk;
        }

        // D5. 
        bool need_compensate = false;
        if ((j + n) < u.len) {
            if ((DBASE)u.coef[j + n] < k) {
                need_compensate = true;
            } else {
                u.coef[j + n] = (BASE)((DBASE)u.coef[j + n] - k);
            }
        }

        // D6. 
        if (need_compensate) {
            qh--;
            DBASE ak = 0;
            for (int i = 0; i < n; i++) {
                DBASE t       = (DBASE)u.coef[i + j] + (DBASE)v.coef[i] + ak;
                u.coef[i + j] = (BASE)(t);
                ak             = t >> BASE_SIZE;
            }
         
        }

        q.coef[j] = (BASE)(qh);   // D5
    }

    q.normalize();
    return q;
}
BigNumber& BigNumber::operator/=(const BigNumber& bn) { return *this = *this / bn; }

// ─── Division by single BASE digit ─────────────────────────────────────────

BigNumber BigNumber::operator/(BASE v) const {
    BigNumber divisor(1, 0); divisor.coef[0] = v; divisor.len = 1;
    return *this / divisor;
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

// ─── Hex ───────────────────────────────────────────────────────────────

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
    cout << "Running 1000 random division tests (base 2^32)..." << endl;
        mt19937_64 rng(random_device{}());
        uniform_int_distribution<int> distA(1, 6);
        uniform_int_distribution<int> distD(1, 3);
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

    BigNumber sum  = n1 + n2;
    BigNumber rus = sum - n1;
    cout<< sum<< endl;
    cout << rus <<endl;
    cout << (rus == n2)<< endl;


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
    BigNumber num ;
    cin >> num;
    cout << num << endl;

   

    cout << "\nDONE." << endl;
    return 0;
}