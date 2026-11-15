#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <climits>
using namespace std;

const int MAXN = 30;
const int MAXE = 10;
const int NAME_LEN = 30;

struct Edge {
    int to;
    int cost;
};

class Location {
public:
    char name[NAME_LEN];
    Edge edges[MAXE];
    int edgeCount;

    Location() {
        name[0] = '\0';
        edgeCount = 0;
    }
};
