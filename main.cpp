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

typedef int type;
enum MEASURE {
    Euclidean, Manhattan
};

struct PointC {
    type x;
    type y;
    int c;//cluster
};
static vector<PointC> points;
static Mat image;

void generateRandom(vector<PointC> &points, type minX, type maxX, int num);
void generateRandom(vector<PointC> &points, type minX, type maxX, int num, type minY, type maxY);
vector<vector<PointC> > cluster(vector<PointC>, int k, MEASURE measure = Euclidean);

/****************************
 *
 *
 *
 *
 * main
 *
 *
 *
 ****************************/
int main() {
    try {
        image = Mat::zeros(420, 420, CV_8UC3);
        srand(time(nullptr));
        //vector<Point> points;
        generateRandom(points, 1, 100, 100);


        cluster(points, 2);
        //cluster(points, 4);

        // todo: intra-cluster distance of each cluster
        // todo: sum of intra-cluster distance of each cluster
        // todo: minimum and maximum distances between pairs of clusters
        // todo: plot the points

        return 0;
    } catch (string error) {
        cout << error;
        return 1;
    }

}

void plot(){

    circle(image, Point(32,32), 32, Scalar(0, 0, 255), -1);
    //opencv coordinates are not mathematical coordinates, we need to flip the image
    Mat flipped;
    flip(image,flipped,0);
    imshow("flipped 0", flipped);
    waitKey(0);
}



void plot(vector<PointC> points){
    int padding = 20, size = 400, windowSize = size+padding;
    int scale = 4;
    int p2 = padding/2;

    //opencv coordinates are not mathematical coordinates, we need to flip the image
    for (auto p:points){
        if (p.c < 0)//centroids
            circle(image, Point((p.x*4)+p2,(p.y*4)+p2), 3, Scalar(0, 0, 255), -1);
        else
            circle(image, Point((p.x*4)+p2,(p.y*4)+p2), 3, Scalar(255, 255, 255), -1);
    }

}

void show(){
    Mat flipped;
    flip(image,flipped,0);
    imshow("flipped 0", flipped);
    waitKey(5000);
}


/****************************
 *
 * generate random points
 *
 ****************************/

inline int getRandom(type min, type max) {

    return (rand() % (max - min + 1)) + min;
}

void generateRandom(vector<PointC> &points, type minX, type maxX, int num, type minY, type maxY) {
    PointC point;
    while (num) {
        point.x = getRandom(minX, maxX);// (rand()%(maxX-minX+1))+minX;
        point.y = getRandom(minY, maxY);//(rand()%(maxY-minY+1))+minY;
        points.push_back(point);
        num--;
    }
}

void generateRandom(vector<PointC> &points, type minX, type maxX, int num) {
    generateRandom(points, minX, maxX, num, minX, maxX);
}


type getMaxX() {
    type max = points.at(0).x;
    for (auto point: points) {
        if (point.x > max)
            max = point.x;
    }
    return max;
}

type getMaxY() {
    type max = points.at(0).y;
    for (auto point: points) {
        if (point.y > max)
            max = point.y;
    }
    return max;
}

type getMinX() {
    type min = points.at(0).x;
    for (auto point: points) {
        if (point.x < min)
            min = point.x;
    }
    return min;
}

type getMinY() {
    type min = points.at(0).y;
    for (auto point: points) {
        if (point.y < min)
            min = point.y;
    }
    return min;
}

void initialCenters(vector<PointC> &centroids, int k) {
    generateRandom(centroids, 1, 100, k);


    // make sure that initial centroids are unique
    auto fun = [&centroids](int index) {

        for (int i = index; i < centroids.size() - 1; i++) {
            //todo: compare new generated from beginning if such
            for (int j = i + 1; j < centroids.size(); j++) {
                if (centroids[i].x == centroids[j].x)
                    if (centroids[i].y == centroids[j].y)
                        return i;
            }
        }
        return -1;
    };

    int index = fun(0);

    while (index >= 0) {
        index = fun(index);
        centroids[index].x = getRandom(1, 100);
        centroids[index].y = getRandom(1, 100);
    }

    cout << "Initial centroids:" << endl;
    for (auto &p: centroids) {
        p.c = -1;
        cout << p.x << "," << p.y << endl;
    }
}

inline bool checkCentroids(vector<PointC> &points, vector<PointC> &centroids) {
    vector<PointC> oldcentroids = centroids;
    //checkCentroids the labels and the centorids
    //
    return true;


}

inline void updateLabels(vector<PointC> &points, vector<PointC> &centroids) {

}

vector<vector<PointC> > cluster(vector<PointC> points, int k, MEASURE measure) {
    vector<vector<PointC> > clusters;
    vector<PointC> centroids;

    initialCenters(centroids, k);
    plot(points);
    plot(centroids);
    show();
    return clusters;
    do {
        updateLabels(points, centroids);
    } while (checkCentroids(points, centroids));


    return clusters;
}

