//#include "processor.h"
//#include <opencv2/core.hpp>
//#include <opencv2/video.hpp>
//#include <opencv2/highgui.hpp>
//#include <opencv2/imgproc.hpp>
//#include <QFile>
//#include <QCoreApplication>
//#include <iostream>

//Processor::Processor(){

//}


//cv::Mat Processor::imageGray;
//cv::Mat Processor::imageBGR;
//cv::Mat Processor::imageBILFFT;

//bool Processor::loadImage(std::shared_ptr<Paper> paper){
//    imageGray = cv::imread(paper->path.toStdString(), cv::IMREAD_GRAYSCALE);
//    if(imageGray.step > imageGray.cols){
//        cv::cvtColor(imageGray, imageGray, cv::COLOR_BGR2GRAY);
//    }
//    cv::cvtColor(imageGray, imageBGR, cv::COLOR_GRAY2BGR);
//    return !imageBGR.empty();
//}

//void Processor::threshByRange(cv::Mat &in, cv::Mat &out, int lower, int upper){
//    if(in.data != out.data){
//        in.copyTo(out);
//    }
//    int cnt = in.step * in.rows;
//    for(int i=0;i<cnt; i++){
//        if(in.data[i] < lower) out.data[i] = 0;
//        else if(in.data[i] > upper) out.data[i] = 0;
//        else out.data[i]=255;
//    }
//}

//bool Processor::clipContentArea(std::shared_ptr<Paper> paper){
//    cv::Mat matContent;
//    threshByRange(imageGray, matContent, 10,230);
//    cv::erode(matContent, matContent, cv::getStructuringElement(cv::MORPH_ERODE, cv::Size(49,49)));
//    cv::rectangle(matContent, cv::Rect(0,0,300,300), cv::Scalar(0));
//    std::vector<std::vector<cv::Point>> contours;
//    cv::findContours(matContent, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);
//    int maxArea = 0;
//    cv::RotatedRect minRect;
//    int matchIndex = -1;
//    for(int i=0;i<contours.size(); i++){
//        int area = cv::contourArea(contours[i]);
//        if(area> maxArea){
//            maxArea = area;
//            matchIndex = i;
//            minRect = cv::minAreaRect(contours[i]);
//        }
//    }
//    if(matchIndex >=0){
//        cv::Rect rect = cv::boundingRect(contours[matchIndex]);
//        std::cout << rect.x << "/" << maxArea << std::endl;
//        cv::Mat(imageGray, rect).copyTo(matContent);
//        matContent.copyTo(imageGray);
//        cv::cvtColor(imageGray, imageBGR, cv::COLOR_GRAY2BGR);
//        paper->contentArea = contours[matchIndex];
//        for(auto &it : paper->contentArea){
//            it.x -= rect.x;
//            it.y -= rect.y;
//        }
//    }
//    return true;
//}


//bool Processor::detectArea(std::shared_ptr<Paper> paper){
//    cv::Mat matContent;
//    cv::Mat matBin;
//    imageGray.copyTo(matContent);

//    // black
//    cv::threshold(matContent, matBin,80,255, cv::THRESH_BINARY_INV);
//    cv::erode(matBin, matBin, cv::getStructuringElement(cv::MORPH_ERODE, cv::Size(7,1)));
//    std::vector<std::vector<cv::Point>> contours;
//    cv::findContours(matBin, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);
//    int maxArea = 0;
//    int matchIndex = -1;
//    for(int i=0;i<contours.size(); i++){
//        int area = cv::contourArea(contours[i]);
//        if(area> maxArea){
//            maxArea = area;
//            matchIndex = i;
//        }
//    }
//    if(matchIndex >=0){
//        cv::convexHull(contours[matchIndex], paper->blackArea.contour);
//        paper->blackArea.minRect = cv::minAreaRect(paper->blackArea.contour);
//    }

//    cv::threshold(matContent, matBin,80,255, cv::THRESH_BINARY);
//    cv::dilate(matBin, matBin, cv::getStructuringElement(cv::MORPH_DILATE, cv::Size(7,1)));
//    contours.clear();
//    contours.push_back(paper->contentArea);
//    contours.push_back(paper->blackArea.contour);
//    cv::drawContours(matBin, contours,0, cv::Scalar(0),7);
//    cv::drawContours(matBin, contours,1, cv::Scalar(0),7);

