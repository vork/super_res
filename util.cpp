//
// Created by Paul Sanzenbacher on 03.12.16
//


#include "util.h"
#include <fstream>
#include <iostream>


using namespace std;
using namespace cv;

int mod(int a, int m) {

    if (a < 0) {
        return ((a % m) + m) % m;
    }
    else {
        return a % m;
    }
}

float signf(float val) {

    int intval = (int)val;

    if (intval > 0) {
        return 1;
    }
    else if (intval < 0) {
        return -1;
    }
    return 0;
}

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

Mat1f readMat1fFromFile(string filename, Size size) {
    Mat1f m(size);

    ifstream file(filename);
    if (file.is_open()) {
//        float f;
//        while (file.read((char *)&f, sizeof(float))) {
//            cout << f << endl;
//        }
        file.read((char *)m.data, sizeof(float) * size.width * size.height);
    }
    else {
        cout << "file '" << filename << "' could not be opened" << endl;
    }

    return m;

}

void showMatrixRegion(Mat1f mat, Rect region) {
    Mat1b matb(region.size());
    Mat1f matRegion = mat(region);
    matRegion.convertTo(matb, CV_8UC1);
    cout << matb << endl;
}

cv::Mat1f alignImages(std::vector<Mat1f> images) {
    Size size(0, 0);
    for (Mat1f image : images) {
        size.width += image.cols;
        size.height = max(size.height, image.rows);
    }
    Mat1f aligned(size, 0.0f);
    int x = 0;
    for (Mat1f image : images) {
        Mat1f region = aligned(Rect(Point(x, 0), image.size()));
        image.copyTo(region);
        x += image.cols ;
    }
    return aligned;
}

cv::Mat1f shiftImage(cv::Mat1f image, cv::Point offset, int padding, float fill) {

    Size size(image.cols - 2 * padding, image.rows - 2 * padding);
    assert(size.width > 0 && size.height > 0);

    Mat1f shifted(image.size(), fill);

    Size window(min(image.cols, (image.cols - abs(offset.x))), min(image.rows, image.rows - abs(offset.y)));

    Rect rectShifted(Point(max(0, offset.x), max(0, offset.y)), window);
    Rect rectImage(Point(max(0, -offset.x), max(0, -offset.y)), window);

    image(rectImage).copyTo(shifted(rectShifted));

    // TODO: more efficient implementation
    Mat1f cropped(size);
    shifted(Rect(Point(padding, padding), size)).copyTo(cropped);



    return cropped;
}

