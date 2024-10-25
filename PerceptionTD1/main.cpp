// g++ main.cpp -o main `pkg-config --cflags --libs opencv4`
// ./main
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <fstream>
 
using namespace cv;
using namespace std;

Mat capturedImage;  // Original image cloned for processing
Mat roi;            // Region of interest
Scalar meanColor;   // Stores the mean color of the ROI
bool regionSelected = false;  // Control variable to check if the ROI was selected

void mouseCallback(int event, int x, int y, int flags, void *userdata);
Mat convetToMonochrome(Mat &originalImage);
Mat convetToMonochromeManual(Mat &originalImage);
void mouseClick(int event, int x, int y, int flags, void *param);
void calculateMeanStdDev(Mat &roi);
void colorSimilarPixels(Mat &image, Scalar meanColor, double threshold);

void selectROI(Mat &image);
Scalar calculateMeanColor(const Mat &roi);
void colorPixelsSimilarToMean(Mat &image, Scalar meanColor, double threshold);
Mat colorYellowAndOrangeShades(const Mat& image, const Scalar& lower_limit, const Scalar& upper_limit);
Point calculateCenterOfGravity(const Mat& mask);
void detectAndMarkBall(Mat& frame, const Scalar& lower_limit, const Scalar& upper_limit);

int main(){
    // Opening image
    string image_path = samples::findFile("balle.jpg");
    Mat image = imread(image_path, IMREAD_COLOR);

    if(image.empty()){
        cout << "Could not read the image: " << image_path << endl;
        return 1;
    }

    //Exercice 1
    /*
    namedWindow("Full Image", WINDOW_NORMAL);
    setMouseCallback("Full Image", mouseCallback, &image);
    imshow("Full Image", image);
    waitKey(0);
    
    */

    //Exercice 2
    /*
    Mat imageMonochrome = convetToMonochrome(image);
    namedWindow("Automatic Monochromatic Image", WINDOW_NORMAL);
    imshow("Automatic Monochromatic Image", imageMonochrome);

    Mat imageMonochromeManual = convetToMonochromeManual(image);
    namedWindow("Manual Monochromatic Image", WINDOW_NORMAL);
    imshow("Manual Monochromatic Image", imageMonochromeManual);
    waitKey(0);
    */
    

    // Exercice 3
    /*
    capturedImage = image.clone();

    namedWindow("Clickable Image", WINDOW_NORMAL);
    setMouseCallback("Clickable Image", mouseClick, &roi);
    imshow("Clickable Image", image);
 
    // Loop to keep the window open
    while (true) {
        int k = waitKey(1); 
        if (k == 27){ // Press ESC to exit
            break;
        }
    }

    if (!roi.empty()) {
        calculateMeanStdDev(roi);
        roi.release();
    }
    */

    // Exercice 4
    /* 
    // Cloning the original image to be manipulated
    capturedImage = image.clone();

    // Initial display of the image
    namedWindow("Original Image", WINDOW_NORMAL);
    imshow("Original Image", capturedImage);

    // Function to select an area (ROI) in the image
    selectROI(capturedImage);

    // Checks if the ROI was correctly selected
    if (!roi.empty()) {
        // Calculates the average color of the ROI
        meanColor = calculateMeanColor(roi);

        // Colors the pixels of the image that have a color similar to the ROI mean
        colorPixelsSimilarToMean(capturedImage, meanColor, 60.0); // Uses 30 as similarity threshold

        // Displays the modified image
        namedWindow("Modified Image", WINDOW_NORMAL);
        imshow("Modified Image", capturedImage);
        waitKey(0); // Waits until the user presses a key
    }
    */

    /*
    // Exercice 5
    // Defines the color limits for selection in HSV space
    Scalar lower_limit(0, 120, 140);  // Lower limit for Hue, Saturation, and Value
    Scalar upper_limit(30, 255, 255); // Upper limit

    // Calls the segmentation function
    Mat result = colorYellowAndOrangeShades(image, lower_limit, upper_limit);

    // Displays the original image and the segmentation result
    namedWindow("Original Image", WINDOW_NORMAL);
    imshow("Original Image", image);

    namedWindow("HSV Image", WINDOW_NORMAL);
    imshow("HSV Image", result);

    waitKey(0);
    */
    
    //Exercice 6
    /*
    // Loads the video
    VideoCapture video("balle.mp4");
    if (!video.isOpened()) {
        cerr << "Error loading the video!" << endl;
        return -1;
    }

    // Defines the color limits for the ball in HSV space
    Scalar lower_limit(20, 100, 100);   // Lower limit for yellow shades
    Scalar upper_limit(30, 255, 255);   // Upper limit for brighter yellow tones

    // Loop to process each video frame
    while (true) {
        Mat frame;
        video >> frame;  // Reads a new frame from the video
        if (frame.empty()) break;  // Ends the loop at the end of the video

        // Detects and marks the ball in the frame
        detectAndMarkBall(frame, lower_limit, upper_limit);

        // Displays the frame with the blue "X" and the ball colored red
        imshow("Tracking the Ball", frame);

        // Press 'q' to exit the loop
        if (waitKey(30) == 'q') break;
    }

    video.release();
    destroyAllWindows();
    */
    return 0;
}

