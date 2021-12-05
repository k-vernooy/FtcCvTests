
#include <iostream>
#include <string>

#include "EOCVDummy.h"
#include "Pipelines/BarcodePipeline.h"


int main(int argc, char** argv)
{
    srand(time(NULL));

    // must pass in a camera or img source
    assert(argc > 1);
    EOCVCamera cam;

    // TODO: looping image input srcs

    try
    {
        cam = EOCVCamera(std::stoi(argv[1]));
    }
    catch (...)
    {
        cam = EOCVCamera(argv[1]);
    }

    // Attach a processing pipeline to the camera
    EOCVPipeline* pipeline = new BarcodePipeline();
    cam.setPipeline(pipeline);

    // Constantly apply pipeline to incoming frames and display the result
    cam.startStreaming();

    delete pipeline;
    return 0;
}