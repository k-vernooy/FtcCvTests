#include "../EOCVDummy.h"


class PositioningPipeline : public EOCVPipeline
{
public:    
    PositioningPipeline() {name = "Positioning Pipeline";}


    cv::Mat processFrame(cv::Mat input)
    {
        // cv::Mat x(input.size(), CV_8UC1, cv::Scalar(0));

        cv::cvtColor(input, output, cv::COLOR_BGR2GRAY);
        cv::Mat bilat;    
        cv::bilateralFilter(output, bilat, 4, 50, 50);
        // return bilat;
        output = bilat;
        // cv::threshold(output, output, 130, 255, cv::THRESH_BINARY);
        
        // cv::GaussianBlur(output, output, cv::Size(7, 7), 7);
        // cv::adaptiveThreshold(output, output, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY_INV, 11, 2);
        
        // Canny Edge Detection  
        std::vector<cv::Mat> channels;
        cv::Mat hsv;
        cv::cvtColor(input, hsv, cv::COLOR_RGB2HSV);
        cv::split(hsv, channels);
        cv::Canny(channels[2], output, 35, 150);
        // cv::Canny(channels[2], output, 35, 90);

        // return output;
        cv::morphologyEx(output, output, cv::MORPH_DILATE, cv::Mat::ones(cv::Size(3, 3), CV_32F));
        // cv::GaussianBlur(output, output, cv::Size(3, 3), 3);
        // cv::threshold(output, output, 30, 255, cv::THRESH_BINARY);
        // return output;

        // cv::Mat labels, stats, centroids;
        // int cc = cv::connectedComponentsWithStats(output, labels, stats, centroids, 8);
        
        // cv::Mat filtered = cv::Mat(input.size(), CV_8UC1, cv::Scalar(0,0,0));


        // for (int i = 1; i < cc; i++)
        // {
        //     cv::Mat interm;
        //     double area = stats.at<int>(i, cv::CC_STAT_AREA);
        //     if (area > 220)
        //     {
        //         cv::compare(labels, i, interm, cv::CMP_EQ);
        //         cv::bitwise_or(interm, filtered, filtered);
        //     }
            
        //     // if ( >= minLength && width > minWidth)
        //     // {
        //     //     // if the current component is large enough, add it to the `interm` mask
        // }

        
        // cv::morphologyEx(output, output, cv::MORPH_CLOSE, cv::Mat::ones(cv::Size(15, 15), CV_32F));

        // cv::bilateralFilter(contours, output, 9, 75, 75);
        // cv::Laplacian(output, output, CV_16S, 3);
        // cv::threshold(output, output, 120, 255, cv::THRESH_BINARY_INV);
        // cv::adaptiveThreshold(output, output, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 25, 4);
        // return output;

        // return output;
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(output, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        input.copyTo(output);


        std::vector<cv::Scalar> colors(3);
        colors[0] = cv::Scalar(255, 255, 0);
        colors[1] = cv::Scalar(255, 0, 255);
        colors[2] = cv::Scalar(0, 255, 255);

        
        // for (int i = 0; i < contours.size(); i++)
        //     cv::drawContours(output, contours, i, colors[i % 3], 2);


        std::vector<std::vector<cv::Point>> rects;

        for (int i = 0; i < contours.size(); i++)
        {
            std::vector<cv::Point> approx;
            cv::approxPolyDP(contours[i], approx, 0.04 * cv::arcLength(contours[i], true), true);
            
            if (approx.size() == 4)
                rects.push_back(approx);
        }


        for (int i = 0; i < rects.size(); i++)
            cv::drawContours(output, rects, i, cv::Scalar(0, 255, 0), 2);

        return output;
    }
};