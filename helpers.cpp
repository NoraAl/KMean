#include <fstream>
#include "cluster.hpp"

// overload operators
bool operator==(const P &p1, const P &p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

bool operator!=(const P &p1, const P &p2) {
    return !(p1 == p2);
}

double getDistance(P p1, P p2, MEASURE m) {
    if (m == Euclidean)
        return getEuclidean(p1, p2);
    return getManhattan(p1, p2);

}

inline double getEuclidean(P p1, P p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));

}

inline double getManhattan(P p1, P p2) {
    return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

inline void getMinMaxPerCluster(Points p, int i, int j){

}

void minMax(Points &points, Points &centroids, MEASURE m){
    int k = centroids.size();
    double minMatrix[k][k];
    double maxMatrix[k][k];
    //     0 1 ...... k-1
    //   0| | | .....|   |
    //   1| | | .....|   |
    //   .| | | .....|   |
    //   .| | | .....|   |
    // k-2| | | .....|   |
    // k-1| | | .....|   |
    for (int i = 0; i <k; i++)
        for (int j = 0; j<k; j++){
            minMatrix[i][j] = DBL_MAX;
            maxMatrix[i][j] = DBL_MIN;
        }

    for (int i = 0; i <k; i++)// current cluster
        for (int j = 0; j<k; j++){//every other cluster

            if (i>=j)
                continue;

            else {
                for (auto p: points){
                    if (p.cluster == i){
                        for (auto otherP: points){
                            if (otherP.cluster == j){
                                double current = getDistance(p,otherP,m);
                                if ( current< minMatrix[i][j])
                                    minMatrix[i][j] = current;
                                if (current > maxMatrix[i][j])
                                    maxMatrix[i][j] = current;
                            }
                        }
                    }
                }
            }

        }

    cout <<BGREEN<< "Minimum distances: "<< endl;
    for (int i = 0; i <k; i++) {
        for (int j = 0; j < k; j++) {
            cout << fnum(minMatrix[i][j]);

        }
        cout <<endl;
    }
    cout <<RESET<<endl;

    cout <<BRED<< "Max distances: "<< endl;
    for (int i = 0; i <k; i++) {
        for (int j = 0; j < k; j++) {
            cout << fnum(maxMatrix[i][j]);

        }
        cout <<endl;
    }
    cout <<RESET<<endl;

}


string fnum(double num){
    stringstream stream;
    if (num == DBL_MIN || num == DBL_MAX){
        stream << setfill(' ') << setw(10) << "*";
    } else {
        stream << setfill(' ') << setw(10) << num;
    }
    return stream.str();
}


void plot(Points points, Mat image) {
    int padding = 20, size = 400, windowSize = size + padding;
    int scale = 4;
    int p2 = padding / 2;

    vector<Scalar> colors;
    colors.push_back(Scalar(255, 255, 255));//1
    colors.push_back(Scalar(255, 0, 255));//2
    colors.push_back(Scalar(0, 255, 0));//3
    colors.push_back(Scalar(255, 0, 0));//4
    colors.push_back(Scalar(127, 127, 127));//5
    colors.push_back(Scalar(0, 127, 255));//6
    colors.push_back(Scalar(127, 50, 40));//7
    colors.push_back(Scalar(40, 0, 127));//8
    colors.push_back(Scalar(255, 30, 127));//9
    colors.push_back(Scalar(0, 0, 255));//red for centroids

    //opencv coordinates are not mathematical coordinates, we need to flip the image
    for (auto p:points) {
        if (p.cluster < 0)//centroids
            circle(image, Point((p.x * 40) + p2, (p.y * 40) + p2), 3, colors[colors.size() - 1], -1);
        else
            circle(image, Point((p.x * 40) + p2, (p.y * 40) + p2), 2, colors[p.cluster], -1);
    }
}

void show(Mat image, MEASURE m, int i) {
    Mat flipped;
    flip(image, flipped, 0);
    string filename = m ? "../results/Manhattan_" : "../results/Euclidean_";
    filename += to_string(i) + ".png";
    imwrite(filename, flipped);
}

void printPoints(Points points) {
    int i = 0;
    for (auto p: points) {
        i++;
        cout  << "" << p.x << "," << p.y << "\n" ;
    }
    cout << endl;
}

void plot(Points &points, Points &centroids, MEASURE m, int i) {
    Mat image = Mat::zeros(420, 420, CV_8UC3);
    plot(points, image);
    plot(centroids, image);
    show(image, m, i);
}

Points readPoints(string filename, bool centroids, char separator) {
    Points points;
    P point;
    point.cluster =(centroids? -1:0);;
    ifstream file(filename, ifstream::in);
    if (!file) {
        throw "Not a valid file name.";
    }

    if (file.is_open())
    {
        string line, x, y ;
        while (getline(file, line)) {
            stringstream lineStream(line);
            getline(lineStream, x, separator);
            getline(lineStream, y);

            if(!x.empty() && !y.empty()) {
                point.x = atof(x.c_str());
                point.y = atof(y.c_str());

                points.push_back(point);
            }
        }
        file.close();
    } else {
        throw "couldn't open " + filename + "\n";
    }

    return points;

}