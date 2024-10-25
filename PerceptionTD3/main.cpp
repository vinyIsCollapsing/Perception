#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace cv;
using namespace std;

// Define constants
#define DEG2RAD 0.01745329251  // Degree to radian conversion factor

Mat applySobel(const Mat& image);
Mat applyScharr(const Mat& image);
Mat loadImage(const string& path);
Mat applyCanny(const Mat& src, int low_threshold, int high_threshold);
vector<pair<int, int>> manualHoughTransform(const Mat& edges, int threshold, int& max_dist, int& num_thetas, double& theta_step);
Mat drawManualLines(const Mat& edges, const vector<pair<int, int>>& lines, int max_dist, int num_thetas, double theta_step);
vector<Vec2f> applyHoughTransform(const Mat& edges, double rho_resolution, double theta_resolution, int threshold);
Mat drawLines(const Mat& edges, const vector<Vec2f>& lines);
Mat colorYellowAndOrangeTones(const Mat& image, const Scalar& lower_bound, const Scalar& upper_bound);
void detectYellowBall(Mat image);

VideoCapture loadVideo(const string& videoPath);
Mat getYellowMask(const Mat& frame);
vector<Vec3f> detectCircles(const Mat& mask);
void drawCircles(Mat& frame, const vector<Vec3f>& circles);
void processVideo(VideoCapture& cap);

int main() {
    /*
    // Exercise 1
    // Load the image
    string path = "escalier.png";
    Mat image = loadImage(path);
    if (image.empty()) return -1;

    // Apply Sobel and Scharr
    Mat sobelEdges = applySobel(image);
    Mat scharrEdges = applyScharr(image);

    // Display the results
    imshow("Original Image", image);
    imshow("Sobel Edges", sobelEdges);
    imshow("Scharr Edges", scharrEdges);

    // Wait for close key
    waitKey(0);
    */
    /*
    // Exercise 3
    string path = "escalierWithoutTree.png";
    Mat src = loadImage(path);
    if (src.empty()) return -1;

    // Apply Canny edge detection
    Mat edges = applyCanny(src, 100, 200);

    // Execute manual Hough Transform
    int max_dist, num_thetas;
    double theta_step;
    vector<pair<int, int>> lines = manualHoughTransform(edges, 200, max_dist, num_thetas, theta_step);

    // Draw detected lines
    Mat dst = drawManualLines(edges, lines, max_dist, num_thetas, theta_step);

    // Display original image and image with detected lines
    imshow("Original Image", src);
    imshow("Manual Canny Image", edges);
    imshow("Manual Detected Lines", dst);

    waitKey(0);
    */
    // Exercise 4
    // Load the image
    /*
    string path = "escalierWithoutTree.png";
    Mat src = loadImage(path);
    if (src.empty()) return -1;

    // Apply edge detection using Canny
    Mat edges = applyCanny(src, 100, 200);

    // Apply Hough Transform for line detection
    vector<Vec2f> lines = applyHoughTransform(edges, 1, CV_PI / 180, 200);

    // Draw detected lines
    Mat dst = drawLines(edges, lines);

    // Display original image and image with detected lines
    imshow("Original Image", src);
    imshow("Canny Image", edges);
    imshow("Detected Lines", dst);

    waitKey(0);
    */

    // Exercise 5
    // Load the image
    /* 
    Mat image = imread("balle.jpg");

    if(image.empty()) {
        cout << "Error loading image!" << endl;
        return -1;
    }

    // Detect the yellow ball
    detectYellowBall(image);
    */


    /*
    // Exercise 6
    VideoCapture cap = loadVideo("balle.mp4");

    // Process the video to detect the yellow ball
    processVideo(cap);

    cap.release();
    destroyAllWindows();
    */
    return 0;
    
}

// Exercise 1
// Function to apply the Sobel operator
Mat applySobel(const Mat& image) {
    Mat sobelX, sobelY, sobelResult;

    // Sobel in X and Y directions
    Sobel(image, sobelX, CV_64F, 1, 0, 3); // X-direction
    Sobel(image, sobelY, CV_64F, 0, 1, 3); // Y-direction

    // Compute gradient magnitude
    magnitude(sobelX, sobelY, sobelResult);
    sobelResult.convertTo(sobelResult, CV_8U);

    return sobelResult;
}

