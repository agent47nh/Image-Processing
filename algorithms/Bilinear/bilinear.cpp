#include "../headers/Image.h"
#include <math.h>

int bilinearInterpolation(GrayscaleImage& source, const float xScale, const float yScale, const std::string& output){
    GrayscaleImage target(source.GetWidth()*xScale, source.GetHeight()*yScale);
    const double xScaleFactor = (double)source.GetWidth()/target.GetWidth(), yScaleFactor = (double)source.GetHeight()/target.GetHeight();
    for(int y=0; y<target.GetHeight(); y++)
        for(int x=0; x<target.GetWidth(); x++){
            double sourceX = xScaleFactor*x, sourceY = yScaleFactor*y, k = sourceX - floor(sourceX), j = 1 - k,
            pixelA[2]= {floor(sourceX), floor(sourceY)}, pixelB[2]= {(floor(sourceX))+1, floor(sourceY)},
            pixelC[2]= {floor(sourceX), (floor(sourceY))+1}, pixelD[2]= {(floor(sourceX))+1, (floor(sourceY))+1},
            y1 = (j * source.Get(pixelA[0], pixelA[1])) + (k * source.Get(pixelB[0], pixelB[1])),
            y2 = (j * source.Get(pixelC[0], pixelC[1])) + (k * source.Get(pixelD[0], pixelD[1]));
            target(x,y) = (k * y1) + (j * y2);
        }

    target.Save(output);
    return 1;
}

int main(){
    GrayscaleImage source;
    float xScale=3, yScale=3;
    source.Load("../../images/read.png");
    if (bilinearInterpolation(source, xScale, yScale, "../../output/BilinearOutput.png"))
        std::cout<<"Image Resized to " << source.GetWidth()*xScale <<"x" << source.GetHeight()*yScale << std::endl;
    else std::cout<<"Image could not be resized."<< std::endl;
    return 0;
}