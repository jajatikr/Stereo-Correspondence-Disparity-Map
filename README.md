# Stereo-Correspondence-Disparity-Map
Stereo matching and sparse disparity map implementation using OpenCV (BRISK, ORB algorithms)

Feature descriptors are obtained from images using BRISK and ORB algorithm implemented in OpenCV. The feature descriptors are used for stereo matching between images and generate sparse disparity map.

### How to run?

1. Setup [Visual Studio](https://www.visualstudio.com/) and [OpenCV](https://www.opencv.org/releases.html)
2. Open the project present in [Code](https://github.com/jajatikr/Stereo-Correspondence-Disparity-Map/tree/master/Code) folder and run the source file.
3. The Input and Output folder contains the input and output images.
4. Dataset can be downloaded from http://vision.middlebury.edu/stereo/data/

    My [python script](https://github.com/jajatikr/download-middlebury-stereo-dataset) will be helpful if you want to download the whole dataset.

### Images:

#### Input images (View-1, View-5):

![](https://raw.githubusercontent.com/jajatikr/Stereo-Correspondence-Disparity-Map/master/Input/Lampshade-ThirdSize/view1.png) ![](https://raw.githubusercontent.com/jajatikr/Stereo-Correspondence-Disparity-Map/master/Input/Lampshade-ThirdSize/view5.png)

#### Output images (BRISK, ORB):
![](https://raw.githubusercontent.com/jajatikr/Stereo-Correspondence-Disparity-Map/master/Output/Lampshade-ThirdSize/BRISK-Third.png) ![](https://raw.githubusercontent.com/jajatikr/Stereo-Correspondence-Disparity-Map/master/Output/Lampshade-ThirdSize/ORB-Third.png)

#### Sparse Disparity Map:
![](https://raw.githubusercontent.com/jajatikr/Stereo-Correspondence-Disparity-Map/master/Output/Lampshade-ThirdSize/BRISK-Third-sparse_disparity_img.png) ![](https://raw.githubusercontent.com/jajatikr/Stereo-Correspondence-Disparity-Map/master/Output/Lampshade-ThirdSize/ORB-Third-sparse_disparity_img.png)
