#include "../headers/Image.h"
#include <math.h>

int main(){
    const float xScale = 0.5, yScale = 0.5;
    GrayscaleImage source;
    source.Load("../../images/read.png");
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

    target.Save("../../output/BilinearOutput.png");
    return 0;
}