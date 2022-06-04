#include "include.h"

using namespace std;

vector<int> baseline(string &t, string &p) {
    int N = t.length(), M = p.length();
    bitset<P_LEN + 1> init;
    init.set();
    vector<vector<bitset<P_LEN + 1>>> dp(K + 1, vector<bitset<P_LEN + 1>>(2, init));

    vector<bitset<P_LEN + 1>> patternMask(alpha, init);
    for (int i = 0; i < M; i++)
        patternMask[mp[p[i]]].reset(i);

    vector<int> ans;

    for (int i = 0; i <= K; i++) {
        for (int j = 0; j <= i; j++)
            dp[i][0].reset(j);
    }
    for (int i = 0; i < N; i++) {
//        dp[0][1] = ((dp[0][0] << 1) | patternMask[mp[t[i]]]);
        dp[0][1] = (dp[0][0] | patternMask[mp[t[i]]]) << 1;
        for (int j = 1; j <= K; j++) {
//            dp[j][1] = ((dp[j][0] << 1) | patternMask[mp[t[i]]]) & ((dp[j - 1][0] & dp[j - 1][1]) << 1) & dp[j - 1][0];
            dp[j][1] = ((dp[j][0] | patternMask[mp[t[i]]]) << 1) & ((dp[j - 1][0] & dp[j - 1][1]) << 1) & dp[j - 1][0];
            swap(dp[j - 1][0], dp[j - 1][1]);
        }
        if (!dp[K][1][M]) ans.push_back(i);
        swap(dp[K][0], dp[K][1]);
    }
    return ans;
}