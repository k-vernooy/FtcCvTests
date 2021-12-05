#include "EOCVDummy.h"


class BarcodePipeline : public EOCVPipeline
{
public:    
    BarcodePipeline() {name = "Barcode Pipeline";}
 
    cv::Mat processFrame(cv::Mat& input) override
    {
        cv::cvtColor(input, output, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(output, output, cv::Size(1, 7), 0);
        cv::Laplacian(output, output, CV_16S, 3);
        return output;
    }
};