#include "include.h"
#include <iostream>
#include <omp.h>

using namespace std;

std::string text = TEXT;
std::string pattern = PATTERN;
std::ifstream file;

int main() {
#ifdef BUILD
    text = "../" + text;
    pattern = "../" + pattern;
#endif
    READ_FILE(text, t);
    READ_FILE(pattern, p);

    timeval clTime{};
    vector<int> ans;
    tick(&clTime);

#ifdef BASE
//    ans = baseline(t, p);
    ans = partition(t, p);
#endif

    double calcTime = tock(&clTime);
     for (auto &i : ans)
       cout << i << " ";
     cout << "\n";
    cout << "***-----***\n";
    vector<int> verify = verifier(t, p, ans);
     for (auto &i: verify) cout << i << " ";
     cout << "\n";
        cout << "***-----***\n";
        cout << "Length of text: " << t.length() << "\n";
        cout << "Length of pattern: " << p.length() << "\n";
        cout << "Number of matches found: " << ans.size() << "\n";
        cout << "Time Taken(ms): " << calcTime * 1000 << "\n";
    cout << calcTime * 1000;
    return 0;
}
