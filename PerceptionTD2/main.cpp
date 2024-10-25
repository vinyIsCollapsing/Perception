/* Enter the following commands on the terminal to run:
    g++ main.cpp -o main `pkg-config --cflags --libs opencv4`
    ./main
*/

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

double scale = 0.2;

Mat poly_convol_manual(const Mat &image, const Mat &mask, const Mat &maskY); // Aplly convolution on a BGR image
Mat aplyConvolutionManual(const Mat &channel, const Mat &mask); // Aplly convolution on a single channel
Mat gradientMagnitude(const Mat &gradX, const Mat &gradY); // Compute the gradient magnitude (horizontal and vertical)
Mat makemask(const Mat& image); // Create a mask in a pre-selected zone
Mat erodeMask(const Mat& mask, int erosionSize); // Erode the mask
Mat dilateMask(const Mat& mask, int dilatedSize); // Dilate the mask

int main() {
    
    Mat image = imread("balle.jpg"); // Load the image
    if (image.empty()) {
        cerr << "Error: Image not found!" << endl; // Check if loaded correctly
        return -1;
    }
    resize(image, image, Size(), scale, scale, INTER_LINEAR); // Resize image
    
    // Uncomment the section of the exercise you want to run
    // Exercise 1 et 2
    /*
    // Define Sobel masks for the gradients
    Mat maskX = (Mat_<float>(3, 3) << 
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1);

    Mat maskY = (Mat_<float>(3, 3) << 
        -1, -2, -1,
        0, 0, 0,
        1, 2, 1);
    
    // Find the gradient magnitudes of each channel (BGR) 
    Mat gradientBGR = poly_convol_manual(image, maskX, maskY);

    // Show original and gradient magnitude images
    imshow("Original image", image);
    imshow("Gradient Magnitude (BGR)", gradientBGR);
    waitKey(0);
    */

    // Exercise 3
    /*
    // Define Sobel masks for the gradients
    Mat maskX = (Mat_<float>(3, 3) << 
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1);

    Mat maskY = (Mat_<float>(3, 3) << 
        -1, -2, -1,
        0, 0, 0,
        1, 2, 1);

    // Calculate the horizontal gradient using filter2D
    Mat gradX;
    filter2D(image, gradX, CV_32F, maskX); // -1 means keeping the same depth as the input image

    // Calculate the vertical gradient using filter2D
    Mat gradY;
    filter2D(image, gradY, CV_32F, maskY);

    // Compute the magnitude of the gradient for each pixel
    Mat gradMagnitude;
    magnitude(gradX, gradY, gradMagnitude);

    // Convert back to uchar to better visualization
    gradX.convertTo(gradX, CV_8U);
    gradY.convertTo(gradY, CV_8U);
    gradMagnitude.convertTo(gradMagnitude, CV_8U);

    // Display the original image, horizontal gradient, vertical gradient, and gradient magnitude
    imshow("Original Image", image);
    imshow("Horizontal Gradient (Sobel X)", gradX);
    imshow("Vertical Gradient (Sobel Y)", gradY);
    imshow("Gradient Magnitude", gradMagnitude);
    waitKey(0);
    */

    // Exercise 4
    /*
    // Define a 5x5 averaging mask
    Mat averagingFilter = Mat::ones(5, 5, CV_32F) / 25.0; // Each element is 1/25 for averaging

    // Apply the averaging filter using filter2D
    Mat result;
    filter2D(image, result, -1, averagingFilter);  // -1 means the output image will have the same depth as the input

    // Display the original and filtered images
    imshow("Original Image", image);
    imshow("Averaging Filter Result (5x5)", result);

    // Wait for a key press indefinitely
    waitKey(0);
    */

    //Exercise 5
    /*
    //Create a mask in a pre-selected zone
    Mat mask = makemask(image);

    //Erode the mask
    Mat eroded_mask = erodeMask(mask, 2);

    //Dilate the mask
    Mat dilated_mask = dilateMask(mask, 2);

    Mat imageEroded = image.clone();
    Mat imageDilated = image.clone();
    Mat result = image.clone();

    // Apply the masks and change the pixels with orange and yellow tons to red 
    result.setTo(Scalar(0, 0, 255), mask);
    imageEroded.setTo(Scalar(0, 0, 255), eroded_mask);
    imageDilated.setTo(Scalar(0, 0, 255), dilated_mask);

    //Show the original, eroded and dilated images
    namedWindow("Original Image", WINDOW_NORMAL);
    imshow("Original Image", result);
    namedWindow("Eroded Image", WINDOW_NORMAL);
    imshow("Eroded Image", imageEroded); 
    namedWindow("Dilated Image", WINDOW_NORMAL);
    imshow("Dilated Image", imageDilated); 
    waitKey(0);
    */

    // Exercise 6
    /*
    // Create a mask in a pre-selected zone
    Mat mask = makemask(image);

    // Define structuring element
    int kernelSize = 3;
    Mat structuralElement = getStructuringElement(MORPH_RECT, Size(2 * kernelSize + 1,2 * kernelSize + 1));

    // Apply Opening
    Mat openedMask;
    morphologyEx(mask, openedMask, MORPH_OPEN, structuralElement);

    // Aplly Closing
    Mat closedMask;
    morphologyEx(mask, closedMask, MORPH_CLOSE, structuralElement);
    
    Mat imageOpened = image.clone();
    Mat imageClosed = image.clone();
    Mat result = image.clone();

    // Apply the masks and change the pixels with orange and yellow tons to red 
    result.setTo(Scalar(0, 0, 255), mask);
    imageOpened.setTo(Scalar(0, 0, 255), openedMask);
    imageClosed.setTo(Scalar(0, 0, 255), closedMask);

    //Show the original, Opened and Closed images
    namedWindow("Original Image", WINDOW_NORMAL);
    imshow("Original Image", result);
    namedWindow("Opened Image", WINDOW_NORMAL);
    imshow("Opened Image", imageOpened); 
    namedWindow("Closed Image", WINDOW_NORMAL);
    imshow("Closed Image", imageClosed); 
    */
    waitKey(0);
   
    return 0;
}


