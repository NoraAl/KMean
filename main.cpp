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
static vector <Point> points;

void generateRandom(vector<Point>& points, type minX, type maxX,int num);
void generateRandom(vector<Point>& points, type minX, type maxX,int num,  type minY, type maxY );
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
        srand(time(nullptr));
        //vector<Point> points;
        generateRandom(points,1,100,100);

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

inline int getRandom(type min, type max ){

    return (rand()%(max-min+1))+min;
}

void generateRandom(vector<Point>& points, type minX, type maxX, int num,  type minY, type maxY ){
    Point point;
    while (num){
        point.x = getRandom(minX,maxX);// (rand()%(maxX-minX+1))+minX;
        point.y = getRandom(minY,maxY);//(rand()%(maxY-minY+1))+minY;
        points.push_back(point);
        num--;
    }
}

void generateRandom(vector<Point>& points, type minX, type maxX, int num ){
    generateRandom(points,minX,maxX,num,minX,maxX);
}


type getMaxX(){
    type max = points.at(0).x;
    for (auto point: points){
        if (point.x > max)
            max = point.x;
    }
    return max;
}

type getMaxY(){
    type max = points.at(0).y;
    for (auto point: points){
        if (point.y > max)
            max = point.y;
    }
    return max;
}

type getMinX(){
    type min = points.at(0).x;
    for (auto point: points){
        if (point.x < min)
            min = point.x;
    }
    return min;
}

type getMinY(){
    type min = points.at(0).y;
    for (auto point: points){
        if (point.y < min)
            min = point.y;
    }
    return min;
}

void initialCenters( vector<Point>& centroids, int k ){
    generateRandom(centroids, 1,100, k);

    for (auto p: centroids){
        cout <<p.x<<","<<p.y<<endl;
    }
    cout <<"==============="<<endl;
    auto fun = [&centroids](int index){

        for (int i =index; i< centroids.size()-1; i++) {
            for (int j = i + 1; j < centroids.size(); j++) {
                cout << "comparing"<<i<<","<<j<<" ("<<centroids[i].x<<","<<centroids[i].y<<"),("<<centroids[j].x<<","<<centroids[j].y<<")";
                if (centroids[i].x == centroids[j].x)
                    if (centroids[i].y == centroids[j].y){
                        cout <<" ==\n";
                        return i;

                    }
                cout <<" √\n";
            }
        }
        return -1;
    };

    int index = fun(0);

    while(index >= 0){
        //cout << centroids[index].x<<","<<centroids[index].y<<endl;
        index = fun(index);
        centroids[index].x = getRandom(1,100);
        centroids[index].y = getRandom(1,100);

    }

    cout <<"==============="<<endl;
    for (auto p: centroids){
        cout <<p.x<<","<<p.y<<endl;
    }




}

inline bool checkCentroids(vector<Point> &points, vector<Point> &centroids){
    vector <Point> oldcentroids = centroids;
    //checkCentroids the labels and the centorids
    //
    return true;


}

inline void updateLabels(vector<Point> &points, vector<Point> &centroids){

}

vector<vector <Point> > cluster(vector <Point> points, int k, MEASURE measure ){
    vector<vector <Point> > clusters;
    vector<Point> centroids;

    //todo: now it is random, do it smartly

    initialCenters(centroids, k);
    do{
        updateLabels(points, centroids);
    }
    while (checkCentroids(points, centroids));


    return clusters;
}