//    contours.clear();
//    cv::findContours(matBin, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);
//    maxArea = 0;
//    matchIndex = -1;
//    for(int i=0;i<contours.size(); i++){
//        int area = cv::contourArea(contours[i]);
//        if(area> maxArea){
//            maxArea = area;
//            matchIndex = i;
//        }
//    }
//    if(matchIndex >=0){
//        cv::convexHull(contours[matchIndex], paper->grayArea.contour);
//        paper->grayArea.minRect = cv::minAreaRect(paper->grayArea.contour);
//    }
//    return true;
//}


//bool Processor::detectObjectErrorsOnBlack(std::shared_ptr<Paper> paper){
//    if(paper->blackArea.contour.empty()) return false;
//    cv::Mat imageInput;
//    cv::Rect roi = cv::boundingRect(paper->blackArea.contour);
//    cv::Mat(imageGray, roi).copyTo(imageInput);
//    cv::blur(imageInput, imageInput, cv::Size(3,3));

//    cv::Mat matBack;
//    cv::blur(imageInput, matBack, cv::Size(39,39));

//    int cnt = imageInput.step * imageInput.rows;
//    for(int i=0;i<cnt;i++){
//        int thresh = matBack.data[i] + 15;
//        imageInput.data[i] = imageInput.data[i] > thresh ? 255: 0;
//    }
//    //    matBack.release();

//    cv::dilate(imageInput, imageInput, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));

//    std::vector<std::vector<cv::Point>> contours;
//    cv::Mat mask = cv::Mat::zeros(imageInput.rows, imageInput.cols,CV_8UC1);
//    contours.push_back(paper->blackArea.contour);
//    cv::drawContours(mask, contours,0, cv::Scalar(255),cv::FILLED, cv::LINE_8, cv::noArray(), INT_MAX, cv::Point(-roi.x, - roi.y));
//    cv::drawContours(mask, contours,0, cv::Scalar(0),4, cv::LINE_8, cv::noArray(), INT_MAX, cv::Point(-roi.x, - roi.y));
//    imageInput &= mask;
//    mask.release();

//    contours.clear();
//    cv::findContours(imageInput, contours, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);
//    for(auto &ct : contours){
//        cv::RotatedRect minRect = cv::minAreaRect(ct);
//        double w = minRect.size.width;
//        double h = minRect.size.height;
//        if(w < 10 || h < 10) continue;

//        PrintError err;
//        err.minRect = minRect;
//        err.contour = ct;
//        err.offset = cv::Point(roi.x, roi.y);
//        paper->blackArea.errors.push_back(err);
//    }

//    return true;
//}

//bool Processor::detectLineErrorsOnBlack(std::shared_ptr<Paper> paper){
//    return true;
//}












////cv::Mat Processor::computeDFT(cv::Mat image){
////    cv::Mat padded;                            //expand input image to optimal size
////    int m = cv::getOptimalDFTSize( image.rows );
////    int n = cv::getOptimalDFTSize( image.cols ); // on the border add zero values
////    cv::copyMakeBorder(image, padded, 0, m - image.rows, 0, n - image.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
////    cv::Mat planes[] = {cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F)};
////    cv::Mat complex;
////    merge(planes, 2, complex);         // Add to the expanded another plane with zeros
////    dft(complex, complex, cv::DFT_COMPLEX_OUTPUT);  // fourier transform
////    return complex;
////}

////void Processor::fftShift(cv::Mat magI){
////    // crop if it has an odd number of rows or columns
////    magI = magI(cv::Rect(0, 0, magI.cols & -2, magI.rows & -2));

////    int cx = magI.cols/2;
////    int cy = magI.rows/2;

////    cv::Mat q0(magI, cv::Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
////    cv::Mat q1(magI, cv::Rect(cx, 0, cx, cy));  // Top-Right
////    cv::Mat q2(magI, cv::Rect(0, cy, cx, cy));  // Bottom-Left
////    cv::Mat q3(magI, cv::Rect(cx, cy, cx, cy)); // Bottom-Right

////    cv::Mat tmp;                            // swap quadrants (Top-Left with Bottom-Right)
////    q0.copyTo(tmp);
////    q3.copyTo(q0);
////    tmp.copyTo(q3);