// Function to apply the Scharr operator
Mat applyScharr(const Mat& image) {
    Mat scharrX, scharrY, scharrResult;

    // Scharr in X and Y directions
    Scharr(image, scharrX, CV_64F, 1, 0); // X-direction
    Scharr(image, scharrY, CV_64F, 0, 1); // Y-direction

    // Compute gradient magnitude
    magnitude(scharrX, scharrY, scharrResult);
    scharrResult.convertTo(scharrResult, CV_8U);

    return scharrResult;
}

// Generic function
// Function to load the image
Mat loadImage(const string& path) {
    Mat src = imread(path, IMREAD_GRAYSCALE);
    if (src.empty()) {
        cout << "Error loading image!" << endl;
    }
    return src;
}

// Exercise 3
// Function to apply Canny edge detection
Mat applyCanny(const Mat& src, int low_threshold, int high_threshold) {
    Mat edges;
    Canny(src, edges, low_threshold, high_threshold, 3);
    return edges;
}

// Function to perform manual Hough Transform
vector<pair<int, int>> manualHoughTransform(const Mat& edges, int threshold, int& max_dist, int& num_thetas, double& theta_step) {
    int width = edges.cols;
    int height = edges.rows;
    max_dist = sqrt(width * width + height * height); // Maximum rho
    num_thetas = 180; // Number of angles
    int num_rhos = 2 * max_dist; // Number of rho values
    theta_step = CV_PI / num_thetas;

    // Create the accumulator
    vector<vector<int>> accumulator(num_rhos, vector<int>(num_thetas, 0));

    // Populate the accumulator
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (edges.at<uchar>(y, x) > 0) { // Edge point detection
                for (int t = 0; t < num_thetas; t++) {
                    double theta = t * theta_step;
                    double rho = x * cos(theta) + y * sin(theta);
                    int rho_idx = cvRound(rho + max_dist); // Rho index in the accumulator
                    accumulator[rho_idx][t]++;
                }
            }
        }
    }

    // Identify peaks in the accumulator
    vector<pair<int, int>> lines; // Stores pairs of (rho, theta)

    for (int r = 0; r < num_rhos; r++) {
        for (int t = 0; t < num_thetas; t++) {
            if (accumulator[r][t] > threshold) {
                lines.push_back({r, t});
            }
        }
    }

    return lines;
}

// Function to draw detected lines
Mat drawManualLines(const Mat& edges, const vector<pair<int, int>>& lines, int max_dist, int num_thetas, double theta_step) {
    Mat dst;
    cvtColor(edges, dst, COLOR_GRAY2BGR);

    for (size_t i = 0; i < lines.size(); i++) {
        int r = lines[i].first;
        int t = lines[i].second;

        double rho = r - max_dist;
        double theta = t * theta_step;

        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        pt1.x = cvRound(x0 + 1000 * (-b));
        pt1.y = cvRound(y0 + 1000 * (a));
        pt2.x = cvRound(x0 - 1000 * (-b));
        pt2.y = cvRound(y0 - 1000 * (a));
        line(dst, pt1, pt2, Scalar(0, 0, 255), 2, LINE_AA);
    }

    return dst;
}

// Exercise 4
// Function to apply Hough Transform
vector<Vec2f> applyHoughTransform(const Mat& edges, double rho_resolution, double theta_resolution, int threshold) {
    vector<Vec2f> lines;
    HoughLines(edges, lines, rho_resolution, theta_resolution, threshold, 350, 50);
    return lines;
}

// Function to draw detected lines
Mat drawLines(const Mat& edges, const vector<Vec2f>& lines) {
    Mat dst;
    cvtColor(edges, dst, COLOR_GRAY2BGR);

    for (size_t i = 0; i < lines.size(); i++) {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * sin(theta);
        pt1.x = cvRound(x0 + 1000 * (-b));
        pt1.y = cvRound(y0 + 1000 * (a));
        pt2.x = cvRound(x0 - 1000 * (-b));
        pt2.y = cvRound(y0 - 1000 * (a));
        line(dst, pt1, pt2, Scalar(0, 0, 255), 2, LINE_AA);
    }

    return dst;
}

