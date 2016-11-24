//
// Created by Benedikt Boss on 22/11/2016.
//

#include "OpticalFlow.h"


void calculateOpticalFlow(vector<Mat> lowResImg, vector<Point2f> &points) {

    Mat1b m1(10, 10), m2(10, 10);

    TermCriteria termcrit(TermCriteria::COUNT|TermCriteria::EPS,20,0.03);
    Size subPixWinSize(10,10), winSize(31,31);

    vector<Point> v0, v1;
    vector<uchar> status;
    vector<float> err;
    calcOpticalFlowPyrLK(m1, m2, v0, v1, status, err, winSize, 3, termcrit, 0, 0.001);
}