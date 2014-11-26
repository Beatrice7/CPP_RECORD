#ifndef STRINGUTILS_H
#define STRINGUTILS_H 
#include <string>
#include <vector>
namespace stringutils
{
    int minValue(int a, int b, int c);
    void handleLine(std::string &line);
    int distanceOfString(const std::string &strA, const std::string &strB);
    int getLenOfUTF8(unsigned char c);
    void parseUTF8String(const std::string &s, std::vector<uint32_t> &vec);
    int distanceOfUint32(const std::vector<uint32_t> &w1, const std::vector<uint32_t> &w2);
    int distanceOfStringCn(const std::string &a, const std::string &b);

    struct matchWord
    {
        std::string word_;
        int count_;
        int distance_;
        explicit matchWord(const std::string &word, int count, int distance)
            :word_(word), count_(count), distance_(distance) {}
        bool operator<(const matchWord &A) const
        {
            return distance_ != A.distance_ ?
                (distance_ > A.distance_) :
                (count_ < A.count_);
        }
    };
}

#endif  /*STRINGUTILS_H*/
