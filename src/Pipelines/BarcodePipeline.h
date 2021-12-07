#include "../EOCVDummy.h"


class BarcodePipeline : public EOCVPipeline
{
public:    
    BarcodePipeline() {name = "Barcode Pipeline";}
 
    cv::Mat processFrame(cv::Mat input)
    {
        // const cv::Rect ImageCrop = cv::Rect(220, 120, 500, 230);
        // input = input(ImageCrop);

        // convert input image to HSV space and perform basic blur
        cv::Mat hsv;
        cv::cvtColor(input, hsv, cv::COLOR_BGR2HSV);
        cv::GaussianBlur(hsv, hsv, cv::Size(7, 7), 5);
        

        // Do HSV thresholding to identify the barcode tape as well as the shipping element
        cv::Mat capRegions, tapeRegions;
    
        cv::inRange(hsv, cv::Scalar(15, 80, 80), cv::Scalar(40, 255, 255), capRegions);
        cv::morphologyEx(capRegions, capRegions, cv::MORPH_CLOSE, cv::Mat::ones(cv::Size(25, 25), CV_32F));
        cv::morphologyEx(capRegions, capRegions, cv::MORPH_OPEN, cv::Mat::ones(cv::Size(35, 35), CV_32F));
        // cv::morphologyEx(capRegions, capRegions, cv::MORPH_CLOSE, cv::Mat::ones(cv::Size(25, 25), CV_32F));
        // cv::morphologyEx(capRegions, capRegions, cv::MORPH_OPEN, cv::Mat::ones(cv::Size(15, 15), CV_32F));
        
        cv::inRange(hsv, cv::Scalar(120, 80, 80), cv::Scalar(180, 255, 255), tapeRegions);
        cv::morphologyEx(tapeRegions, tapeRegions, cv::MORPH_CLOSE, cv::Mat::ones(cv::Size(25, 25), CV_32F));
        cv::morphologyEx(tapeRegions, tapeRegions, cv::MORPH_OPEN, cv::Mat::ones(cv::Size(15, 15), CV_32F));
        

        // Determine the centroids of the tape regions
        std::vector<double> tapeCentroidsX;
        cv::Mat tapeLabels, tapeStats, tapeCentroids;
        int tapeComponentsCount = cv::connectedComponentsWithStats(tapeRegions, tapeLabels, tapeStats, tapeCentroids, 8);
        
        // For now, we'll make sure that we're identifying only two non-cap tapes.
        assert(tapeComponentsCount == 3);

        for (int i = 1; i < tapeComponentsCount; i++) tapeCentroidsX.push_back(tapeCentroids.at<double>(i, 0));

        // Make sure the centroids are listed in ascending order of X-coordinate (left-to-right, in screen space)
        if (tapeCentroidsX[0] > tapeCentroidsX[1]) std::swap(tapeCentroidsX[0], tapeCentroidsX[1]);


        // Determine the centroid of the cap region
        cv::Mat capLabels, capStats, capCentroids;
        int capComponentsCount = cv::connectedComponentsWithStats(capRegions, capLabels, capStats, capCentroids, 8);

        assert(capComponentsCount == 2);
        double capCentroidX = capCentroids.at<double>(1, 0);


        // Check the positoin of the cap region relative to the tape regions
        int barcodePosition = -1;

        if (capCentroidX < tapeCentroidsX[0]) barcodePosition = 1;
        else if (capCentroidX < tapeCentroidsX[1]) barcodePosition = 2;
        else barcodePosition = 3;


        std::cout << barcodePosition << std::endl;

        
        // std::vector<std::vector<cv::Point>> tapeContours;
        // std::vector<std::vector<cv::Point>> capContours;
        // cv::findContours(tapeRegions, tapeContours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
        // cv::findContours(capRegions, capContours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

        // cv::Mat yResult;
        // input.copyTo(yResult, capRegions);
        

        // cv::Mat rResult;
        // input.copyTo(rResult, tapeRegions);
        // cv::Mat result;

        // cv::bitwise_or(rResult, yResult, result);

        // for (size_t idx = 0; idx < tapeContours.size(); idx++)
        // {
        //     // cv::Mat mask = cv::Mat::zeros(input.size(), CV_8U);

        //     // for (cv::Point& p : contours[idx])
        //     // {
        //     //     p.x += ImageCrop.x;
        //     //     p.y += ImageCrop.y;
        //     // }

        //     cv::drawContours(result, tapeContours, idx, cv::Scalar(0, 0, 255), 6);
        //     // cv::drawContours(contourImage, contours, idx, cv::Scalar(255));
        //     // input.copyTo(maskedInput, mask);
        //     // cv::bitwise_or(input, result, result, mask);
        //     // return maskedInput;
        // }
        
        
        return input;
    }
};