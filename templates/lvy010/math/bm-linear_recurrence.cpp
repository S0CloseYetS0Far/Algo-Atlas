#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;
using ll = long long;
using Poly = vector<ll>;

ll mod_pow(ll a, ll b) {
    ll res = 1;
    while (b) {
        if (b & 1) res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}

vector<ll> berlekamp_massey(const vector<ll>& s) {
    int n = s.size();
    vector<ll> C(n), B(n);
    C[0] = B[0] = 1;
    int L = 0, m = 1;
    ll b = 1;
    for (int i = 0; i < n; ++i) {
        ll d = s[i] % MOD;
        for (int j = 1; j <= L; ++j)
            d = (d + C[j] * s[i - j]) % MOD;
        if (d == 0) {
            m++;
            continue;
        }
        ll coef = d * mod_pow(b, MOD - 2) % MOD;
        vector<ll> T = C;
        for (int j = m; j < n; ++j)
            C[j] = (C[j] - coef * B[j - m] % MOD + MOD) % MOD;
        if (2 * L <= i) {
            L = i + 1 - L;
            B = T;
            b = d;
            m = 1;
        } else {
            m++;
        }
    }
    C.resize(L + 1);
    C.erase(C.begin());
    for (auto& x : C) x = (MOD - x) % MOD;
    return C;
}

Poly operator*(const Poly& a, const Poly& b) {
    int n = a.size(), m = b.size();
    Poly res(n + m - 1, 0);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            res[i + j] = (res[i + j] + a[i] * b[j]) % MOD;
    return res;
}

Poly operator%(Poly a, const Poly& b) {
    int n = a.size(), m = b.size();
    if (n < m) return a;
    ll inv = mod_pow(b.back(), MOD - 2);
    for (int i = n - 1; i >= m - 1; --i) {
        ll coef = a[i] * inv % MOD;
        for (int j = 0; j < m; ++j)
            a[i - j] = (a[i - j] - coef * b[m - 1 - j] % MOD + MOD) % MOD;
    }
    a.resize(m - 1);
    return a;
}

Poly poly_pow(Poly a, ll k, const Poly& mod) {
    Poly res = {1};
    while (k) {
        if (k & 1) res = (res * a) % mod;
        a = (a * a) % mod;
        k >>= 1;
    }
    return res;
}

ll bostan_mori(const vector<ll>& rec, const vector<ll>& init, ll n) {
    int k = rec.size();
    Poly Q(k + 1);
    Q[0] = 1;
    for (int i = 0; i < k; ++i)
        Q[i + 1] = (MOD - rec[i]) % MOD;
    Poly P(init.begin(), init.begin() + k);
    P = P * Q;
    P.resize(k);
    Poly x = {0, 1};
    x = poly_pow(x, n, Q);
    P = P * x % Q;
    return P[0];
}

int main() {
    // example: the first few Fibonacci numbers 0, 1, 1, 2, 3, 5, 8, 13, 21, 34
    vector<ll> s = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34};
    vector<ll> rec = berlekamp_massey(s);
    
    cout << "Shortest linear recurrence: ";
    for (ll x : rec) cout << x << " ";
    cout << endl;
    
    ll n = 100;
    ll ans = bostan_mori(rec, s, n);
    cout << "Fibonacci term #" << n << ": " << ans << endl;
    
    return 0;
}
