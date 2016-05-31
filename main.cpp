#include "deHaze.h"
#include <ctime>

int main() {
	DeHaze dhModel;
	clock_t start = clock();
	//dhModel.loadImage("../testSet/tiananmen.png");  
	dhModel.loadImage("../testSet/tree.bmp");
	//dhModel.loadImage("../testSet/city.png");
	//dhModel.loadImage("../testSet/toys.jpg");
	dhModel.showImage("src", DeHaze::SRC);

	dhModel.getDarkChannelPrior();
	//dhModel.("dark", DeHaze::DARK);
	dhModel.getAtmosphericLight();
	dhModel.getTransmission();
	//dhModel.showImage("tran", DeHaze::TRAN);
	dhModel.gFilter();
	//dhModel.showImage("", DeHaze::GSRC);

	dhModel.recoverSceneRadiace();
	dhModel.showImage("dst", DeHaze::DST);
	clock_t end = clock();
	cout << "Time consumed : " << (float)(end - start) / CLOCKS_PER_SEC << "s" << endl;
	cin.get();
	return 0;
}