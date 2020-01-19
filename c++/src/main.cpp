//
// Created by pratheek on 2020-01-19.
//

#include <shapeDetection.h>

using namespace cv;
using namespace std;


void shapeDetection::circleDetection() {
    Mat image = imread("../../oriental_picture.png");

    if (!image.data) {
        printf("No image data \n");
        exit(-1);
    }
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);

    Mat img_blur;
    medianBlur(gray, img_blur, 5);

    vector<Vec3f> circles;
    HoughCircles(img_blur, circles, HOUGH_GRADIENT, 1, 160, 20, 50, 40, 90);

    for (size_t i = 0; i < circles.size(); i++) {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        circle(image, center, radius, Scalar(0, 255, 255), 2, 1, 0);
    }

    // Compute Enclosing box around the circle
    vector<int> x1, x2;

    x1.push_back(circles[0][1] - circles[0][2]);
    x1.push_back(circles[0][0] - circles[0][2]);
    x2.push_back(circles[0][1] + circles[0][2]);
    x2.push_back(circles[0][0] + circles[0][2]);

    cout << x1[0] << " " << x1[1] << endl;
    cout << x2[0] << " " << x2[1] << endl;

    namedWindow("output");
    imshow("output", image);
    waitKey(0);
}


int main(int argc, char **argv) {

    auto sD = new shapeDetection;
    sD->circleDetection();

    return 0;
}