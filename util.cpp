//
// Created by Paul Sanzenbacher on 03.12.16
//


#include "util.h"

using namespace std;
using namespace cv;

vector<Point> roundPoints(vector<Point2f> &fpoints) {
    vector<Point> roundedPoints;
    for (Point2f fpoint : fpoints) {
        roundedPoints.push_back(Point((int)roundf(fpoint.x), (int)roundf(fpoint.y)));
    }
    return roundedPoints;
}

int partition(float * a, int l, int r) {
    float pivot = a[l], t;
    int i = l, j = r + 1;

    while( 1)
    {
        do ++i; while( a[i] <= pivot && i <= r );
        do --j; while( a[j] > pivot );
        if( i >= j ) break;
        t = a[i]; a[i] = a[j]; a[j] = t;
    }
    t = a[l]; a[l] = a[j]; a[j] = t;
    return j;
}

void quickSort( float * a, int l, int r)
{
    int j;

    if( l < r )
    {
        // divide and conquer
        j = partition( a, l, r);
        quickSort( a, l, j-1);
        quickSort( a, j+1, r);
    }

}


float medianOfFloatArray(float * a, unsigned int n) {
    if (n > 2) {
        quickSort(a, 0, n - 1);
    }
    if (n % 2 == 0) {
        return 0.5f * (a[n / 2] + a[n / 2 - 1]);
    }
    else {
        return a[n / 2];
    }
}

float powern(float a, unsigned int n) {
    if (n == 0) {
        return 1;
    }
    else if (n % 2 == 0) {
        float sq = powern(a, n / 2);
        return sq * sq;
    }
    else {
        return a * powern(a, n - 1);
    }
}