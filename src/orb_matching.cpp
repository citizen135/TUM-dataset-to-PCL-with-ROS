#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    Mat trg = imread("/home/pjh/data/rgbd_dataset_freiburg2_desk/rgb/1311868164.363181.png", IMREAD_GRAYSCALE);
    Mat src = imread("/home/pjh/data/rgbd_dataset_freiburg2_desk/rgb/1311868164.863058.png", IMREAD_GRAYSCALE);

    if (trg.empty() || src.empty()) {
        cerr << "Image load failed!" << endl;

        return -1;
    }
    Ptr<Feature2D> feature = ORB::create();

    vector<KeyPoint> trgKeypoints, srcKeypoints;
    Mat trgDesc, srcDesc;
    feature->detectAndCompute(trg, Mat(), trgKeypoints, trgDesc);
    feature->detectAndCompute(src, Mat(), srcKeypoints, srcDesc);
    // Detects keypoints and computes the descriptors.
    
    Ptr<DescriptorMatcher> matcher = BFMatcher::create(NORM_HAMMING);
    // Brute-force matcher create method.
    
    vector<DMatch> matches;
    // Class for matching keypoint descriptors.
    matcher->match(trgDesc, srcDesc, matches);
    // Finds the best match for each descriptor from a query set.
    
    sort(matches.begin(), matches.end());
    vector<DMatch> good_matches(matches.begin(), matches.begin() + 100);
    
    for (size_t i = 0; i < good_matches.size(); ++i){
        cout<<"\n cnt: "<<i<<endl;
        cout<<good_matches[i].distance<<endl;
        cout<<good_matches[i].imgIdx<<endl;
        cout<<good_matches[i].queryIdx<<endl;
        cout<<good_matches[i].trainIdx<<endl;
        cout<<trgKeypoints[good_matches[i].trainIdx].pt<<endl;
    }

    

    Mat dst;
    drawMatches(trg, trgKeypoints, src, srcKeypoints, good_matches, dst,
        Scalar::all(-1), Scalar(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    // Draws the found matches of keypoints from two images.
    
    imshow("dst", dst);
    waitKey();
    
    return 0;
}