// Exercice 1
// Mouse event capture
// Capture coordinates and RGB values
void mouseCallback(int event, int x, int y, int flags, void *userdata){
    if (event == EVENT_LBUTTONDOWN){
        Mat* image = static_cast<Mat*>(userdata);
        Vec3b pixel = image->at<Vec3b>(y,x);
        cout << "Coordinates (x, y): (" << x << ", " << y << ")" << endl;
        cout << "Color values - R: " << (int)pixel[2] <<
                                " G: " << (int)pixel[1] <<
                                " B: " << (int)pixel[0] << endl;
    }
}

// Exercice 2.1 (with OpenCV)
// Automatic color conversion
Mat convetToMonochrome(Mat &originalImage){
    Mat monoImage; 
    cvtColor(originalImage, monoImage, COLOR_BGR2GRAY);
    return monoImage;
}

// Exercice 2.2 (without OpenCV)
// Manual color conversion
Mat convetToMonochromeManual(Mat &originalImage){
    int width = originalImage.cols;
    int height = originalImage.rows;

    // CV_8UC1: 8-bit unsigned matrix type
    Mat monoImage(height, width, CV_8UC1);

    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            Vec3b pixel = originalImage.at<Vec3b>(y, x);
            // Gray intensity
            uchar grayValue = static_cast<uchar>(0.299 * pixel[2] + 0.587 * pixel[1] + 0.114 * pixel[0]);
            monoImage.at<uchar>(y, x) = grayValue;
        }
    }
    
    return monoImage;
}

// Exercice 3
// Function to capture an area of the image with the mouse
void mouseClick(int event, int x, int y, int flags, void *param) {
    static Point pt1, pt2;
    static bool dragging = false; // Variable to control mouse dragging

    switch (event) {
        case EVENT_LBUTTONDOWN:
            cout << "Mouse Pressed at (" << x << ", " << y << ")" << endl;
            pt1 = Point(x, y);  // First ROI coordinate
            dragging = true;    // Start capturing the region
            break;

        case EVENT_MOUSEMOVE:
            if (dragging) {
                Mat tempImage = capturedImage.clone();
                pt2 = Point(x, y);
                rectangle(tempImage, pt1, pt2, Scalar(0, 255, 0), 2); // Draws the rectangle during selection
                imshow("Clickable Image", tempImage); // Updates image display as the mouse moves
            }
            break;

        case EVENT_LBUTTONUP:
            cout << "Mouse Released at (" << x << ", " << y << ")" << endl;
            pt2 = Point(x, y); // Second ROI coordinate

            if (dragging) {
                // Captures the region of interest (ROI)
                Rect roiRect(pt1, pt2);
                roi = capturedImage(roiRect).clone(); // Copies the selected area
                imshow("Captured Region", roi); // Shows the selected area in a new window

                regionSelected = true;  // Sets that the region was selected
                dragging = false;       // Stops mouse dragging
            }
            break;

        default:
            break;
    }
}

