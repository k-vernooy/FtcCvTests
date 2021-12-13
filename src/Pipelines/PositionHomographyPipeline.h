#include "../EOCVDummy.h"



class PositionHomographyPipeline : public EOCVPipeline
{
public:    
    PositionHomographyPipeline() {name = "Positioning Pipeline";}


    cv::Mat processFrame(cv::Mat input)
    {
        // read sample image
        cv::Mat img1 = cv::imread("data/features1.png", 0);

        cv::Ptr<cv::xfeatures2d::SURF> siftPtr = cv::xfeatures2d::SURF::create();
        std::vector<cv::KeyPoint> kp1, kp2;
        cv::Mat des1, des2;

        
        siftPtr->detectAndCompute(img1, cv::noArray(), kp1, des1);
        siftPtr->detectAndCompute(input, cv::noArray(), kp2, des2);

        cv::FlannBasedMatcher matcher = cv::FlannBasedMatcher(cv::makePtr<cv::flann::KDTreeIndexParams>(5), cv::makePtr<cv::flann::SearchParams>(50));
        std::vector<std::vector<cv::DMatch>> matches;
        std::vector<cv::DMatch> goodMatches;
        
        if (kp1.size() < 2 && kp2.size() < 2) return input;

        matcher.knnMatch(des1, des2, matches, 2);
        goodMatches.reserve(matches.size());
        // std::cout << matches.size() << std::endl;


        for (int i = 0; i < matches.size(); i++)
        {
            if (matches[i].size() < 2)
                continue;
            const cv::DMatch& m1 = matches[i][0];
            const cv::DMatch& m2 = matches[i][1];
            // if (m1.distance < 0.7 * m2.distance)
            //     goodMatches.push_back(m1);

            if (m2.distance - m1.distance > 0.19)
                goodMatches.push_back(m1);
        }


        std::cout << goodMatches.size() << std::endl;
        cv::drawMatches(img1, kp1, input, kp2, goodMatches, output);
            
        input.copyTo(output);

        if (goodMatches.size() > 10)
        {
            std::vector<cv::Point2f> obj;
            std::vector<cv::Point2f> scene;

            for (size_t i = 0; i < goodMatches.size(); i++)
            {
                obj.push_back(kp1[goodMatches[i].queryIdx].pt);
                scene.push_back(kp2[goodMatches[i].trainIdx].pt);
            }

            cv::Mat mask;
            cv::Mat homography = cv::findHomography(obj, scene, cv::RANSAC, 5.0, mask);
            
            // cv::Mat matchesMask = mask.ravel().tolist()
        //     h,w = img1.shape
        //     pts = np.float32([ [0,0],[0,h-1],[w-1,h-1],[w-1,0] ]).reshape(-1,1,2)
            // dst = cv.perspectiveTransform(pts,M)
        //     img2 = cv.polylines(img2,[np.int32(dst)],True,255,3, cv.LINE_AA)

            std::vector<cv::Point2f> obj_corners(4);
            obj_corners[0] = cv::Point2f(0, 0);
            obj_corners[1] = cv::Point2f((float)img1.cols, 0 );
            obj_corners[2] = cv::Point2f((float)img1.cols, (float)img1.rows);
            obj_corners[3] = cv::Point2f(0, (float)img1.rows );

            std::vector<cv::Point2f> scene_corners(4);
            perspectiveTransform(obj_corners, scene_corners, homography);

            line(output, scene_corners[0], scene_corners[1], cv::Scalar(0, 255, 0), 4);
            line(output, scene_corners[1], scene_corners[2], cv::Scalar(0, 0, 255), 4);
            line(output, scene_corners[2], scene_corners[3], cv::Scalar(255, 0, 0), 4);
            line(output, scene_corners[3], scene_corners[0], cv::Scalar(0, 255, 255), 4);
            cv::Mat x;
            cv::warpPerspective(input, x, homography, img1.size(), cv::WARP_INVERSE_MAP);
            cv::cvtColor(x, x, cv::COLOR_BGR2GRAY);

            // cv::Mat trans_mat = (cv::Mat_<double>(2,3) << 1, 0, 15, 0, 1, 15);
            // warpAffine(x, x, trans_mat, x.size());

            cv::absdiff(img1, x, output);
        }

        // imshow("test", output);
        // cv::waitKey(10000);

        // exit(1);

        return output;
    }
};