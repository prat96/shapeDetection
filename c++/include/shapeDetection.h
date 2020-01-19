//
// Created by pratheek on 2020-01-19.
//

#ifndef MYCUJOO_SHAPEDETECTION_H
#define MYCUJOO_SHAPEDETECTION_H

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <sys/stat.h>
#include <fstream>

class shapeDetection {
public:
    static void circleDetection();

    static void cannyHough();

    static void redDetection();

private:

    static void registerJsonShape(const char *shape, std::vector<int> x1, std::vector<int> x2);

};


#endif //MYCUJOO_SHAPEDETECTION_H
