/*
    ======================================
    Make Loop Up Table For Marching Square
    ======================================

    p0          a           p1
        ○ ㅡㅡㅡㅡㅡㅡㅡㅡㅡ ○
        |                |
        |                |
    d   |                |   b
        |                |
        |                |
        |                |
        ○ ㅡㅡㅡㅡㅡㅡㅡㅡㅡ ○
    p3          c           p2

    Vertex : p0, p1, p2, p3
    Edge : a, b, c, d
    
    Edge Case : {a, b, c, d} = 2^4 = 16 
    
    Example Edge Bit Rules : 
    
    p0          a           p1
        ○ ㅡㅡㅡㅡㅡㅡㅡㅡㅡ ○
        |                |
        |                |
    d   |                ●   b
        |               /|
        |              / |
        |             /  |
        ○ ㅡㅡㅡㅡㅡㅡ ● ㅡ ●
    p3          c           p2

    {p0, p1, p2, p3} = {0, 0, 1, 0} <-----> {a, b, c, d} = {0, 1, 1, 0}

    // Not Make Any Line
    {0, 0, 0, 0} <-----> {0, 0, 0, 0} = 0
    {1, 1, 1, 1} <-----> {0, 0, 0, 0} = 0

    // Make Horizontal & Vertical Line
    {1, 1, 0, 0} <-----> {0, 1, 0, 1} = 5
    {0, 0, 1, 1} <-----> {0, 1, 0, 1} = 5
    {0, 1, 1, 0} <-----> {1, 0, 1, 0} = 10
    {1, 0, 0, 1} <-----> {1, 0, 1, 0} = 10

    // Make One Diagonal Line 
    {1, 0, 0, 0} <-----> {1, 0, 0, 1} = 9
    {0, 1, 1, 1} <-----> {1, 0, 0, 1} = 9
    {0, 1, 0, 0} <-----> {1, 1, 0, 0} = 12
    {1, 0, 1, 1} <-----> {1, 1, 0, 0} = 12
    {0, 0, 1, 0} <-----> {0, 1, 1, 0} = 6
    {1, 1, 0, 1} <-----> {0, 1, 1, 0} = 6
    {0, 0, 0, 1} <-----> {0, 0, 1, 1} = 3
    {1, 1, 1, 0} <-----> {0, 0, 1, 1} = 3

    // Make Two Diagonal Line
    p0          a           p1              p0          a           p1
        ● ㅡ ● ㅡㅡㅡㅡㅡㅡ ○                      ○ ㅡㅡㅡㅡㅡ ● ㅡㅡㅡ ●
        |  /             |                      |           \    |
        | /              |                      ●            \   |
    d   |/               ●   b              d   | \           \  |   b
        ●               /|                      |  \           \ |
        |              / |                      |   \            ●
        |             /  |                      |    \           |
        ○ ㅡㅡㅡㅡㅡㅡ ● ㅡ ●                      ● ㅡㅡ ● ㅡㅡㅡㅡㅡㅡ ○
    p3          c           p2              p3          c           p2

    {0, 1, 0, 1} <-----> {1, 1, 1, 1} = 15
    {1, 0, 1, 0} <-----> {1, 1, 1, 1} = 15 
    
*/

#ifndef MARCHING_SQUARE
#define MARCHING_SQUARE

#include <iostream>
#include <array>
#include <map>

#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/xfeatures2d/nonfree.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"

#include "utility.h"

std::array<int, 4> get_vertice_density(cv::Mat img, cv::Point2f p0, cv::Point2f p1, cv::Point2f p2, cv::Point2f p3)
{

    bool p0_is_white = false;
    bool p1_is_white = false;
    bool p2_is_white = false;
    bool p3_is_white = false;

    cv::Vec3b white(255, 255, 255);
    if(img.at<cv::Vec3b>(p0.y, p0.x) == white)
        p0_is_white = true;
    if(img.at<cv::Vec3b>(p1.y, p1.x) == white)
        p1_is_white = true;
    if(img.at<cv::Vec3b>(p2.y, p2.x) == white)
        p2_is_white = true;
    if(img.at<cv::Vec3b>(p3.y, p3.x) == white)
        p3_is_white = true;

    std::array<int, 4> vertices_density;
    if(p0_is_white == false && p1_is_white == false &&
       p2_is_white == false && p3_is_white == false)
       vertices_density = std::array<int, 4>{0, 0, 0, 0};
    else if(p0_is_white == false && p1_is_white == false &&
            p2_is_white == false && p3_is_white == true)
            vertices_density = std::array<int, 4>{0, 0, 0, 1};
    else if(p0_is_white == false && p1_is_white == false &&
            p2_is_white == true && p3_is_white == false)
            vertices_density = std::array<int, 4>{0, 0, 1, 0};
    else if(p0_is_white == false && p1_is_white == false &&
            p2_is_white == true && p3_is_white == true)
            vertices_density = std::array<int, 4>{0, 0, 1, 1};
    else if(p0_is_white == false && p1_is_white == true &&
            p2_is_white == false && p3_is_white == false)
            vertices_density = std::array<int, 4>{0, 1, 0, 0};
    else if(p0_is_white == false && p1_is_white == true &&
            p2_is_white == false && p3_is_white == true)
            vertices_density = std::array<int, 4>{0, 1, 0, 1};
    else if(p0_is_white == false && p1_is_white == true &&
            p2_is_white == true && p3_is_white == false)
            vertices_density = std::array<int, 4>{0, 1, 1, 0};
    else if(p0_is_white == false && p1_is_white == true &&
            p2_is_white == true && p3_is_white == true)
            vertices_density = std::array<int, 4>{0, 1, 1, 1};
    else if(p0_is_white == true && p1_is_white == false &&
            p2_is_white == false && p3_is_white == false)
            vertices_density = std::array<int, 4>{1, 0, 0, 0};
    else if(p0_is_white == true && p1_is_white == false &&
            p2_is_white == false && p3_is_white == true)
            vertices_density = std::array<int, 4>{1, 0, 0, 1};
    else if(p0_is_white == true && p1_is_white == false &&
            p2_is_white == true && p3_is_white == false)
            vertices_density = std::array<int, 4>{1, 0, 1, 0};
    else if(p0_is_white == true && p1_is_white == false &&
            p2_is_white == true && p3_is_white == true)
            vertices_density = std::array<int, 4>{1, 0, 1, 1};
    else if(p0_is_white == true && p1_is_white == true &&
            p2_is_white == false && p3_is_white == false)
            vertices_density = std::array<int, 4>{1, 1, 0, 0};
    else if(p0_is_white == true && p1_is_white == true &&
            p2_is_white == false && p3_is_white == true)
            vertices_density = std::array<int, 4>{1, 1, 0, 1};
    else if(p0_is_white == true && p1_is_white == true &&
            p2_is_white == true && p3_is_white == false)
            vertices_density = std::array<int, 4>{1, 1, 1, 0};
    else if(p0_is_white == true && p1_is_white == true &&
            p2_is_white == true && p3_is_white == true)
            vertices_density = std::array<int, 4>{1, 1, 1, 1};

    return vertices_density;
}

