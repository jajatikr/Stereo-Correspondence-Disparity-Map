#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/calib3d.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;
using namespace cv;

void descriptor(Mat imgL, Mat imgR, Mat imgD, string descript, string imgSz)
{
    // Start timer
    int64 t = getTickCount();
    
    // Initialize keypoints, descriptors and type of descriptor
    vector<KeyPoint> keypointsL, keypointsR;
    Mat descriptorsL, descriptorsR;
    Ptr<Feature2D> ftrD;

    // Type of descriptor: BRISK or ORB
    if (descript == "BRISK") {
        cout << "BRISK " + imgSz << endl;
        ftrD = BRISK::create();
    }
    else if (descript == "ORB") {
        cout << "ORB " + imgSz << endl;
        ftrD = ORB::create();
    }
    else {
        cout << "Please provide BRISK or ORB descriptor" << endl;
        //return 0;
    }

    // Detect keypoints and describe features
    ftrD->detectAndCompute(imgL, Mat(), keypointsL, descriptorsL, false);
    ftrD->detectAndCompute(imgR, Mat(), keypointsR, descriptorsR, false);

    // Initialize descriptor matcher variables and result image
    vector<DMatch> matches, bestMatches;
    Mat result;

    // Match descriptors
    Ptr<DescriptorMatcher> descriptorMatcher = DescriptorMatcher::create("BruteForce-Hamming"); //FlannBased or BruteForce-Hamming
    descriptorMatcher->match(descriptorsL, descriptorsR, matches, Mat());
   
    // Best matches - Sort distance by ascending order between descriptor matches
    // and select indices of smallest 40 distances | Reference: OpenCV opensource sample cpp code
    Mat index;
    int nbMatch1 = int(matches.size());
    Mat tab(nbMatch1, 1, CV_32F);
    for (int i = 0; i<nbMatch1; i++) {
        tab.at<float>(i, 0) = matches[i].distance;
    }
    sortIdx(tab, index, SORT_EVERY_COLUMN + SORT_ASCENDING);
    for (int i = 0; i<40; i++) {
        bestMatches.push_back(matches[index.at<int>(i, 0)]);
    }

    //Draw bestMatches and save resulting image
    drawMatches(imgL, keypointsL, imgR, keypointsR, bestMatches, result); //result: 8UC3 CV_8U 1026x444 i.e. 513+513x444
    string winname = descript + "-" + imgSz;
    string outputFolder = "../../Output/Lampshade-" + imgSz + "Size/";
    namedWindow(winname, WINDOW_AUTOSIZE);
    imshow(winname, result);
    imwrite(outputFolder + winname + ".png", result);

    //Disparity Calculation at bestMatches
    double rmsDiff = 0, bad = 0;
    Mat disparity_img(imgL.size().height, imgL.size().width, CV_8UC1, Scalar(0));
    vector<DMatch>::iterator it;
    int threshold = 1;
    for (it = bestMatches.begin(); it != bestMatches.end(); ++it) {
        // Get index
        int xL = keypointsL[it->queryIdx].pt.x;
        int yL = keypointsL[it->queryIdx].pt.y;
        int xR = keypointsR[it->trainIdx].pt.x;

        // Calculate disparity & store in image
        int disparity = abs(xL - xR);
        disparity_img.at<uchar>(yL, xL) = disparity;

        // Storing for RMSE calculation
        int imgDisp = imgD.at<uchar>(yL, xL);
        if (imgSz == "Half")
            imgDisp = imgDisp / 2;
        else if (imgSz == "Third")
            imgDisp = imgDisp / 4;
        int disparityDiff = disparity - imgDisp; //Feature descriptor disparity - True disparity
        rmsDiff = rmsDiff + pow(disparityDiff, 2);

        // Storing for Number of bad pixels calculation
        disparityDiff = abs(disparityDiff);
        if (disparityDiff > threshold)
            bad = bad + disparityDiff;
    }

    //Calculating RMSE
    cout << "RMSE = " << sqrt(rmsDiff / bestMatches.size()) << endl;

    //Calculating Number of bad pixels
    cout << "Number of bad pixels = " << bad / bestMatches.size() << endl;

    //Disparity Image
    double minValue, maxValue;
    minMaxLoc(disparity_img, &minValue, &maxValue); //0
    //cout << "MinValue: " << minValue << " | MaxValue: " << maxValue << endl; //46

    double alpha = 255 / (maxValue - minValue); //scale factor, default=1
    double beta = 0;                            //optional beta added to the scaled values

    disparity_img.convertTo(disparity_img, CV_8UC1, alpha, beta);

    string winname1 = descript + "-" + imgSz + "-" + "sparse_disparity_img";
    //namedWindow(winname1, cv::WINDOW_AUTOSIZE);
    //imshow(winname1, disparity_img);
    imwrite(outputFolder + winname1 + ".png", disparity_img);
    
    // Stop timer and calculate elapsed time
    t = getTickCount() - t;
    cout << "Time elapsed: " << t * 1000 / getTickFrequency() << " ms" << "\n" << endl;
}


int main()
{
    string imgSize[3] = { "Full", "Half", "Third" };
    for (int size = 0; size < 3; size++) {

        //Initialize
        string leftImgName = "../../Input/Lampshade-" + imgSize[size] + "Size/view1.png";
        string rightImgName = "../../Input/Lampshade-" + imgSize[size] + "Size/view5.png";
        string leftDispName = "../../Input/Lampshade-" + imgSize[size] + "Size/disp1.png";

        //Read images
        Mat leftImg = imread(leftImgName);
        Mat rightImg = imread(rightImgName);
        Mat leftDisp = imread(leftDispName);

        descriptor(leftImg, rightImg, leftDisp, "BRISK", imgSize[size]);
        descriptor(leftImg, rightImg, leftDisp, "ORB", imgSize[size]);
    }
    waitKey(0);
    destroyAllWindows();
}