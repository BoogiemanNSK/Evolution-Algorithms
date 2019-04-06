#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <Windows.h>
#include "src/Genetic.h"
#include "src/Chromosome.h"
using namespace cv;
using namespace std;

#define IMAGE_PATH "D:\\VS17Projects\\Image-GA\\Image_GA\\input\\pirate.jpg"

/// HELPER FUNCTIONS ///

void ShowInitialImage(Mat image) {
	if (image.empty()) {
		cout << "Could not open or find the image" << endl;
		return;
	}

	String windowName = "Before GA";
	namedWindow(windowName);
	imshow(windowName, image);
	waitKey(0);
	destroyWindow(windowName);
}

void GenerateOutput(Mat image, vector<Vec3b> result) {
	Vec3b color;
	for (int i = 0; i < IMAGE_SIDE; i++) {
		for (int j = 0; j < IMAGE_SIDE; j++) {
			color = image.at<Vec3b>(Point(i, j));

			color[0] = result[i * IMAGE_SIDE + j][0];
			color[1] = result[i * IMAGE_SIDE + j][1];
			color[2] = result[i * IMAGE_SIDE + j][2];

			image.at<Vec3b>(Point(i, j)) = color;
		}
	}

	String windowName = "After GA";
	namedWindow(windowName);
	imshow(windowName, image);
	waitKey(0);
	destroyWindow(windowName);
}

/// MAIN ///

int main() {
	Mat image = imread(IMAGE_PATH);

	ShowInitialImage(image);
	Genetic GA(image);
	GA.Evolve();
	GenerateOutput(image, GA.ResultingState);
	
	system("pause");
	return 0;
}