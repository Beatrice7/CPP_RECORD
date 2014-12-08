#ifndef SA_TSP_H_
#define SA_TSP_H_ 
#include <iostream>
#include <vector>
#include <string>
#define CITIES 99
#define INITAL_TEMP 1000
#define MIN_TEMP 10

struct city{
    int id;
    int x;
    int y;
};

struct unit{
    double length;
    std::vector<int> path;
    bool operator<(const struct unit &other) const
    {
        return length < other.length;
    }
};

class SA_TSP
{
    public:
        SA_TSP();
        void initMatrix(const std::string &filename);
        void initBestUnit();
        double lenOfUnit(unit &);
        void genNewUnit(unit &u);
        void simulateAnnealing();
        bool isAccept(unit &, unit &);
        void printBestUnit();
        ~SA_TSP() {}
    private:
        unit bestUnit_;
        double speed_;
        int markov_;
        double currentTemp_;
};

#endif  /*SA_TSP_H_*/
