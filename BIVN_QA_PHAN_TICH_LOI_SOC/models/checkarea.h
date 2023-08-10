#ifndef CHECKAREA_H
#define CHECKAREA_H

#include <opencv2/core.hpp>
#include <models/printerror.h>

class CheckArea{
public:
    cv::RotatedRect minRect;
    std::vector<cv::Point> contour;
    std::vector<PrintError> errors;
    CheckArea();
};

#endif // CHECKAREA_H
