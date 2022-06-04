#ifndef APPROX_BITAP_INCLUDE_H
#define APPROX_BITAP_INCLUDE_H

#include <bitset>
#include <fstream>
#include <vector>
#include <map>

#ifdef SMALL
#define INP 0
#define P_LEN 100
#define T_LEN 10035
#endif
#ifdef MED
#define P_LEN 490
#define T_LEN 3147090
#define INP 1
#endif
#ifdef LARGE
#define P_LEN 3366
#define T_LEN 5154862
#define INP 2
#endif

#define K 1000

const int alpha = 4;

const long long text_len[] = {10035, 3147090, 5154862};
const long long pattern_len[] = {100, 490, 140};

const std::string text_file[] = {"sequences/small_t.txt", "sequences/med_t.txt",
                                 "sequences/large_t.txt"};
const std::string pattern_file[] = {
        "sequences/small_p.txt", "sequences/med_p.txt", "sequences/large_p.txt"};

const std::string TEXT = text_file[INP];
const std::string PATTERN = pattern_file[INP];

#define READ_FILE(f, str)                                                      \
  string __attribute__((aligned(16))) (str);                                   \
  file.open((f).c_str());                                                      \
  while (!file.eof()) {                                                        \
    string line;                                                               \
    file >> line;                                                              \
    (str).append(line);                                                        \
  }                                                                            \
  file.close();

void tick(struct timeval *t);

double tock(struct timeval *t);

std::vector<int> baseline(std::string &t, std::string &p);
std::vector<int> partition(std::string &t, std::string &p);
std::vector<int> verifier(std::string &t, std::string &p, std::vector<int> &pos);

#endif // APPROX_BITAP_INCLUDE_H
