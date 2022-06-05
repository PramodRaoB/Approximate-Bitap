#include "include.h"
#include <iostream>
#include <omp.h>

using namespace std;

std::string text = TEXT;
std::string pattern = PATTERN;
//std::string pattern = "sequences/sd_0001.fastq";
std::ifstream file;

map<char, int> mp;
int r;

int main() {
#ifdef BUILD
    text = "../" + text;
    pattern = "../" + pattern;
#endif
    READ_FILE(text, t);
    READ_FILE(pattern, p);

//long long count = 0;
//  file.open((pattern).c_str());
//  while (!file.eof()) {
//      string p;
//    string line;
//    file >> line;
//    cout << "Id: " << line << endl;
//    file >> p;
//    file >> line;
//    file >> line;
//    if (p.length() >= P_LEN)
//        continue;
//    count++;

    cout << "Reading files: " << "\n" << text << "\n" << pattern << endl;
    cout << "Of lengths: " << T_LEN << " " << P_LEN << endl;

    r = p.length() / (K + 1);
    mp['A'] = 0, mp['C'] = 1, mp['G'] = 2, mp['T'] = 3;

    timeval clTime{};
    vector<int> ans;
    tick(&clTime);

#ifdef BASE
    ans = baseline(t, p);
#endif
#ifdef PART
//    ans = partition(t, p);
    ans = partition_parallel(t, p);
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
//    cout << calcTime * 1000;
//    cout << endl;
//      }
//  file.close();
//  cout << "Count: " << count << "\n";

    return 0;
}
