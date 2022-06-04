#include <algorithm>
#include <iostream>
#include "include.h"

using namespace std;

vector<int> partition_parallel(string &t, string &p) {
    int N = t.length(), M = p.length();
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
#pragma omp parallel for schedule(static, 1) num_threads(4) proc_bind(spread)
    for (int ind = 0; ind < range.size(); ind++) {
        int begin = range[ind].first;
        int end = range[ind].second;
        int n = end - begin, m = K + 1;
        vector<int> thread_ans;
        vector<vector<bitset<P_LEN + 1>>> dp(3, vector<bitset<P_LEN + 1>>(max(m, n), init));

        dp[0][0].reset(0);
        dp[1][0].reset(0);
        dp[1][0].reset(1);
        dp[1][1] = (dp[0][0] | patternMask[mp[t[begin+1]]]) << 1;
        for (int i = 2; i < m; i++) {
            // init first guy
            for (int j = 0; j <= i; j++) {
                dp[2][0].reset(j);
            }
            // init last guy
            dp[2][i] = (dp[1][i-1] | patternMask[mp[t[begin+i]]]) << 1;

            // do compute
#pragma omp parallel for schedule(static, 1) num_threads(2) proc_bind(close)
            for(int j = 1; j < i; j++) {
                dp[2][j] = ((dp[1][j-1] | patternMask[mp[t[begin+j]]]) << 1) & ((dp[0][j-1] & dp[1][j]) << 1) & dp[0][j-1];
            }
                swap(dp[0], dp[1]);
                swap(dp[1], dp[2]);
        }
        // check dp[2][0] (bottom left corner)??

        // init first diagonal, i = m
        dp[2][m-1] = (dp[1][m-1] | patternMask[mp[t[begin+m]]]) << 1;
#pragma omp parallel for schedule(static, 1) num_threads(2) proc_bind(close)
        for(int j = 0; j < m-1; j++) {
            dp[2][j] =
                    ((dp[1][j - 1] | patternMask[mp[t[begin + j + 1]]]) << 1) & ((dp[0][j] & dp[1][j]) << 1) & dp[0][j];
        }
        if(!dp[2][0][M]) thread_ans.push_back(begin+1);
        swap(dp[0], dp[1]);
        swap(dp[1], dp[2]);

        for(int i = m+1; i < n; i++) {
            dp[2][m-1] = (dp[1][m-1] | patternMask[mp[t[begin+i]]]) << 1;
#pragma omp parallel for schedule(static, 1) num_threads(2) proc_bind(close)
            for(int j = 0; j < m-1; j++) {
                dp[2][j] = ((dp[1][j] | patternMask[mp[t[begin + i + j - m + 1]]]) << 1) &
                           ((dp[0][j + 1] & dp[1][j + 1]) << 1) & dp[0][j + 1];
            }
                if(!dp[2][0][M]) thread_ans.push_back(begin+i-m+1);
                swap(dp[0], dp[1]);
                swap(dp[1], dp[2]);
        }

        for(int i = n; i < n + m - 1; i++) {
            // m + n - 1 - i elements on diagonal
#pragma omp parallel for schedule(static, 1) num_threads(2) proc_bind(close)
            for(int j = 0; j < m + n - i - 1; j++) {
                dp[2][j] = ((dp[1][j] | patternMask[mp[t[begin+i+j-m+1]]]) << 1) & ((dp[0][j+1] & dp[1][j+1]) << 1) & dp[0][j+1];
            }
                if(!dp[2][0][M]) thread_ans.push_back(begin+i-m+1);
                swap(dp[0], dp[1]);
                swap(dp[1], dp[2]);
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