////    q1.copyTo(tmp);                     // swap quadrant (Top-Right with Bottom-Left)
////    q2.copyTo(q1);
////    tmp.copyTo(q2);
////}

////cv::Mat Processor::magnitudeSpectrum(cv::Mat complex) {
////    cv::Mat magI;
////    cv::Mat planes[] = {
////        cv::Mat::zeros(complex.size(), CV_32F),
////        cv::Mat::zeros(complex.size(), CV_32F)
////    };
////    split(complex, planes); // planes[0] = Re(DFT(I)), planes[1] = Im(DFT(I))
////    magnitude(planes[0], planes[1], magI); // sqrt(Re(DFT(I))^2 + Im(DFT(I))^2)
////    // switch to logarithmic scale: log(1 + magnitude)
////    magI += cv::Scalar::all(1);
////    log(magI, magI);
////    fftShift(magI); // rearrage quadrants
////    // Transform the magnitude matrix into a viewable image (float values 0-1)
////    cv::normalize(magI, magI, 0, 1, cv::NORM_MINMAX);

////    return magI;
////}

////void Processor::lowpassFilter(const cv::Mat &dft_Filter, double distance){
////    cv::Mat tmp = cv::Mat(dft_Filter.rows, dft_Filter.cols, CV_32F);
////    cv::Point centre = cv::Point(dft_Filter.rows / 2, dft_Filter.cols / 2);
////    double radius;

////    for(int i = 0; i < dft_Filter.rows; i++)
////    {
////        for(int j = 0; j < dft_Filter.cols; j++)
////        {
////            radius = (double) sqrt(pow((i - centre.x), 2.0) + pow((double) (j - centre.y), 2.0));
////            if(radius>distance){
////                tmp.at<float>(i,j) = (float)0;
////            }else{
////                tmp.at<float>(i,j) = (float)1;
////            }

////        }
////    }

////    cv::Mat toMerge[] = {tmp, tmp};
////    merge(toMerge, 2, dft_Filter);
////}


////// Compute the low pass highpass
////void Processor::highpassFilter(cv::Mat &dft_Filter, double distance){
////    cv::Mat tmp = cv::Mat(dft_Filter.rows, dft_Filter.cols, CV_32F);

////    cv::Point centre = cv::Point(dft_Filter.rows / 2, dft_Filter.cols / 2);
////    double radius;

////    for(int i = 0; i < dft_Filter.rows; i++)
////    {
////        for(int j = 0; j < dft_Filter.cols; j++)
////        {
////            radius = (double) sqrt(pow((i - centre.x), 2.0) + pow((double) (j - centre.y), 2.0));
////            if(radius>distance){
////                tmp.at<float>(i,j) = (float)1;
////            }else{
////                tmp.at<float>(i,j) = (float)0;
////            }

////        }
////    }

////    cv::Mat toMerge[] = {tmp, tmp};
////    merge(toMerge, 2, dft_Filter);
////}

////// Compute the inverse of the Fourier Transform
////cv::Mat Processor::computeIDFT(const cv::Mat &complexImage) {
////    //calculating the idft
////    cv::Mat inverseTransform;
////    cv::dft(complexImage, inverseTransform, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);
////    cv::normalize(inverseTransform, inverseTransform, 0, 1, cv::NORM_MINMAX);
////    return inverseTransform;
////}









//cv::Mat computeDFT(cv::Mat image);
//void fftShift(cv::Mat magI);
//void lowpassFilter(const cv::Mat &dft_Filter, int distance);



//// Compute the Discrete fourier transform
//cv::Mat computeDFT(cv::Mat image) {
//    cv::Mat padded;                            //expand input image to optimal size
//    int m = cv::getOptimalDFTSize( image.rows );
//    int n = cv::getOptimalDFTSize( image.cols ); // on the border add zero values
//    cv::copyMakeBorder(image, padded, 0, m - image.rows, 0, n - image.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
//    cv::Mat planes[] = {cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F)};
//    cv::Mat complex;
//    cv::merge(planes, 2, complex);         // Add to the expanded another plane with zeros
//    cv::dft(complex, complex, cv::DFT_COMPLEX_OUTPUT);  // fourier transform
//    return complex;
//}

//void fftShift(cv::Mat magI) {

//    // crop if it has an odd number of rows or columns
//    magI = magI(cv::Rect(0, 0, magI.cols & -2, magI.rows & -2));

