#ifndef PRINTERROR_H
#define PRINTERROR_H
#include <vector>
#include <opencv2/core.hpp>

class PrintError{
public:
    cv::Point offset;
    cv::RotatedRect minRect;
    std::vector<cv::Point> contour;
    PrintError();
};

#endif // PRINTERROR_H
