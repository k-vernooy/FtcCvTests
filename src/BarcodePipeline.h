#include "EOCVDummy.h"


class BarcodePipeline : public EOCVPipeline
{
public:    
    BarcodePipeline() {name = "Barcode Pipeline";}
 
    cv::Mat processFrame(cv::Mat input) override
    {
        const cv::Rect ImageCrop = cv::Rect(220, 120, 500, 230);

        output = input(ImageCrop);
        cv::cvtColor(output, output, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(output, output, cv::Size(7, 7), 5);
        // cv::threshold(output, output, 120, 255, cv::THRESH_BINARY);

        cv::adaptiveThreshold(output, output, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 25, 5);
        cv::GaussianBlur(output, output, cv::Size(55, 55), 5);
        cv::threshold(output, output, 40, 255, cv::THRESH_BINARY);

        // cv::adaptiveThreshold(output, output, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 25, 3);
        // cv::

        // cv::findContours(output, output, )

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(output, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

        std::cout << contours.size() << std::endl;

        //Draw the output
        cv::Mat contourImage(output.size(), CV_8UC3, cv::Scalar(0,0,0));
        
        cv::Scalar colors[3];
        colors[0] = cv::Scalar(255, 0, 0);
        colors[1] = cv::Scalar(0, 255, 0);
        colors[2] = cv::Scalar(0, 0, 255);

        for (size_t idx = 0; idx < contours.size(); idx++) {
            // if (contours[idx].size() < 10)
            cv::drawContours(contourImage, contours, idx, colors[idx % 3]);
        }
        
        
        // cv::GaussianBlur(output, output, cv::Size(1, 7), 0);
        // cv::Laplacian(output, output, CV_16S, 3);
        // return output;
        return contourImage;
    }
};