//    int cx = magI.cols/2;
//    int cy = magI.rows/2;

//    cv::Mat q0(magI, cv::Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
//    cv::Mat q1(magI, cv::Rect(cx, 0, cx, cy));  // Top-Right
//    cv::Mat q2(magI, cv::Rect(0, cy, cx, cy));  // Bottom-Left
//    cv::Mat q3(magI, cv::Rect(cx, cy, cx, cy)); // Bottom-Right

//    cv::Mat tmp;                            // swap quadrants (Top-Left with Bottom-Right)
//    q0.copyTo(tmp);
//    q3.copyTo(q0);
//    tmp.copyTo(q3);

//    q1.copyTo(tmp);                     // swap quadrant (Top-Right with Bottom-Left)
//    q2.copyTo(q1);
//    tmp.copyTo(q2);
//}

//// create an ideal low pass filter
//void lowpassFilter(const cv::Mat &dft_Filter, int distance)
//{
//    cv::Mat tmp = cv::Mat(dft_Filter.rows, dft_Filter.cols, CV_32F);

//    cv::Point centre = cv::Point(dft_Filter.rows / 2, dft_Filter.cols / 2);
//    double radius;

//    for(int i = 0; i < dft_Filter.rows; i++)
//    {
//        for(int j = 0; j < dft_Filter.cols; j++)
//        {
//            radius = (double) sqrt(pow((i - centre.x), 2.0) + pow((double) (j - centre.y), 2.0));
//            if(radius>distance){
//                tmp.at<float>(i,j) = (float)0;
//            }else{
//                tmp.at<float>(i,j) = (float)1;
//            }

//        }
//    }

//    cv::Mat toMerge[] = {tmp, tmp};
//    merge(toMerge, 2, dft_Filter);
//}


//bool Processor::BIL_GRAY_FFT(std::shared_ptr<Paper> paper){
//    int radius=20;
//    cv::Mat image;
//    cv::Mat(imageGray, cv::boundingRect(paper->grayArea.contour)).copyTo(image);

//    cv::Mat complexImg, filter, filterOutput, imgOutput, planes[2];

//    cv::Mat matF32;
//    image.convertTo(matF32, CV_32FC1);

//    cv::Mat padded;                            //expand input image to optimal size
//    int m = cv::getOptimalDFTSize( image.rows );
//    int n = cv::getOptimalDFTSize( image.cols ); // on the border add zero values
//    copyMakeBorder(image, padded, 0, m - image.rows, 0, n - image.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));


//    cv::imshow("V", padded);





















//    //    complexImg = computeDFT(I);
//    //    filter = complexImg.clone();

//    ////    lowpassFilter(filter, radius); // create an ideal low pass filter

//    ////    fftShift(complexImg); // rearrage quadrants
//    //    mulSpectrums(complexImg, filter, complexImg, 0); // multiply 2 spectrums
//    ////    fftShift(complexImg); // rearrage quadrants




//    ////    // compute inverse
//    ////    idft(complexImg, complexImg);

//    //    split(complexImg, planes);
//    //    normalize(planes[0], imgOutput, 0, 1, cv::NORM_MINMAX);

//    //    split(filter, planes);
//    //    normalize(planes[1], filterOutput, 0, 1, cv::NORM_MINMAX);

//    //    cv::imshow("Filter", filterOutput);
//    //    cv::waitKey(0);
//    //    cv::imshow("Low pass filter", imgOutput);
//    //    cv::waitKey(0);
//    //    cv::destroyAllWindows();




//    //    cv::Mat padded;                            //expand input image to optimal size
//    //    int m = cv::getOptimalDFTSize( I.rows );
//    //    int n = cv::getOptimalDFTSize( I.cols ); // on the border add zero values
//    //    copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

//    //    cv::Mat planes[] = {cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F)};
//    //    cv::Mat complexI;
//    //    merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

//    //    dft(complexI, complexI);            // this way the result may fit in the source matrix

//    //    // compute the magnitude and switch to logarithmic scale
//    //    // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
//    //    split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
//    //    magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
//    //    cv::Mat magI = planes[0];

//    //    magI += cv::Scalar::all(1);                    // switch to logarithmic scale
//    //    log(magI, magI);

//    //    // crop the spectrum, if it has an odd number of rows or columns
//    //    magI = magI(cv::Rect(0, 0, magI.cols & -2, magI.rows & -2));

