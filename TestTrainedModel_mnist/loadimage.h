#ifndef LOADIMAGE_H
#define LOADIMAGE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>

class loadimage
{
public:
    loadimage();

    std::vector<std::vector<unsigned char>> readIDX3UByteFile(const std::string& filename);

    std::vector<std::vector<unsigned char>> readLabelFile(const std::string& filename);

};

#endif // LOADIMAGE_H
