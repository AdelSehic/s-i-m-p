#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

void expand_img_to_optimal(cv::Mat& padded,cv::Mat& img) {
	int row = cv::getOptimalDFTSize(img.rows);
	int col = cv::getOptimalDFTSize(img.cols);
	cv::copyMakeBorder(img,padded,0,row - img.rows,0,col - img.cols,cv::BORDER_CONSTANT,cv::Scalar::all(0));
}

void crop_and_rearrange(cv::Mat& magI)
{
	  magI = magI(cv::Rect(0, 0, magI.cols & -2, magI.rows & -2));
    int cx = magI.cols/2;
    int cy = magI.rows/2;
		cv::Mat q0(magI, cv::Rect(0, 0, cx, cy));
		cv::Mat q1(magI, cv::Rect(cx, 0, cx, cy));  // Top-Right
    cv::Mat q2(magI, cv::Rect(0, cy, cx, cy));  // Bottom-Left
    cv::Mat q3(magI, cv::Rect(cx, cy, cx, cy)); // Bottom-Right
    cv::Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);

}

cv::Mat fourier_transform(cv::Mat& img) {
	cv::Mat padded;
	expand_img_to_optimal(padded,img);

	// Since the result of Fourier Transformation is in complex form we make two planes to hold  real and imaginary value
	cv::Mat planes[] = {cv::Mat_<float>(padded),cv::Mat::zeros(padded.size(),CV_32F)};
	cv::Mat complexI;
	cv::merge(planes,2,complexI);

	cv::dft(complexI,complexI,cv::DFT_COMPLEX_OUTPUT); // Fourier Transform

	cv::split(complexI,planes);
	cv::magnitude(planes[0],planes[1],planes[0]);
	cv::Mat magI = planes[0];

	magI += cv::Scalar::all(1);
	cv::log(magI,magI);

	crop_and_rearrange(magI);

	cv::normalize(magI, magI, 0, 1, cv::NORM_MINMAX); // for visualization purposes
	return magI;
}

