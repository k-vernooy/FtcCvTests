# FtcCvTests

A repo, written in C++ to emulate the FTC [EasyOpenCV](https://github.com/OpenFTC/EasyOpenCV) library, that contains a collection of Pipelines for various computer-vision related tasks in 2021's Freight Frenzy challenge. This was made to allow development without having access to the necessary Android phones, and to allow for faster testing.


## Usage
The actual CV development and processing should occur inside a "Pipeline" class that inherits from `EOCVPipeline`, where the `processFrame` method is redefined to perform the necessary image processing. Such pipelines are located in `src/Pipelines`.

`src/OpMode.cpp` is to be compiled, containing the main method where pipelines are constructed and passed into instances of the camera. If you want to test a different pipeline, just change which one is constructed.

Compilation & running is as simple as `make && bin/OpMode [webcam id/images]`, assuming OpenCV is properly installed.