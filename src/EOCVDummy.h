#ifndef EOCV_H
#define EOCV_H

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>


class EOCVPipeline
{
public:
    EOCVPipeline() {}

    virtual cv::Mat processFrame(cv::Mat input)
    {
        input.copyTo(output);
        return output;
    }

    std::string getName() {return name;}

protected:
    cv::Mat output;
    std::string name;
};


class EOCVCamera
{
public:
    EOCVCamera () {}
    EOCVCamera(std::string filename) : isLiveCam(false), isStatic(true), filename(filename) {}
    EOCVCamera(std::size_t camera) : isLiveCam(true), isStatic(false), camera(camera) {}


    void setPipeline(EOCVPipeline* pipeline_) {pipeline = pipeline_;}


    void startStreaming()
    {
        open();
        // cv::namedWindow(pipeline->getName(), cv::WINDOW_AUTOSIZE);
        assert(cap.isOpened());
        captureFrame(frame);

        while (true)
        {
            cv::resize(frame, frame, cv::Size(WinHeight, (int)((double)frame.rows * ((double)WinHeight / frame.cols))));

            cv::Mat outFrame = pipeline->processFrame(frame);
            cv::imshow(pipeline->getName(), outFrame);

            if ((char)cv::waitKey(1) == 32) break;
            if (!isStatic) captureFrame(frame);
        }
    }


    void open()
    {
        if (!isOpen)
        {
            if (isLiveCam) cap.open(camera, cv::CAP_ANY);
            else cap.open(filename);
        }
    }


    void close() {cap.release();}

    void captureFrame(cv::Mat& frame) {cap.read(frame);}

    bool getIsOpen() {return cap.isOpened();}

    void setStatic(bool isStatic_) {isStatic = isStatic_;}


private:
    cv::Mat frame;
    
    bool isLiveCam = true;
    bool isOpen = false;
    bool isStatic = false;

    std::size_t camera;
    std::string filename;

    cv::VideoCapture cap;
    EOCVPipeline* pipeline;

    const static int WinHeight = 780;
};


#endif