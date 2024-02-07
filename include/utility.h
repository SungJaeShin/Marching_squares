#ifndef UTILITY
#define UTILITY

#include <iostream>
#include <cstdlib> 
#include <ctime>

#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/xfeatures2d/nonfree.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"

#include "parameters.h"

cv::Mat gen_grid_img(int width, int height)
{
    // Init Grid Img
    cv::Mat grid_img(height, width, CV_8UC3, cv::Scalar(0, 0, 0));

    // Make Random Grid Img
    std::srand(time(0)); 
    for(int y = 1; y < height - 1; y++)
    {
        for(int x = 1; x < width - 1; x++)
        {
            if(x % DIFF_X == 0 && y % DIFF_Y == 0)
            {
                double value = (std::rand() % 2) * 255;
                double r, g, b;
                r = g = b = value;

                cv::Vec3b rgb(r, g, b);
                grid_img.at<cv::Vec3b>(y, x) = rgb;
            }
        }
    }
        
    return grid_img;
}

cv::Point2f interpolation(cv::Point2f pt1, cv::Point2f pt2, double isovalue)
{
    double inter_x = (double)pt1.x + isovalue * ((int)pt2.x - (int)pt1.x);
    double inter_y = (double)pt1.y + isovalue * ((int)pt2.y - (int)pt1.y);

    return cv::Point2f(inter_x, inter_y);
}

#endif