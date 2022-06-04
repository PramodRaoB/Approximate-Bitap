#ifndef APPROX_BITAP_INCLUDE_H
#define APPROX_BITAP_INCLUDE_H

#include <bitset>
#include <fstream>
#include <vector>
#include <map>

//#define K 700

const int alpha = 4;

const long long text_len[] = {0, 5154862, 5154862, 5154862, 5154862, 5154862, 5154862, 5154862};
const long long pattern_len[] = {0, 100, 248, 306, 390, 460, 11532, 22539};

const std::string text_file[] = {"",
                                 "sequences/text_1.txt",
                                 "sequences/text_1.txt",
                                 "sequences/text_1.txt",
                                 "sequences/text_1.txt",
                                 "sequences/text_1.txt",
                                 "sequences/text_1.txt",
                                 "sequences/text_1.txt",
                                 };

const std::string pattern_file[] = {"",
                                    "sequences/pattern_1.txt",
                                    "sequences/pattern_2.txt",
                                    "sequences/pattern_3.txt",
                                    "sequences/pattern_4.txt",
                                    "sequences/pattern_5.txt",
                                    "sequences/pattern_6.txt",
                                    "sequences/pattern_7.txt",
                                    };

#define NUM_SRS 5
#define NUM_LRS 2

#ifndef INPUT
#define INPUT 0
#endif

#if INPUT <= NUM_SRS
#define P_LEN 500
#define T_LEN 5154862
// approx 5% error rate for SRS
#define K 15
#elif INPUT <= NUM_SRS + NUM_LRS
#define P_LEN 25000
#define T_LEN 5154862
// approx 15% error rate for LRS
#define K 1000
#endif

const std::string TEXT = text_file[INPUT];
const std::string PATTERN = pattern_file[INPUT];

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
