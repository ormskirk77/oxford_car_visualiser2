//
// Created by Tim Fernandez-Hart on 01/05/2020.
//
#include "point_cloud_builder.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

/*
 *  Takes a string reference to the filename of the binary file to be turned into a point cloud.
 *  Returns a pointer to an array containing the cartesian coordinates of all the LiDAR points in a
 *  particular .bin file.
 *  The .bin file format appears to be in {x * N}, {y * N}, {z * N}, {I * N}. This is in contrast to
 *  the paper which states the data format is in {x, y, z, I} * N.
 *
 */
float* PointCloudBuilder::getPointCLoud(const std::string& filename) {
    if (filename == "") {std::cerr << "File name not provided.";}

    std::ifstream file;
    file.open(filename, std::ios::binary | std::ios::in);

    if(!file){
        std::cout << "Problem opening file in getPointCloud." << std::endl;
    } else {
        std::streampos start = file.tellg();
        file.seekg(0, std::ios::end);
        const size_t num_coords = file.tellg()/sizeof(float);
        std::streampos end = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<float> lidar(num_coords);

        file.read(reinterpret_cast<char*>(&lidar[0]), num_coords*sizeof(float));

        int stride = (lidar.size()/4);

        float points [num_coords];

        int i=0;
        for(int vertex=0 ; vertex<stride ; vertex++){
            for(int coord=0 ; coord<4 ; coord++){
                int index = vertex+(coord*stride);
                points[i] = lidar.at(index);
                i++;
            }
        }
        file.close();
        return points;
    }
}

// Convert LiDAR filename to the time when it was taken.
std::string PointCloudBuilder::convertTime(const std::string &timeForConversion) {

    std::string stringUnixTime = timeForConversion.substr(0, 15);
    time_t rawtime;
    rawtime = posix2time(std::stoi(stringUnixTime));

    return ctime(&rawtime);

}

// Takes a string reference to the file.
// Returns number of coordinates in a binary file.
int PointCloudBuilder::vertexCountOfCloud(const std::string &filename) {
    if (filename == "") {std::cerr << "File name not provided.";}

    std::ifstream file;
    file.open(filename, std::ios::binary | std::ios::in);

    if(!file){
        std::cout << "Problem opening file in vertexCountOfPointCloud." << std::endl;
    } else {
        std::streampos start = file.tellg();
        file.seekg(0, std::ios::end);
        const size_t numPoints = file.tellg() / sizeof(float);
        return numPoints;
    }

}

// Takes a string reference to the timestamps file provided in the dataset.
// Returns a set of strings of the relative file paths to the LiDAR binary
// files in a vector.
std::vector<std::string> PointCloudBuilder::loadTimesStamps(const std::string &filename) {
    if(filename == ""){std::cout << "No filename for timestamps" << std::endl;}
    std::ifstream file;
    std::vector<std::string> timeStampFileNames;
    file.open(filename, std::ios::binary | std::ios::in);
    if(!file) {
        std::cout << "Problem opening timestamps file." << std::endl;
    } else {
        std::string timestamp;
        std::string temp;
        while(getline(file, timestamp)){
            size_t spacePos = timestamp.find(" ");
            temp = timestamp.substr(0, spacePos);
            temp.append(".bin");
            temp.insert(0, "../data/binaries/");
            timeStampFileNames.push_back(temp);
        }
        file.close();

    return timeStampFileNames;

    }

}





