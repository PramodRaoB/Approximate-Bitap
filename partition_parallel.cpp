#include <algorithm>
#include <iostream>
#include "include.h"

using namespace std;

#define CHUNK_SIZE (262144)

map<char, int> mp;
int r;

vector<int> bitap_base_parallel(string &t, string &p) {
    int N = t.length(), M = p.length();
    vector<int> ans;
    bitset<P_LEN + 1> R;
    R.set();

    vector<bitset<P_LEN + 1>> pattern_mask(alpha, R);
//   build pattern mask
    for (int i = 0; i < M; i++)
        pattern_mask[mp[p[i]]][i] = false;


    int n = N - r + 1;
    bitset<P_LEN + 1> test;
    for (int i = 0; i < K + 1; i++) test.set(M - i);
#pragma omp parallel for schedule(static, 1)
    for (int chunk_start = 0; chunk_start < n; chunk_start += CHUNK_SIZE) {
        int end = min(chunk_start + CHUNK_SIZE, n);
        int begin = max(chunk_start - r + 1, 0);
        vector<int> thread_ans;
        bitset<P_LEN + 1> S;
        S.set();

        S[0] = false;
        for (int i = begin; i < end; i++) {
            S |= pattern_mask[mp[t[i]]];
            S <<= K + 1;
            if ((test & S).count() < K + 1)
                thread_ans.push_back(i - r + 1);
        }

        if (!thread_ans.empty()) {
#pragma omp critical
            {
                ans.insert(ans.end(), thread_ans.begin(), thread_ans.end());
            }
        }
    }
    return ans;
}


vector<int> partition_parallel(string &t, string &p) {
    int N = t.length(), M = p.length();
    r = M / (K + 1);
    mp['A'] = 0, mp['C'] = 1, mp['G'] = 2, mp['T'] = 3;
    string interleaved(r * (K + 1), 'A');
    for (int i = 0, ind = 0; i < interleaved.length(); i++) {
        interleaved[i] = p[ind];
        ind += r;
        if (ind >= interleaved.length()) ind = (ind % r) + 1;
    }
    vector<int> ret = bitap_base_parallel(t, interleaved);
    cout << "Found exact matches of partitions: " << ret.size() << endl;
    vector<pair<int, int>> range;
    pair<int, int> curr = {-1, -1};
    sort(ret.begin(), ret.end());
    for (auto &i: ret) {
        int L = max(0, i - M - K), R = min(N, i + M + K);
        if (curr.first == -1) {
            curr = {L, R};
            continue;
        }
        if (curr.second >= L) curr.second = R;
        else {
            range.push_back(curr);
            curr = {L, R};
        }
    }
    if (curr.first != -1) range.push_back(curr);
    cout << "Number of ranges to be checked: " << range.size() << endl;

    bitset<P_LEN + 1> init;
    init.set();


//    mp['A'] = 0, mp['B'] = 1, mp['C'] = 2, mp['D'] = 3;
    vector<bitset<P_LEN + 1>> patternMask(alpha, init);

    for (int i = 0; i < M; i++)
        patternMask[mp[p[i]]].reset(i);

    vector<int> ans;
#pragma omp parallel for schedule(static, 1)
    for (int ind = 0; ind < range.size(); ind++) {
        int begin = range[ind].first;
        int end = range[ind].second;
        int n = end - begin, m = K + 1;
        vector<int> thread_ans;
        vector<vector<bitset<P_LEN + 1>>> dp(3, vector<bitset<P_LEN + 1>>(max(m, n), init));

        for (int i = 0; i < m; i++) {
            for (int j = 0; j <= i; j++) {
                dp[2][0].reset(j);
            }
        }
//        for (int i = begin; i < end; i++) {
//            dp[0][1] = (dp[0][0] | patternMask[mp[t[i]]]) << 1;
//            for (int j = 1; j <= K; j++) {
//                dp[j][1] =
//                        ((dp[j][0] | patternMask[mp[t[i]]]) << 1) & ((dp[j - 1][0] & dp[j - 1][1]) << 1) & dp[j - 1][0];
//                swap(dp[j - 1][0], dp[j - 1][1]);
//            }
//            if (!dp[K][1][M]) thread_ans.push_back(i);
//            swap(dp[K][0], dp[K][1]);
//        }
        if (!thread_ans.empty()) {
#pragma omp critical
            {
                ans.insert(ans.end(), thread_ans.begin(), thread_ans.end());
            }
        }
    }
    return ans;
}
