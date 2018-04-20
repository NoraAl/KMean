#include "cluster.hpp"

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
    colors.push_back(Scalar(127, 50, 40));
    colors.push_back(Scalar(40, 0, 127));
    colors.push_back(Scalar(255, 30, 127));
    colors.push_back(Scalar(0, 0, 255));//red for centroids

    //opencv coordinates are not mathematical coordinates, we need to flip the image
    for (auto p:points) {
        if (p.cluster < 0)//centroids
            circle(image, Point((p.x * 4) + p2, (p.y * 4) + p2), 3, colors[colors.size()-1], -1);
        else
            circle(image, Point((p.x * 4) + p2, (p.y * 4) + p2), 2, colors[p.cluster], -1);
    }

}

void show(Mat image, MEASURE m, int i) {
    Mat flipped;
    flip(image, flipped, 0);
    string filename = m? "../results/Manhattan_":"../results/Euclidean_";
    filename += to_string(i)+".png";
    //imshow("flipped 0", flipped);
    //waitKey(1000);
    imwrite(filename,image);
}

void printPoint(vector<PointC> points) {
    for (auto p: points) {
        cout << BCYAN<<"(" << p.x << "," << p.y << ")\t"<<RESET;
    }

    cout << endl;
}



//type getMaxX() {
//    type max = points.at(0).x;
//    for (auto point: points) {
//        if (point.x > max)
//            max = point.x;
//    }
//    return max;
//}
//
//type getMaxY() {
//    type max = points.at(0).y;
//    for (auto point: points) {
//        if (point.y > max)
//            max = point.y;
//    }
//    return max;
//}
//
//type getMinX() {
//    type min = points.at(0).x;
//    for (auto point: points) {
//        if (point.x < min)
//            min = point.x;
//    }
//    return min;
//}
//
//type getMinY() {
//    type min = points.at(0).y;
//    for (auto point: points) {
//        if (point.y < min)
//            min = point.y;
//    }
//    return min;
//}
