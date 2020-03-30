#include "../headers/Image.h"

int nearestInterpolation(GrayscaleImage& source, const float xScale, const float yScale, const std::string& output){
    GrayscaleImage target(source.GetWidth()*xScale, source.GetHeight()*yScale);
    const float xScaleFactor = (float)source.GetWidth()/target.GetWidth(), yScaleFactor = (double)source.GetHeight()/target.GetHeight();
	for(int y = 0; y<target.GetHeight(); y++)
		for(int x = 0; x<target.GetWidth(); x++)
			target(x,y) = source(x*xScaleFactor, y*yScaleFactor);
	target.Save(output);
	return 1;
}

int main() {
	float xScale = 2, yScale = 2;
	GrayscaleImage source;
	source.Load("../../images/read.png");
	if(nearestInterpolation(source, xScale, yScale, "../../output/nnOutput.png"))
        std::cout<<"Image Resized to " << source.GetWidth()*xScale <<"x" << source.GetHeight()*yScale << std::endl;
    else std::cout<<"Image could not be resized."<< std::endl;	
	return 0;
}