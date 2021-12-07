#include "../EOCVDummy.h"


class BarcodePipeline : public EOCVPipeline
{
public:    
    BarcodePipeline() {name = "Barcode Pipeline";}
 
    cv::Mat processFrame(cv::Mat input)
    {
        // const cv::Rect ImageCrop = cv::Rect(220, 120, 500, 230);
        // input = input(ImageCrop);

        cv::Mat hsv;

        cv::cvtColor(input, hsv, cv::COLOR_BGR2HSV);
        cv::GaussianBlur(hsv, hsv, cv::Size(7, 7), 5);
        
        cv::Mat yellowRegions, redRegions;

        cv::inRange(hsv, cv::Scalar(15, 80, 80), cv::Scalar(40, 255, 255), yellowRegions);
        cv::morphologyEx(yellowRegions, yellowRegions, cv::MORPH_CLOSE, cv::Mat::ones(cv::Size(25, 25), CV_32F));
        cv::morphologyEx(yellowRegions, yellowRegions, cv::MORPH_OPEN, cv::Mat::ones(cv::Size(15, 15), CV_32F));
        
        cv::inRange(hsv, cv::Scalar(120, 80, 80), cv::Scalar(180, 255, 255), redRegions);
        cv::morphologyEx(redRegions, redRegions, cv::MORPH_CLOSE, cv::Mat::ones(cv::Size(25, 25), CV_32F));
        cv::morphologyEx(redRegions, redRegions, cv::MORPH_OPEN, cv::Mat::ones(cv::Size(15, 15), CV_32F));
        

        // cv::findContours(output, output, )

        // return output;
        // cv::Mat labels, stats, centroids;
        // int ncomponents = cv::connectedComponentsWithStats(yellowRegions, labels, stats, centroids, 8);
        // cv::Mat result = cv::Mat(labels.size(), CV_8U, cv::Scalar(0));

        // for (int i = 1; i < ncomponents; i++) {
        //     cv::Mat interm;
        //     // find the dimensions of the current component
            
        //     cv::Point2d centroid(centroids.at<double>(i, 0), centroids.at<double>(i, 1));
        // }

        
        std::vector<std::vector<cv::Point>> redContours;
        std::vector<std::vector<cv::Point>> yellowContours;
        cv::findContours(redRegions, redContours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
        cv::findContours(yellowRegions, yellowContours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

        cv::Mat yResult;
        input.copyTo(yResult, yellowRegions);
        

        cv::Mat rResult;
        input.copyTo(rResult, redRegions);
        cv::Mat result;

        cv::bitwise_or(rResult, yResult, result);

        // for (size_t idx = 0; idx < redContours.size(); idx++)
        // {
        //     // cv::Mat mask = cv::Mat::zeros(input.size(), CV_8U);

        //     // for (cv::Point& p : contours[idx])
        //     // {
        //     //     p.x += ImageCrop.x;
        //     //     p.y += ImageCrop.y;
        //     // }

        //     cv::drawContours(result, redContours, idx, cv::Scalar(0, 0, 255), 6);
        //     // cv::drawContours(contourImage, contours, idx, cv::Scalar(255));
        //     // input.copyTo(maskedInput, mask);
        //     // cv::bitwise_or(input, result, result, mask);
        //     // return maskedInput;
        // }
        
        
        return result;
        // return result;
    }
};