//Aplly convolution on a BGR image
Mat poly_convol_manual(const Mat &image, const Mat &maskX, const Mat &maskY) {
  
    //Split the image into 3 channels
    vector<Mat> bgrChannels(3);
    split(image, bgrChannels);

    vector<Mat> gradX(3), gradY(3), gradMagnitude(3);

    //Aplly convolution (X and Y) to each channel
    for (int i = 0; i < 3; i++) {
        gradX[i] = aplyConvolutionManual(bgrChannels[i], maskX);
        gradY[i] = aplyConvolutionManual(bgrChannels[i], maskY);
        gradMagnitude[i] = gradientMagnitude(gradX[i], gradY[i]);
    }

    //Merge the gradients magnitudes back into a BGR image
    Mat gradientBGR;
    merge(gradMagnitude, gradientBGR);

    return gradientBGR;
}

// Aplly convolution on a single channel
Mat aplyConvolutionManual(const Mat &channel, const Mat &mask) {
    int maskHeight = mask.rows;
    int maskWidth = mask.cols;
    int padH = maskHeight / 2;
    int padW = maskWidth / 2;

    //Create the output image with the same size as the input
    Mat output = Mat::zeros(channel.size(), CV_32F); //Using float for more precision

    //Iterate over the image excluding the borders
    for (int i = padH; i < channel.rows - padH; i++) {
        for (int j = padW; j < channel.cols - padW; j++) {
            float sum = 0.0;
            //Apply the mask on the current pixel neighborhood
            for (int m = -padH; m <= padH; m++) {
                for (int n = -padW; n <= padW; n++) {
                    sum += channel.at<uchar>(i + m, j + n) * mask.at<float>(padH + m, padW + n);
                }
            }
            output.at<float>(i, j) = sum; //Assign the result to the output matrix
        }
    }
    output.convertTo(output, CV_8U); //convert back to uchar
    return output;
}

//Compute the gradient magnitude (horizontal and vertical)
Mat gradientMagnitude(const Mat &gradX, const Mat &gradY) {
    Mat magnitude = Mat::zeros(gradX.size(), CV_32F);

    for (int i = 0; i < gradX.rows; i++) {
        for (int j = 0; j < gradX.cols; j++)  {
           float gx = gradX.at<uchar>(i, j);
           float gy = gradY.at<uchar>(i, j);
           magnitude.at<float>(i, j) = sqrt(gx * gx + gy * gy);
        }
    }
    
    magnitude.convertTo(magnitude, CV_8U); //convert back to uchar
    return magnitude;
}

//Create a mask in a pre-selected zoned
Mat makemask(const Mat& image) {
    Scalar bottom_limit(0, 120, 140);  // Bottom limit for Hue, Saturation and Value
    Scalar upper_limit(30, 255, 255); // Upper limit

    Mat image_hsv, mask;
    
    // Convert the image to the selected colorspace
    cvtColor(image, image_hsv, COLOR_BGR2HSV);

    // Create a mask with the color limits selected
    inRange(image_hsv, bottom_limit, upper_limit, mask);

    return mask;
}

//Create eroded mask
Mat erodeMask(const Mat& mask, int erosionSize) {
    Mat eroded_mask; 
    Mat erosionElement = getStructuringElement(MORPH_RECT, Size(2* erosionSize + 1, 2 * erosionSize + 1));
    erode(mask, eroded_mask, erosionElement);

    return eroded_mask;
}

//Create dilated mask
Mat dilateMask(const Mat& mask, int dilatedSize) {
    Mat dilated_mask; 
    Mat dilatedElement = getStructuringElement(MORPH_RECT, Size(2* dilatedSize + 1, 2 * dilatedSize + 1));
    dilate(mask, dilated_mask, dilatedElement);

    return dilated_mask;
}