//    //    // rearrange the quadrants of Fourier image  so that the origin is at the image center
//    //    int cx = magI.cols/2;
//    //    int cy = magI.rows/2;

//    //    cv::Mat q0(magI, cv::Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
//    //    cv::Mat q1(magI, cv::Rect(cx, 0, cx, cy));  // Top-Right
//    //    cv::Mat q2(magI, cv::Rect(0, cy, cx, cy));  // Bottom-Left
//    //    cv::Mat q3(magI, cv::Rect(cx, cy, cx, cy)); // Bottom-Right

//    //    cv::Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
//    //    q0.copyTo(tmp);
//    //    q3.copyTo(q0);
//    //    tmp.copyTo(q3);

//    //    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
//    //    q2.copyTo(q1);
//    //    tmp.copyTo(q2);

//    //    normalize(magI, magI, 0, 1, cv::NORM_MINMAX); // Transform the matrix with float values into a
//    //        // viewable image form (float between values 0 and 1).

//    //    cv::imshow("Input Image"       , I   );    // Show the result
//    //    cv::imshow("spectrum magnitude", magI);
//    //    cv::waitKey();


//    return true;
//}




//void Processor::BIVN_HANDLE(std::shared_ptr<Paper> paper){

//}

//void Processor::draw(std::shared_ptr<Paper> paper){
//    std::vector<std::vector<cv::Point>> contours;
//    if(paper->blackArea.contour.empty() == false){
//        contours.push_back(paper->blackArea.contour);
//        cv::drawContours(imageBGR, contours, 0, cv::Scalar(0,128,0),2);
//        contours.clear();
//    }
//    if(paper->grayArea.contour.empty() == false){
//        contours.push_back(paper->grayArea.contour);
//        cv::drawContours(imageBGR, contours, 0, cv::Scalar(0,0,128),2);
//        contours.clear();
//    }

//    std::vector<CheckArea*> areas = {&paper->blackArea, &paper->grayArea };
//    for(auto area : areas){
//        for(auto &e : area->errors){
//            cv::Point2f points[4];
//            cv::RotatedRect rect = e.minRect;
//            rect.center.x += e.offset.x;
//            rect.center.y += e.offset.y;
//            rect.points(points);
//            cv::line(imageBGR, points[0], points[1], cv::Scalar(0,0,255));
//            cv::line(imageBGR, points[1], points[2], cv::Scalar(0,0,255));
//            cv::line(imageBGR, points[2], points[3], cv::Scalar(0,0,255));
//            cv::line(imageBGR, points[3], points[0], cv::Scalar(0,0,255));
//        }
//    }

//}

//bool Processor::process(std::shared_ptr<Paper> paper){
//    if(!loadImage(paper)) return false;

//    cv::Mat image;
//    imageGray.copyTo(image);

//    // comppute DFT
//    cv::Mat padded;                            //expand input image to optimal size
//    int m = cv::getOptimalDFTSize( image.rows );
//    int n = cv::getOptimalDFTSize( image.cols ); // on the border add zero values
//    cv::copyMakeBorder(image, padded, 0, m - image.rows, 0, n - image.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
//    cv::Mat planes[] = {cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F)};
//    cv::Mat complex;
//    merge(planes, 2, complex);         // Add to the expanded another plane with zeros
//    cv::dft(complex, complex, cv::DFT_COMPLEX_OUTPUT);  // fourier transform


//    // low pass filter
//    int distance = 20;
//    cv::Mat filter = complex.clone();
//    cv::Mat tmp = cv::Mat(filter.rows, filter.cols, CV_32F);
//    cv::Point centre = cv::Point(filter.rows / 2, filter.cols / 2);
//    double radius;

//    for(int i = 0; i < filter.rows; i++)
//    {
//        for(int j = 0; j < filter.cols; j++)
//        {
//            radius = (double) sqrt(pow((i - centre.x), 2.0) + pow((double) (j - centre.y), 2.0));
//            if(radius>distance){
//                tmp.at<float>(i,j) = (float)0;
//            }else{
//                tmp.at<float>(i,j) = (float)1;
//            }

//        }
//    }

//    cv::Mat toMerge[] = {tmp, tmp};
//    cv::merge(toMerge, 2, filter);


//    return true;
//}
