#define RESET   "\033[0m"
#define BRED     "\033[1m\033[31m"      /* Bold Red */
#define BGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BCYAN    "\033[1m\033[36m"      /* Bold Cyan */

#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>
#include <iomanip>


using namespace std;

typedef int type;
enum MEASURE{Euclidean, Manhattan};
struct Point{
    type x;
    type y;
    int id;
    int cluster;
};

void generateRandom(vector<Point>& points, type min = 1, type max = 100, int num = 100 );
vector<vector <Point> > cluster(vector <Point>, int k, MEASURE measure = Euclidean);

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
    try{

        vector<Point> points;
        generateRandom(points);

        int clusterCounts[]={2,3};
        cluster(points, 2);
        cluster(points, 4);

        // todo: intra-cluster distance of each cluster
        // todo: sum of intra-cluster distance of each cluster
        // todo: minimum and maximum distances between pairs of clusters
        // todo: plot the points

        return 0;
    } catch(string error){
        cout << error;
        return 1;
    }

}


/****************************
 *
 * generate random points
 *
 ****************************/

void generateRandom(vector<Point>& points, type min, type max, int num){
    srand(time(nullptr)); // use current time as seed for random generator

    Point point;
    while (num){
        point.x = (rand()%(max-min+1))+min;
        point.y = (rand()%(max-min+1))+min;
        points.push_back(point);
        num--;
    }
}

void initialCenters( vector<Point>& centroids, int k ){
    generateRandom(centroids, 1, 100, k);
}

inline bool adjust(vector<Point>& points, vector<Point>& centroids){
    vector <Point> oldcentroids = centroids;
    //adjust the labels and the centorids
    //
    if ()


}

vector<vector <Point> > cluster(vector <Point> points, int k, MEASURE measure ){
    vector<vector <Point> > clusters;
    vector<Point> centroids;

    //todo: now it is random, do it smartly
    initialCenters(centroids, k);

    return clusters;
}

