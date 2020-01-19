//
// Created by pratheek on 2020-01-19.
//

#include <shapeDetection.h>

using namespace cv;
using namespace std;
using namespace rapidjson;

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

    vector<int> x1, x2;

    // Compute Enclosing box around the circle
    x1.push_back(circles[0][1] - circles[0][2]);
    x1.push_back(circles[0][0] - circles[0][2]);
    x2.push_back(circles[0][1] + circles[0][2]);
    x2.push_back(circles[0][0] + circles[0][2]);

    registerJsonShape("Circle", x1, x2);

    imshow("output", image);
    waitKey(0);
}

void shapeDetection::cannyHough() {
    Mat image = imread("../../oriental_picture.png");

    if (!image.data) {
        printf("No image data \n");
        exit(-1);
    }

    Mat gray, bilat, edges;
    cvtColor(image, gray, COLOR_BGR2GRAY);
    bilateralFilter(gray, bilat, 5, 100, 50);
    Canny(bilat, edges, 50, 120);

    vector<Vec4i> lines;
    HoughLinesP(edges, lines, 1, CV_PI / 180, 20, 50, 10);

    for (size_t i = 0; i < 2; i++) {
        Vec4i l = lines[i];
        line(image, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255, 0, 0), 2, LINE_AA);
        cout << "Check" << Point(l[0], l[1]) << " " << Point(l[2], l[3]) << endl;
    }
    // Show result image
    imshow("Result Image", image);
    waitKey(0);
    destroyAllWindows();
}

void shapeDetection::redDetection() {
    Mat image = imread("../../oriental_picture.png");

    if (!image.data) {
        printf("No image data \n");
        exit(-1);
    }
    Mat mask, redOutput, gray, blurred, thresh;
    inRange(image, Scalar(0, 0, 0), Scalar(0, 0, 255), mask);

    bitwise_and(image, image, redOutput, mask);

    cvtColor(redOutput, gray, COLOR_BGR2GRAY);
    GaussianBlur(gray, blurred, Size(5, 5), 0);
    threshold(blurred, thresh, 60, 255, THRESH_BINARY);

    vector<vector<Point>> contours;
    vector<Point> approx;
    string shape;

    cv::findContours(thresh.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    for (int i = 0; i < contours.size(); i++) {
        cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true) * 0.04, true);

        if (approx.size() == 3) {
            shape = "Triangle";
        } else if (approx.size() == 4) {
            auto rect = boundingRect(approx);
            cout << rect.x << " " << rect.y << " " << rect.width << " " << rect.height << endl;

            if ((rect.width / rect.height) == 1) {
                shape = "Square";
            } else {
                shape = "Rectangle";
            }
        } else {
            shape = "Circle";
        }
    }

    imshow("output", thresh);
    waitKey(0);
    destroyAllWindows();
}

void shapeDetection::registerJsonShape(const char *shape, vector<int> x1, vector<int> x2) {
    cout << shape << " " << x1[0] << " " << x2[0] << endl;

    StringBuffer s;
    PrettyWriter<StringBuffer> writer(s);

    writer.StartObject();
    writer.Key("boundingPoly");
    writer.StartObject();
    writer.String("vertices");
    writer.StartArray();
    writer.StartObject();
    writer.Key("x");
    writer.Uint(x1[1]);
    writer.Key("y");
    writer.Uint(x1[0]);
    writer.EndObject();
    writer.StartObject();
    writer.Key("x");
    writer.Uint(x2[1]);
    writer.Key("y");
    writer.Uint(x2[0]);
    writer.EndObject();
    writer.EndArray();
    writer.EndObject();
    writer.Key("description");
    writer.String(shape);
    writer.EndObject();


    cout << "CHECK\n" << s.GetString() << endl;

}

int main(int argc, char **argv) {

    auto sD = new shapeDetection;
    sD->redDetection();
//    sD->cannyHough();
//    sD->circleDetection();
    return 0;
}