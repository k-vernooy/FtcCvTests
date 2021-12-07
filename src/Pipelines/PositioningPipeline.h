#include "../EOCVDummy.h"


class PositioningPipeline : public EOCVPipeline
{
public:    
    PositioningPipeline() {name = "Barcode Pipeline";}


    cv::Mat processFrame(cv::Mat input)
    {   
        cv::cvtColor(input, output, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(output, output, cv::Size(5, 5), 5);
        cv::adaptiveThreshold(output, output, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 25, 4);
        // cv::Laplacian(output, output, CV_16S, 3);

        // cv::threshold(output, output, 120, 255, cv::THRESH_BINARY_INV);

        // cv::adaptiveThreshold(output, output, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 25, 4);


        return output;
    }
};