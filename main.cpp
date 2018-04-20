#include "cluster.hpp"
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
static int minP, maxP, k;



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
        vector<int> kmean={2,3,4};
        auto ps = generateRandom(points, 1, 100, 100);
        cout <<"Points:"<<endl;
        printPoint(ps);
        for (auto K: kmean){

            k=K;
            cout <<"\n-------------------------------------------\nk is "<<k<<endl;
            centroids.clear();
            vector <PointC> initial = initialCenters(k);

            printPoint(initial);


            cluster(k);
            centroids = initial;
            cluster(k,Manhattan);
        }
        return 0;
    } catch (string error) {
        cout << error;
        return 1;
    }

}



/****************************
 *
 * generate random points
 *
 ****************************/


inline int getRandom(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

vector <PointC> generateRandom(vector<PointC> &points, type minX, type maxX, int num, bool centroids, type minY, type maxY) {
    PointC point = PointC();
    if (centroids)
        point = PointC(-1);
    while (num) {
        point.x = getRandom(minX, maxX);// (rand()%(maxX-minX+1))+minX;
        point.y = getRandom(minY, maxY);//(rand()%(maxY-minY+1))+minY;

        points.push_back(point);
        num--;
    }

    return points;
}

vector <PointC> generateRandom(vector<PointC> &points, type minX, type maxX, int num, bool centroids) {
    return generateRandom(points, minX, maxX, num, centroids, minX, maxX);
}


vector<PointC> initialCenters(int k) {
    generateRandom(centroids, 1, 100, k, true);
    vector <PointC> p = centroids;
    // make sure that initial centroids are unique
    auto fun = [&](int index) {
        int ii = centroids.size();
        for (int i = index; i < (centroids.size() - 1); i++) {
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

    return centroids;
}

// true if centroids changes
inline bool uppdateCentroids() {
    vector<PointC> oldCentroids = centroids;


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

    for (int i = 0; i < centroids.size(); i++) {
        if (centroids[i] != oldCentroids[i])
            return true;
    }

    return false;


}

inline void updateLabels(MEASURE m) {
    double current;
    for (auto &p: points) {
        double min = DBL_MAX;
        int i = 0;
        for (auto c: centroids) {
            if (m == Euclidean)
                 current = sqrt(pow(p.x - c.x, 2) + pow(p.y - c.y, 2));
            else//Manhatten
                 current = abs(p.x - c.x) + abs(p.y - c.y);
            if (current < min) {
                min = current;
                p.cluster = i;//initially all clusters are zero
            }
            i++;
        }
    }


}

void intracluster(MEASURE m) {
    vector<type> distances;


    for (int i = 0; i< centroids.size(); i++) {
        int count = 0, distance = 0;

        for (auto p: points) {
            if (p.cluster == i) {
                count++;
                if (m == Euclidean)
                    distance += sqrt(pow(p.x - centroids[i].x, 2) + pow(p.y - centroids[i].y, 2));
                else//Manhatten
                    distance += abs(p.x - centroids[i].x) + abs(p.y - centroids[i].y);
            }
        }

        if (count==0){ //empty cluster, regenerate it
            distances.push_back( -1);//
        } else{
            distances.push_back( distance/count);
        }

    }

    string measure = m? "Manhattan":"Euclidean";
    cout <<endl<<"Intra-distance ("<<measure<<"):"<<endl;
    type sum = 0;
    for(auto d: distances){
        sum +=d;
        cout <<BGREEN<<d<<RESET<<"\t";
    }
    sum = sum/k;
    cout <<"\t, and sum of intra-distance is: "<<BRED<<sum<<RESET<<endl;

}

void plot(){
    image = Mat::zeros(420, 420, CV_8UC3);
    plot(points, image);
    plot(centroids, image);
}


void cluster(int k, MEASURE measure) {

    //centroids.clear();
    for(auto &p: points){
        p.cluster = 0;
    }

    int i = 0;
    plot();
    show(image, measure, i);
    do {

        updateLabels(measure);
        i++;
    } while (uppdateCentroids());
    plot();
    show(image, measure, i);

    intracluster(measure);
}