void vertices_to_edge(std::array<int, 4> vertices, int &edgeRule)
{
    if(vertices == std::array<int, 4>{0, 0, 0, 0})
        edgeRule = 0;
    else if(vertices == std::array<int, 4>{0, 0, 0, 1})
        edgeRule = 3;
    else if(vertices == std::array<int, 4>{0, 0, 1, 0})
        edgeRule = 6;
    else if(vertices == std::array<int, 4>{0, 0, 1, 1})
        edgeRule = 5;
    else if(vertices == std::array<int, 4>{0, 1, 0, 0})
        edgeRule = 12;
    else if(vertices == std::array<int, 4>{0, 1, 0, 1}) 
        edgeRule = 15; // Same Bit Num but diff Diagnoal Line 
    else if(vertices == std::array<int, 4>{0, 1, 1, 0})
        edgeRule = 10;
    else if(vertices == std::array<int, 4>{0, 1, 1, 1})
        edgeRule = 9;
    else if(vertices == std::array<int, 4>{1, 0, 0, 0})
        edgeRule = 9;
    else if(vertices == std::array<int, 4>{1, 0, 0, 1})
        edgeRule = 10;
    else if(vertices == std::array<int, 4>{1, 0, 1, 0}) 
        edgeRule = 16; // Same Bit Num but diff Diagnoal Line
    else if(vertices == std::array<int, 4>{1, 0, 1, 1})
        edgeRule = 12;
    else if(vertices == std::array<int, 4>{1, 1, 0, 0})
        edgeRule = 5;
    else if(vertices == std::array<int, 4>{1, 1, 0, 1})
        edgeRule = 6;
    else if(vertices == std::array<int, 4>{1, 1, 1, 0})
        edgeRule = 3;
    else if(vertices == std::array<int, 4>{1, 1, 1, 1})
        edgeRule = 0;
}

void drawLine(int edgeRule, cv::Mat &img, cv::Point2f p0, cv::Point2f p1, cv::Point2f p2, cv::Point2f p3)
{
    // Get interpolation point
    cv::Point2f p01 = interpolation(p0, p1, 0.5);
    cv::Point2f p12 = interpolation(p1, p2, 0.5);
    cv::Point2f p23 = interpolation(p2, p3, 0.5);
    cv::Point2f p03 = interpolation(p0, p3, 0.5);

    switch(edgeRule)
    {
        case 0:
            break;
        case 3:
            cv::line(img, p03, p23, cv::Scalar(0, 255, 0), 1);
            break;
        case 5:
            cv::line(img, p03, p12, cv::Scalar(0, 255, 0), 1);
            break;
        case 6:
            cv::line(img, p12, p23, cv::Scalar(0, 255, 0), 1);
            break;
        case 9:
            cv::line(img, p01, p03, cv::Scalar(0, 255, 0), 1);
            break;
        case 10:
            cv::line(img, p01, p23, cv::Scalar(0, 255, 0), 1);
            break;
        case 12:
            cv::line(img, p01, p12, cv::Scalar(0, 255, 0), 1);
            break;
        case 15:
            cv::line(img, p01, p03, cv::Scalar(0, 255, 0), 1);
            cv::line(img, p12, p23, cv::Scalar(0, 255, 0), 1);
            break;
        case 16:
            cv::line(img, p01, p12, cv::Scalar(0, 255, 0), 1);
            cv::line(img, p23, p03, cv::Scalar(0, 255, 0), 1);
            break;
        default:
            break;
    }
}

#endif