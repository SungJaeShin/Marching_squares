#include <iostream>
#include <random>
#include <cstdlib> 
#include <ctime>
#include <string>
#include <cmath>

#include "../include/utility.h"
#include "../include/marching_square.h"

int main()
{
    // Initialize 640 x 480 Image
    int width = 640;
    int height = 480;
    cv::Mat grid_img = gen_grid_img(width, height);

    // Getting Verices Intensity
    cv::Mat marching_grid_img = grid_img.clone();
    cv::Mat marching_line_img(height, width, CV_8UC3, cv::Scalar(0, 0, 0));
    for(int y = 1; y < height - 1; y++)
    {
        for(int x = 1; x < width - 1; x++)
        {
            if(x % DIFF_X == 0 && y % DIFF_Y == 0)
            {   
                // Get Vertices
                cv::Point2f p0(x - DIFF_X, y - DIFF_Y);
                cv::Point2f p1(x, y - DIFF_Y);
                cv::Point2f p2(x, y);
                cv::Point2f p3(x - DIFF_X, y);
            
                // Get Vertices Density
                std::array<int, 4> vertices_density = get_vertice_density(grid_img, p0, p1, p2, p3);
            
                // Get Edge Rule using Predefined table
                int edgeRule;
                vertices_to_edge(vertices_density, edgeRule);

                // Draw Line
                drawLine(edgeRule, marching_grid_img, p0, p1, p2, p3);
                drawLine(edgeRule, marching_line_img, p0, p1, p2, p3);
            }
        }
    }
    
    return 0;
}