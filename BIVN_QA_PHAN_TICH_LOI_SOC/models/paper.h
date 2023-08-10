#ifndef PAPER_H
#define PAPER_H
#include <opencv2/core.hpp>
#include <QString>
#include "checkarea.h"

class Paper{
public:
    QString name;
    QString path;


    std::vector<cv::Point> contentArea;
    cv::RotatedRect minRect;
    CheckArea grayArea;
    CheckArea blackArea;
    Paper();



public:
    static std::vector<std::shared_ptr<Paper>> items;
    static void scan();
};

#endif // PAPER_H
