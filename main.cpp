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

    double x;
    double y;
    int cluster;
};

// overload equal to check point equality
bool operator==(const PointC &lhs, const PointC &rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator!=(const PointC &lhs, const PointC &rhs) {
    return !(lhs == rhs);
}

static vector<PointC> points;
static vector<PointC> centroids;
static Mat image;
static int minP, maxP;

void generateRandom(vector<PointC> &points, type minX, type maxX, int num, bool centroids = false);

void generateRandom(vector<PointC> &points, type minX, type maxX, int num, bool centroids, type minY, type maxY);

void cluster(int k, MEASURE measure = Euclidean);

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
        minP = 1;
        maxP = 100;

        srand(time(nullptr));
        //vector<Point> points;
        generateRandom(points, 1, 100, 100);


        cluster(4);
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


void plot(vector<PointC> points) {
    int padding = 20, size = 400, windowSize = size + padding;
    int scale = 4;
    int p2 = padding / 2;

    vector<Scalar> colors;
    colors.push_back(Scalar(255, 255, 255));
    colors.push_back(Scalar(255, 0, 255));
    colors.push_back(Scalar(0, 255, 0));
    colors.push_back(Scalar(255, 0, 0));
    colors.push_back(Scalar(0, 0, 255));

    //opencv coordinates are not mathematical coordinates, we need to flip the image
    for (auto p:points) {
        if (p.cluster < 0)//centroids
            circle(image, Point((p.x * 4) + p2, (p.y * 4) + p2), 3, colors[colors.size()-1], -1);
        else
            circle(image, Point((p.x * 4) + p2, (p.y * 4) + p2), 2, colors[p.cluster], -1);
    }

}

void show() {
    Mat flipped;
    flip(image, flipped, 0);
    imshow("flipped 0", flipped);
    waitKey(5000);
}


/****************************
 *
 * generate random points
 *
 ****************************/

void printPoint() {
    for (auto p: points) {
        cout << "(" << p.x << "," << p.y << "):" << p.cluster << endl;
    }
}

inline int getRandom(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

void generateRandom(vector<PointC> &points, type minX, type maxX, int num, bool centroids, type minY, type maxY) {
    PointC point = PointC();
    if (centroids)
        point = PointC(-1);
    while (num) {
        point.x = getRandom(minX, maxX);// (rand()%(maxX-minX+1))+minX;
        point.y = getRandom(minY, maxY);//(rand()%(maxY-minY+1))+minY;

        points.push_back(point);
        num--;
    }
}

void generateRandom(vector<PointC> &points, type minX, type maxX, int num, bool centroids) {
    generateRandom(points, minX, maxX, num, centroids, minX, maxX);
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

void initialCenters(int k) {
    generateRandom(centroids, 1, 100, k, true);


    // make sure that initial centroids are unique
    auto fun = [&](int index) {

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
}

// true if centroids changes
inline bool checkCentroids() {
    cout <<"_____________________"<<endl;
    vector<PointC> oldCentroids = centroids;
    for (auto c: centroids) {
        cout <<"********"<<c.x<<","<<c.y<<endl;
    }


    for (int i = 0; i< centroids.size(); i++) {
        int count = 0;
        PointC currentCentroid = PointC(-1);

        for (auto p: points) {
            if (p.cluster == i) {
                currentCentroid.x += p.x;
                currentCentroid.y += p.y;
                count++;
            }
        }

        if (count==0){ //empty cluster, regenerate it
            centroids[i].x = getRandom(minP,maxP);
            centroids[i].y = getRandom(minP,maxP);
        } else{
            //average
            currentCentroid.x = currentCentroid.x / count;
            currentCentroid.y = currentCentroid.y / count;
            //set current centroid
            centroids[i] = currentCentroid;
        }

    }
    cout <<endl;

    for (int i = 0; i < centroids.size(); i++) {
        cout <<"********"<<centroids[i].x<<","<<centroids[i].y<<"\t"<<oldCentroids[i].x<<","<<oldCentroids[i].y<<endl;
    }
    cout <<"_____________________"<<endl;

    for (int i = 0; i < centroids.size(); i++) {
        if (centroids[i] != oldCentroids[i])
            return true;
    }

    return false;


}

inline void updateLabels() {
    for (auto &p: points) {
        double min = DBL_MAX;
        int i = 0;
        for (auto c: centroids) {
            double current = sqrt(pow(p.x - c.x, 2) + pow(p.y - c.y, 2));
            if (current < min) {
                min = current;
                p.cluster = i;//initially all clusters are zero
            }
            i++;

        }
    }
    cout << "==========\nupdating:" << endl;
    printPoint();

}

void plot(){
    image = Mat::zeros(420, 420, CV_8UC3);
    plot(points);
    plot(centroids);
}


void cluster( int k, MEASURE measure) {

    initialCenters( k);
    printPoint();
    //return clusters;
    do {

        cout << "Plotting:" << endl;
        printPoint();
        plot();
        show();
        updateLabels();
    } while (checkCentroids());
}

