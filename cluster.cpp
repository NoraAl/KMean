#include "cluster.hpp"

static Points points;
static Points centroids;

/****************************
 *
 *
 * main
 *
 *
 ****************************/

int main() {
    try {

        srand(time(nullptr));
        //auto ps = generateRandom(points, minP, maxP, 1000);
        auto ps = readPoints("../points.txt", false);
        
        vector<int> ks={2};
        //cluster for each k
        for (auto k: ks){
            cout <<"\n-------------------------------------------\nk = "<<k<<endl;
            Points currentCentroids = c;//initialCenters(k);
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
 *
 * generate random points
 *
 *
 ****************************/
inline int getRandom(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

Points generateRandom(Points &points, double minX, double maxX, int num, bool centroids, double minY, double maxY) {
    P point = P();
    if (centroids)
        point = P(-1);
    while (num) {
        point.x = getRandom(minX, maxX);
        point.y = getRandom(minY, maxY);

        points.push_back(point);
        num--;
    }

    return points;
}

Points generateRandom(Points &points, double minX, double maxX, int num, bool centroids) {
    return generateRandom(points, minX, maxX, num, centroids, minX, maxX);
}

/****************************
 *
 *
 * initiate centroids
 *
 *
 ****************************/

Points initialCenters(int k) {
    generateRandom(centroids, minP, maxP, k, true);
    Points p = centroids;
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
 *
 * reset each centroid to the
 * average of its cluster
 *
 *
 ****************************/
inline bool updateCentroids() {
    Points oldCentroids = centroids;


    for (int i = 0; i< centroids.size(); i++) {
        int count = 0;
        P currentCentroid = P(-1);

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
 *
 * reassign each point to the
 * nearest cluster
 *
 *
 ****************************/
inline void updateLabels(MEASURE m) {
    double current;
    for (auto &p: points) {
        double min = DBL_MAX;
        int i = 0;
        for (auto c: centroids) {
            current = getDistance(p,c, m);
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
 *
 * k-mean algorithm
 *
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
    minMax(points,centroids,measure);
}


/****************************
 *
 *
 * intra-cluster distance and the
 * average of all intra-cluster distances.
 *
 *
 ****************************/

void intracluster(MEASURE m, int k) {
    vector<double> distances;

    for (int i = 0; i< centroids.size(); i++) {
        int count = 0; double distance = 0;// reset them each iteration

        for (auto p: points) {
            if (p.cluster == i) {
                count++;
                distance += getDistance(p,centroids[i], m);
            }
        }

        if (count==0){ //empty cluster, regenerate it
            distances.push_back(-1);//
        } else{
            distances.push_back( distance/count);
        }

    }


    cout <<endl<<"Intra-distances ("<<(m? "Manhattan":"Euclidean")<<"):"<<endl;
    double sum = 0;
    for(auto d: distances){
        if (d<0)
            continue;
        sum +=d;
        cout <<BGREEN<<d<<RESET<<"\t";
    }
    sum = sum/k;
    cout <<"\nAverage of intra-distances is: "<<BRED<<sum<<RESET<<endl;

}