// Function to calculate the mean and standard deviation of the ROI
void calculateMeanStdDev(Mat &roi) {
    if (roi.empty()) {
        cout << "Empty ROI" << endl;
        return;
    }

    // Calculate the mean of each color channel
    Scalar mean, stddev;
    meanStdDev(roi, mean, stddev); // OpenCV function to calculate mean and standard deviation

    meanColor = mean; // Stores the mean color for use in comparison
    cout << "Mean - R: " << mean[2] << ", G: " << mean[1] << ", B: " << mean[0] << endl;
    cout << "Standard Deviation - R: " << stddev[2] << ", G: " << stddev[1] << ", B: " << stddev[0] << endl;
}

// Exercice 4
// Function to select the ROI with the mouse
void selectROI(Mat &image) {
    // Captures the region of interest (ROI)
    Rect roiRect = selectROI("Original Image", image); // OpenCV function to manually select ROI

    if (roiRect.width > 0 && roiRect.height > 0) {
        roi = image(roiRect).clone(); // Clones the ROI for later use
        imshow("Selected Region", roi); // Displays the selected region in a new window
        waitKey(0); // Waits for the user to view the selected region
    } else {
        cout << "No region was selected." << endl;
    }
}

// Function to calculate the mean color of the ROI
Scalar calculateMeanColor(const Mat &roi) {
    Scalar meanValue, stddevValue;
    meanStdDev(roi, meanValue, stddevValue); // Calculates the mean and standard deviation of the ROI

    cout << "Color mean (R, G, B) - R: " << meanValue[2] << ", G: " << meanValue[1] << ", B: " << meanValue[0] << endl;
    return meanValue; // Returns the mean color of the ROI
}

// Function to color pixels similar to the ROI mean
void colorPixelsSimilarToMean(Mat &image, Scalar meanColor, double threshold) {
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            Vec3b pixel = image.at<Vec3b>(y, x);

            // Checks if the pixel is similar to the ROI mean
            if (abs(pixel[0] - meanColor[0]) < threshold && 
                abs(pixel[1] - meanColor[1]) < threshold && 
                abs(pixel[2] - meanColor[2]) < threshold) {
                // Colors the pixel red
                image.at<Vec3b>(y, x) = Vec3b(0, 0, 255); // RGB for red
            }
        }
    }
}

// Exercice 5
// Function to segment a specific area of the image based on HSV values
Mat colorYellowAndOrangeShades(const Mat& image, const Scalar& lower_limit, const Scalar& upper_limit) {
    Mat image_hsv, mask, result;
    
    // Converts the image to HSV color space
    cvtColor(image, image_hsv, COLOR_BGR2HSV);

    // Creates the mask with the defined color limits
    inRange(image_hsv, lower_limit, upper_limit, mask);

    // Copies the original image to the result variable
    result = image.clone();

    // Changes the pixels corresponding to yellow and orange shades to red
    result.setTo(Scalar(0, 0, 255), mask);  // (0, 0, 255) is red in BGR

    return result;
}

// Exercice 6
Point calculateCenterOfGravity(const Mat& mask) {
    Moments mu = moments(mask);
    if (mu.m00 != 0) {
        int cx = static_cast<int>(mu.m10 / mu.m00);
        int cy = static_cast<int>(mu.m01 / mu.m00);
        return Point(cx, cy);
    }
    return Point(-1, -1); 
}

// Function to detect and mark the center of the ball in a frame
void detectAndMarkBall(Mat& frame, const Scalar& lower_limit, const Scalar& upper_limit) {
    Mat frame_hsv, mask;
    
    // Converts the frame to HSV space
    cvtColor(frame, frame_hsv, COLOR_BGR2HSV);

    // Creates the mask to detect the ball based on color limits
    inRange(frame_hsv, lower_limit, upper_limit, mask);

    // Calculates the center of gravity of the mask
    Point center = calculateCenterOfGravity(mask);

    // Colors the ball red using the mask
    frame.setTo(Scalar(0, 0, 255), mask);  // Sets the ball area to red

    // If the center is valid, draws a blue "X" at the ball center
    if (center.x != -1 && center.y != -1) {
        int size = 10;  // Size of the "X" lines
        
        // Diagonal line from top-left to bottom-right
        line(frame, Point(center.x - size, center.y - size), Point(center.x + size, center.y + size), Scalar(255, 0, 0), 2);
        // Diagonal line from top-right to bottom-left
        line(frame, Point(center.x + size, center.y - size), Point(center.x - size, center.y + size), Scalar(255, 0, 0), 2);
    }
}
