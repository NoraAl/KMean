#ifndef CLUSTER_HPP
#define CLUSTER_HPP


#define RESET   "\033[0m"
#define BRED     "\033[1m\033[31m"      /* Bold Red */
#define BGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BCYAN    "\033[1m\033[36m"      /* Bold Cyan */

#include <iostream>
#include <vector>
#include <iomanip>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;
using namespace cv;

typedef double type;
enum MEASURE {
    Euclidean, Manhattan
};

struct PointC {
    PointC() : x(0), y(0), cluster(0) {}

    PointC(int cluster) : x(0), y(0), cluster(cluster) {}
    PointC(type x, type y, int cluster) : x(x), y(y), cluster(cluster) {}

    double x;
    double y;
    int cluster;
};

// overload equal to check point equality
bool operator==(const PointC &p1, const PointC &p2);

bool operator!=(const PointC &p1, const PointC &p2);

vector<PointC>  generateRandom(vector<PointC> &points, type minX, type maxX, int num, bool centroids = false);

vector<PointC>  generateRandom(vector<PointC> &points, type minX, type maxX, int num, bool centroids, type minY, type maxY);

vector<PointC> initialCenters(int k);

void cluster(int k, MEASURE measure = Euclidean);

void plot(vector<PointC> points, Mat image);

void show(Mat image, MEASURE m, int i);

void plot(vector<PointC>& points, vector<PointC>& centroids, MEASURE measure, int i);

void printPoints(vector<PointC> points) ;


#endif