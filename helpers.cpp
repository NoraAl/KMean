#include "cluster.hpp"

// overload operators
bool operator==(const PointC &p1, const PointC &p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

bool operator!=(const PointC &p1, const PointC &p2) {
    return !(p1 == p2);
}

void plot(vector<PointC> points, Mat image) {
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
            circle(image, Point((p.x * 40) + p2, (p.y * 40) + p2), 3, colors[colors.size()-1], -1);
        else
            circle(image, Point((p.x * 40) + p2, (p.y * 40) + p2), 2, colors[p.cluster], -1);
    }
}

void show(Mat image, MEASURE m, int i) {
    Mat flipped;
    flip(image, flipped, 0);
    string filename = m? "../results/Manhattan_":"../results/Euclidean_";
    filename += to_string(i)+".png";
    imwrite(filename,flipped);
}

void printPoints(vector<PointC> points) {
    int i = 0;
    for (auto p: points) {
        i++;
        if (i%10 ==0)
            cout <<endl;
        cout << BCYAN<<"(" << p.x << "," << p.y << ")\t"<<RESET;
    }
    cout << endl;
}

void plot(vector<PointC>& points, vector<PointC>& centroids, MEASURE m, int i){
    Mat image = Mat::zeros(420, 420, CV_8UC3);
    plot(points, image);
    plot(centroids, image);
    show (image, m,i);
}