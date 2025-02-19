#include "../include/utils.h"

int _abs(int n) {
    return (n < 0) ? -n : n;
}

int _min(int n1, int n2) {
    if (n1 < n2) {
        return n1;
    }
    return n2;
}

int _max(int n1, int n2) {
    if (n1 > n2) {
        return n1;
    }
    return n2;
}
