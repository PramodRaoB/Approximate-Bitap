#define _XOPEN_SOURCE

#include <cstdlib>
#include <algorithm>
#include <sys/time.h>
#include <iostream>
#include "include.h"
using namespace std;

void tick(struct timeval *t) {
    gettimeofday(t, NULL);
}

double tock(struct timeval *t) {
    struct timeval now{};
    gettimeofday(&now, NULL);
    return (double) (now.tv_sec - t->tv_sec) +
           ((double) (now.tv_usec - t->tv_usec) / 1000000.);
}

int solve(string t, string p) {
    reverse(t.begin(), t.end());
    reverse(p.begin(), p.end());
    int n = t.length(), m = p.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    dp[0][0] = 0;
    for (int i = 1; i <= n; i++) dp[0][i] = i;
    for (int i = 1; i <= m; i++) dp[i][0] = i;
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            dp[i][j] = min(dp[i - 1][j], dp[i][j - 1]) + 1;
            dp[i][j] = min(dp[i][j], dp[i - 1][j - 1] + (t[j - 1] != p[i - 1]));
        }
    }
//    for (auto &i: dp) {
//        for (auto &j: i) cout << j << " ";
//        cout << "\n";
//    }
    return *min_element(dp[m].begin(), dp[m].end());
}

vector<int> verifier(string &t, string &p, vector<int> &pos) {
    vector<int> ans;
    for (auto &i: pos) {
        int start = max(0, i - P_LEN - K);
        ans.push_back(solve(t.substr(start, i - start + 1), p));
    }
    return ans;
}
