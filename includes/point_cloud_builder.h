//
// Created by Tim Fernandez-Hart on 01/05/2020.
//

#ifndef OXFORD_CAR_VISUALISER_POINT_CLOUD_BUILDER_H
#define OXFORD_CAR_VISUALISER_POINT_CLOUD_BUILDER_H

#include <iostream>
#include <time.h>
#include <vector>

/*
 *  Use this class to load LiDAR binary files from the Oxford RobotCar dataset.
 *  The getPointCloud function returns a pointer with the data sorted into
 *  {x, y, x, I} * N floats.
 */
class PointCloudBuilder {

public:
    std::string convertTime(const std::string& filename);
    float* getPointCLoud (const std::string& filename);
    int vertexCountOfCloud(const std::string& filename);
    std::vector<std::string> loadTimesStamps(const std::string& filename);

};

#endif //OPEN_GL_LION_POINT_CLOUD_BUILDER_H
