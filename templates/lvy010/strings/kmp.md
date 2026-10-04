

- Find p (the pattern) in the string s (the text)
- Return the starting index of the first match
- Return -1 if not found



1. The full template again

#include <iostream>
#include <vector>
#include <string>
using namespace std;

vector<int> ne;
void get_next(const string &p) {
    int m = p.size(); ne.resize(m);
    for (int i = 1, j = 0; i < m; i++) {
        while (j && p[i] != p[j]) j = ne[j-1];
        if (p[i] == p[j]) j++;
        ne[i] = j;
    }
}

int kmp(const string &s, const string &p) {
    get_next(p);
    int n = s.size(), m = p.size();
    for (int i = 0, j = 0; i < n; i++) {
        while (j && s[i] != p[j]) j = ne[j-1];
        if (s[i] == p[j]) j++;
        if (j == m) return i - m + 1;
    }
    return -1;
}




2. What each part does

① The ne array

A global array that stores:
the length of the longest equal prefix & suffix among the first i characters of the pattern
Purpose: on a mismatch, instead of comparing from the start again, jump directly to the right position

② get_next(p)

Preprocesses the pattern p and computes the ne array
Only needs to run once, very fast

③ kmp(s, p)

- i: walks through the text s, never moves backward
- j: walks through the pattern p, jumps on a mismatch
- Characters equal: i++, j++
- Not equal: jump with j = ne[j-1]
- When j == m: a full match has been found
Returned position: i - m + 1



3. How to use it (example)

int main() {
    string s, p;
    cin >> s >> p;
    int pos = kmp(s, p);
    cout << pos << endl;
    return 0;
}


Input:

abcabcabd
abcab


Returns: 0
because the match starts at index 0.



4. Common modifications in contests (you will definitely use these)

① Find all match positions

Change the return into storing answers:

vector<int> kmp(const string &s, const string &p) {
    get_next(p);
    int n = s.size(), m = p.size();
    vector<int> res;
    for (int i = 0, j = 0; i < n; i++) {
        while (j && s[i] != p[j]) j = ne[j-1];
        if (s[i] == p[j]) j++;
        if (j == m) {
            res.push_back(i - m + 1);
            j = ne[j - 1]; // key: continue matching the next one
        }
    }
    return res;
}


② Find the smallest repeating unit (period)

int m = p.size();
int len = m - ne[m-1];
if (m % len == 0) cout << len << endl; // smallest period




5. One-line summary

- ne: where to jump back to
- i only moves forward
- j jumps to ne[j-1] on a mismatch
- When j == m, a match is found
