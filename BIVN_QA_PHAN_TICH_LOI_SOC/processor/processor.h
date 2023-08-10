#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "models/paper.h"
#include <memory>
class Processor{
public:
    static cv::Mat imageGray;
    static cv::Mat imageBGR;



    static bool loadImage(std::shared_ptr<Paper> paper);
//    static void threshByRange(cv::Mat &in, cv::Mat &out, int lower, int upper);
//    static bool clipContentArea(std::shared_ptr<Paper> paper);
//    static bool detectArea(std::shared_ptr<Paper> paper);

//    static bool detectObjectErrorsOnBlack(std::shared_ptr<Paper> paper);
//    static bool detectLineErrorsOnBlack(std::shared_ptr<Paper> paper);















//    static cv::Mat computeDFT(cv::Mat image);
//    static void fftShift(cv::Mat magI);
//    static cv::Mat magnitudeSpectrum(cv::Mat complex);
//    static void lowpassFilter(const cv::Mat &dft_Filter, double distance);
//    static void highpassFilter(cv::Mat &dft_Filter, double distance);
//    static cv::Mat computeIDFT(const cv::Mat &complexImage);




    static bool BIL_GRAY_FFT(std::shared_ptr<Paper> paper);
    static void BIVN_HANDLE(std::shared_ptr<Paper> paper);















    static void draw(std::shared_ptr<Paper> paper);
    static bool process(std::shared_ptr<Paper> paper);




};

#endif // PROCESSOR_H
