#ifndef GENTIC_TSP_H_
#define GENTIC_TSP_H_
#include <iostream>
#include <vector>
#include <string>
#define CITIES 99
#define UNITS 500
#define MAX_GEN 20


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


class GenticTSP{
    public:
        GenticTSP();
        void initMatrix(const std::string &filename);//file to matrix
        void initGroup();//shuffle a group
        double lenOfUnit(unit &p);//
        int searchCity(unit &, int id);//search a city's id for cross
        void selectGroup();
        void crossUnits(unit &p, unit &q, unit &r);
        void mutateGroup();
        void evoluteGroup();
        void printBestUnit();
        ~GenticTSP();
    private:
     //   city cities[CITIES];
     //   unit grup[UNITS]
     //   double cityMatrix[CITIES][CITIES];
        unit bestUnit_;
        double pm_;// mutate probability
        double ps_;//save probability

};

#endif  /*GENTIC_TSP_H_*/