// Exercise 5
// Function to segment the yellow ball
Mat segmentYellowBall(const Mat& image) {
    Mat image_hsv, mask;
    
    // Convert the image to HSV color space
    cvtColor(image, image_hsv, COLOR_BGR2HSV);

    // Color bounds for yellow ball (adjust as needed)
    Scalar lower_bound(20, 100, 100);  // Darker yellow
    Scalar upper_bound(30, 255, 255);  // Lighter yellow

    // Create a mask to isolate the yellow ball
    inRange(image_hsv, lower_bound, upper_bound, mask);

    return mask;
}

// Function to detect yellow ball using Hough Transform
void detectYellowBall(Mat image) {
    // Segment the yellow ball
    Mat mask = segmentYellowBall(image);

    // Apply blur to smooth the image and reduce noise
    GaussianBlur(mask, mask, Size(9, 9), 3, 3);

    // Detect circles using Hough Transform
    vector<Vec3f> circles;
    HoughCircles(mask, circles, HOUGH_GRADIENT, 1, mask.rows/8, 5, 40, 50, 750); // Parameter adjustments for higher sensitivity

    // Check if any circles were detected
    if (circles.size() == 0) {
        cout << "No circles detected!" << endl;
        return;
    }

    // Draw detected circles
    for(size_t i = 0; i < circles.size(); i++) {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);

        // Draw ball center
        circle(image, center, 3, Scalar(0, 255, 0), -1, 8, 0);

        // Draw ball outline
        circle(image, center, radius, Scalar(255, 0, 0), 5, 8, 0);  // Thicker outline
    }

    // Resize image to 640x480
    Mat resized_image;
    resize(image, resized_image, Size(640, 480));

    // Create a resizable window to display result
    namedWindow("Yellow Ball Detection", WINDOW_NORMAL);
    imshow("Yellow Ball Detection", resized_image);
    waitKey(0);
}

// Exercise 6
// Function to load the video
VideoCapture loadVideo(const string& videoPath) {
    VideoCapture cap(videoPath);
    if (!cap.isOpened()) {
        cerr << "Error opening video!" << endl;
        exit(-1);
    }
    return cap;
}

// Function to apply a mask for yellow color (now adjusted for varied tones)
Mat getYellowMask(const Mat& frame) {
    Mat hsvFrame, mask;
    
    // Color range adjusted to capture varied yellow and orange tones
    Scalar lower_yellow_orange(15, 80, 100);    // Broader lower limit
    Scalar upper_yellow_orange(30, 255, 255);  // Broader upper limit

    // Convert to HSV color space
    cvtColor(frame, hsvFrame, COLOR_BGR2HSV);

    // Create mask to detect yellow and orange tones
    inRange(hsvFrame, lower_yellow_orange, upper_yellow_orange, mask);

    // Apply blur to reduce noise and morphological operation for more precise contours
    GaussianBlur(mask, mask, Size(7, 7), 2, 2);
    morphologyEx(mask, mask, MORPH_CLOSE, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

    return mask;
}

// Function to detect circles using Hough Transform
vector<Vec3f> detectCircles(const Mat& mask) {
    vector<Vec3f> circles;
    HoughCircles(mask, circles, HOUGH_GRADIENT, 1, mask.rows / 8, 100, 20, 10, 100);
    return circles;
}

// Function to draw detected circles on the frame
void drawCircles(Mat& frame, const vector<Vec3f>& circles) {
    for (size_t i = 0; i < circles.size(); i++) {
        Vec3f c = circles[i];
        Point center(cvRound(c[0]), cvRound(c[1]));
        int radius = cvRound(c[2]);

        // Draw circle center
        circle(frame, center, 3, Scalar(0, 255, 0), -1, LINE_AA);
        // Draw circle border
        circle(frame, center, radius, Scalar(0, 0, 255), 3, LINE_AA);
    }
}

// Main function to process the video
void processVideo(VideoCapture& cap) {
    while (true) {
        Mat frame;
        cap >> frame;
        if (frame.empty()) break;

        // Get mask for yellow color
        Mat mask = getYellowMask(frame);

        // Detect circles in mask
        vector<Vec3f> circles = detectCircles(mask);

        // Draw detected circles
        drawCircles(frame, circles);

        // Display result
        imshow("Yellow Ball Detection", frame);

        // Stop on 'q' key press
        if (waitKey(30) == 'q') break;
    }
}
