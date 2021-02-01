#include <opencv2/opencv.hpp>
#include <thread>


cv::VideoCapture vcap;

int main(int argc, const char * argv [ ])
{
	const std::string videoStreamAddress = "rtsp://admin:wavecamera1@10.0.66.201:554";

	if(!vcap.open(videoStreamAddress)) {
		std::cout << "Error opening video stream or file\n";
		return -1;
	}

	for(; ; ) {
		cv::Mat image;
		if(!vcap.read(image)) {
			std::cout << ":----> No frame\n";
			cv::waitKey();
		}
		cv::Mat image1;
		if(!vcap.read(image1)) {

		}
		cv::imshow("OutPut Window", image);
		cv::imshow("Output window", image1);
		if(cv::waitKey(1) >= 0) break;
	}
}

