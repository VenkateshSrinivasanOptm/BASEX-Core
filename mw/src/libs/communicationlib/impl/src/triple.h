#ifndef TRIPLE_H
#define TRIPLE_H

template<typename T1,typename T2, typename T3> struct Triple {
    T1 x;
    T2 y;
    T3 z;
    Triple(T1 _x, T2 _y, T3 _z): x(_x), y(_y), z(_z) {}
    Triple(){}
};
#endif // TRIPLE_H

