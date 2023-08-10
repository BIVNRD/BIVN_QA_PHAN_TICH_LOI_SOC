#include "processor.h"
#include <opencv2/core.hpp>
#include <opencv2/video.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <QFile>
#include <QCoreApplication>


#include <array>
#include <iostream>

using namespace std;
using namespace cv;

cv::Mat Processor::imageGray;
cv::Mat Processor::imageBGR;

bool Processor::loadImage(std::shared_ptr<Paper> paper){
    imageGray = cv::imread(paper->path.toStdString(), cv::IMREAD_GRAYSCALE);
    if(imageGray.step > imageGray.cols){
        cv::cvtColor(imageGray, imageGray, cv::COLOR_BGR2GRAY);
    }
    cv::cvtColor(imageGray, imageBGR, cv::COLOR_GRAY2BGR);
    return !imageBGR.empty();
}

cv::Mat computeDFT(Mat image);
void fftShift(Mat magI);
void lowpassFilter(const cv::Mat &dft_Filter, int distance);



// create an ideal low pass filter
void lowpassFilter(const cv::Mat &dft_Filter, int distance){
    Mat tmp = Mat(dft_Filter.rows, dft_Filter.cols, CV_32F);

    Point centre = Point(dft_Filter.rows / 2, dft_Filter.cols / 2);
    double radius;

    for(int i = 0; i < dft_Filter.rows; i++)
    {
        for(int j = 0; j < dft_Filter.cols; j++)
        {
            radius = (double) sqrt(pow((i - centre.x), 2.0) + pow((double) (j - centre.y), 2.0));
            if(radius>distance){
                tmp.at<float>(i,j) = (float)0;
            }else{
                tmp.at<float>(i,j) = (float)1;
            }

        }
    }

    Mat toMerge[] = {tmp, tmp};
    merge(toMerge, 2, dft_Filter);
}


// Compute the Discrete fourier transform
cv::Mat computeDFT(Mat image) {
    Mat padded;                            //expand input image to optimal size
    int m = getOptimalDFTSize( image.rows );
    int n = getOptimalDFTSize( image.cols ); // on the border add zero values
    copyMakeBorder(image, padded, 0, m - image.rows, 0, n - image.cols, BORDER_CONSTANT, Scalar::all(0));
    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complex;
    merge(planes, 2, complex);         // Add to the expanded another plane with zeros
    dft(complex, complex, DFT_COMPLEX_OUTPUT);  // fourier transform
    return complex;
}


void fftShift(Mat magI) {

    // crop if it has an odd number of rows or columns
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    int cx = magI.cols/2;
    int cy = magI.rows/2;

    Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

    Mat tmp;                            // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                     // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);
}


bool Processor::process(std::shared_ptr<Paper> paper){
    if(!loadImage(paper)) return false;

    int radius=1000;
    cv::Mat img, complexImg, filter, filterOutput, imgOutput, planes[2];
    imageGray.copyTo(img);

    complexImg = computeDFT(img);
    filter = complexImg.clone();

//    split(filter, planes);
//    imshow("dft", planes[1]);

    lowpassFilter(filter, radius); // create an ideal low pass filter

    fftShift(complexImg); // rearrage quadrants
    mulSpectrums(complexImg, filter, complexImg, 0); // multiply 2 spectrums
    fftShift(complexImg); // rearrage quadrants

    // compute inverse
    idft(complexImg, complexImg);

    split(complexImg, planes);
    normalize(planes[0], imgOutput, 0, 1, NORM_MINMAX);

    split(filter, planes);
    normalize(planes[1], filterOutput, 0, 1, NORM_MINMAX);

//    imshow("Input image", img);
//    waitKey(0);
//    imshow("Filter", filterOutput);
//    waitKey(0);
//    cv::Mat m;
//    cv::pyrDown(imgOutput, m);
//    imshow("Low pass filter", m);
//    waitKey(1);
//    destroyAllWindows();


    int rows = imgOutput.rows;
    int cols = imgOutput.cols;
    int cnt = rows * cols;
    float *src = (float*)imgOutput.data;
    std::vector<int> arr = std::vector<int>(cols,0);
    cv::Mat temp(rows, cols, CV_8UC1);
    for(int i=0;i<cnt; i++){
        temp.data[i] = src[i] * 255;
        arr[i % cols] += temp.data[i];
    }

    // value
    cv::Mat matValue(1, cols, CV_8UC1);
    for(int i=0;i<cols; i++){
        matValue.data[i] = arr[i] / rows;
    }

    cv::Mat matMedian;
    cv::medianBlur(matValue, matMedian, 19);

    cv::cvtColor(temp, imageBGR, cv::COLOR_GRAY2BGR);

    QStringList lineItems;
    lineItems.push_back("VALUE,MEDIAN,DIFF");
    for(int i=0;i<cols; i++){
        QString val = QString::number(matValue.data[i]);
        QString med = QString::number(matMedian.data[i]);
        QString diff = QString::number((int)matValue.data[i] - (int)matMedian.data[i]);
        lineItems.push_back(val + "," + med + "," + diff);
    }

    QString csvPath = paper->path + ".csv";
    QFile fileWriter(csvPath);
    if(fileWriter.open(QIODevice::WriteOnly)){
        fileWriter.write(lineItems.join("\n").toStdString().c_str());
        fileWriter.close();
    }

    return true;
}
