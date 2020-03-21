#include "../headers/Image.h"

int main() {
	
	GrayscaleImage img, target(1000,1000);
	img.Load("../../images/read.png");
	float scaleFactor = (float)img.GetWidth() / target.GetWidth();
	for(int y = 0; y<target.GetHeight(); y++)
		for(int x = 0; x<target.GetWidth(); x++)
			target(x,y) = img(x*scaleFactor, y*scaleFactor);
	target.Save("../../output/nnOutput.png");
	
	return 0;
}