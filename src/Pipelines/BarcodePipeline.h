#include "../EOCVDummy.h"


class BarcodePipeline : public EOCVPipeline
{
public:    
    BarcodePipeline() {name = "Barcode Pipeline";}
 
    cv::Mat processFrame(cv::Mat input)
    {
        // const cv::Rect ImageCrop = cv::Rect(220, 120, 500, 230);
        input.copyTo(output);
        // output = input(ImageCrop);
        cv::cvtColor(output, output, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(output, output, cv::Size(7, 7), 5);
        // cv::threshold(output, output, 120, 255, cv::THRESH_BINARY);

        // cv::adaptiveThreshold(output, output, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 25, 5);

        // cv::adaptiveThreshold(output, output, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 25, 4);
        cv::adaptiveThreshold(output, output, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 25, 4);

        cv::GaussianBlur(output, output, cv::Size(65, 65), 9);
        cv::threshold(output, output, 30, 255, cv::THRESH_BINARY);

        // return output;
        // cv::

        // cv::findContours(output, output, )

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(output, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

        std::cout << contours.size() << std::endl;

        //Draw the output
        cv::Mat contourImage; //(output.size(), CV_8UC3, cv::Scalar(0,0,0));
        input.copyTo(contourImage);

        cv::Mat result = cv::Mat::zeros(input.size(), input.type());
        
        for (size_t idx = 0; idx < contours.size(); idx++)
        {
            cv::Mat mask = cv::Mat::zeros(input.size(), CV_8U);

            // for (cv::Point& p : contours[idx])
            // {
            //     p.x += ImageCrop.x;
            //     p.y += ImageCrop.y;
            // }

            cv::drawContours(mask, contours, idx, cv::Scalar(255), -1);
            // cv::drawContours(contourImage, contours, idx, cv::Scalar(255));
            // input.copyTo(maskedInput, mask);
            cv::bitwise_or(input, result, result, mask);
            // return maskedInput;
        }
        
        return result;
    }
};