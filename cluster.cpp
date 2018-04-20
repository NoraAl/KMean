#include "cluster.hpp"

static vector<PointC> points;
static vector<PointC> centroids;
static int minP, maxP;



/****************************
 *
 *
 * main
 *
 *
 ****************************/

int main() {
    try {
        minP = 1;
        maxP = 100;
        int count = 100;

        srand(time(nullptr));

        //auto ps = generateRandom(points, minP, maxP, count);
        vector<PointC> ps = {{1,1,0},{1,2,0},{1,3,0},{1,4,0},
                             {2,1,0},{2,2,0},{2,3,0},{2,4,0}};


        vector<PointC> c = {{1,1,-1},{1,2,-1}};

        cout <<"Points:"<<endl;
        printPoints(ps);

        vector<int> ks={2};
        //cluster for each k
        for (auto k: ks){
            cout <<"\n-------------------------------------------\nk = "<<k<<endl;
            vector <PointC> currentCentroids = c;//initialCenters(k);
            printPoints(currentCentroids);
            centroids = currentCentroids;
            points = ps;
            cluster(k, Euclidean);
            centroids = currentCentroids;
            points = ps;
            cluster(k,Manhattan);
        }
        return 0;
    } catch (string error) {
        cout << error<< endl;
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
        point.x = getRandom(minX, maxX);
        point.y = getRandom(minY, maxY);

        points.push_back(point);
        num--;
    }

    return points;
}

vector <PointC> generateRandom(vector<PointC> &points, type minX, type maxX, int num, bool centroids) {
    return generateRandom(points, minX, maxX, num, centroids, minX, maxX);
}

/****************************
 *
 * initiate centroids
 *
 ****************************/

vector<PointC> initialCenters(int k) {
    generateRandom(centroids, minP, maxP, k, true);
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

/****************************
 *
 * reset each centroid to the average of its cluster
 *
 ****************************/
inline bool updateCentroids() {
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

        if (count){
            //average
            currentCentroid.x = currentCentroid.x / count;
            currentCentroid.y = currentCentroid.y / count;
            //set current centroid
            centroids[i] = currentCentroid;
        }
        //else{ //if empty cluster, regenerate that centroid, or leave it as is
        //    centroids[i].x = getRandom(minP,maxP);
        //     centroids[i].y = getRandom(minP,maxP);
        //}
    }

    // return true if centroids change; i.e. return false if algorithm converge
    for (int i = 0; i < centroids.size(); i++) {
        if (centroids[i] != oldCentroids[i])
            return true;
    }

    return false;
}

/****************************
 *
 * reassign each point to the nearest cluster
 *
 ****************************/
inline void updateLabels(MEASURE m) {
    double current;
    for (auto &p: points) {
        double min = DBL_MAX;
        int i = 0;
        for (auto c: centroids) {
            if (m == Euclidean)
                 current = sqrt(pow(p.x - c.x, 2) + pow(p.y - c.y, 2));
            else//Manhattan
                 current = abs(p.x - c.x) + abs(p.y - c.y);
            if (current < min) {
                min = current;
                p.cluster = i;//initially all clusters are zero
            }
            i++;
        }
    }
}

/****************************
 *
 * intra-cluster distance and the average
 * of all intra-cluster distances.
 *
 ****************************/

void intracluster(MEASURE m, int k) {
    vector<type> distances;

    for (int i = 0; i< centroids.size(); i++) {
        int count = 0;
        double distance = 0;

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

/****************************
 *
 * k-mean algorithm
 *
 ****************************/
void cluster(int k, MEASURE measure) {

    //centroids.clear();
    for(auto &p: points){
        p.cluster = 0;
    }
    int i = 0;
    //plot points before clustering
    plot(points, centroids, measure, i );

    do {
        updateLabels(measure);
        i++;
    } while (updateCentroids());

    //plot the final clustering
    plot(points, centroids, measure, i );

    intracluster(measure,k);
}

