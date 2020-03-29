#include "../headers/Image.h"
#include <math.h>

//(p[1] + 0.5 * x*(p[2] - p[0] + x*(2.0*p[0] - 5.0*p[1] + 4.0*p[2] - p[3] + x*(3.0*(p[1] - p[2]) + p[3] - p[0]))))

int main(){
    const float xScale = 0.5, yScale = 0.5;
    GrayscaleImage source;
    source.Load("../../images/read.png");
    GrayscaleImage target(source.GetWidth()*xScale, source.GetHeight()*yScale);
    const double xScaleFactor = (double)source.GetWidth()/target.GetWidth(), yScaleFactor = (double)source.GetHeight()/target.GetHeight();
    for(int y=0; y<target.GetHeight(); y++)
        for(int x=0; x<target.GetWidth(); x++){
            double sourceX = xScaleFactor*x, sourceY = yScaleFactor*y,
            pixelA[2]= {floor(sourceX)-1, floor(sourceY)-1}, pixelB[2]= {floor(sourceX)-1, floor(sourceY)}, pixelC[2]= {floor(sourceX)-1, floor(sourceY)+1}, pixelD[2]= {floor(sourceX)-1, floor(sourceY)+2},
            pixelAA[2]= {floor(sourceX), floor(sourceY)-1}, pixelAB[2]= {floor(sourceX)+1, floor(sourceY)-1}, pixelAC[2]= {floor(sourceX)+2, floor(sourceY)-1},
            pixelBA[2]= {floor(sourceX), floor(sourceY)}, pixelBB[2]= {floor(sourceX)+1, floor(sourceY)}, pixelBC[2]= {floor(sourceX)+2, floor(sourceY)},
            pixelCA[2]= {floor(sourceX), floor(sourceY)+1}, pixelCB[2]= {floor(sourceX)+1, floor(sourceY)+1}, pixelCC[2]= {floor(sourceX)+2, floor(sourceY)+1},
            pixelDA[2]= {floor(sourceX), floor(sourceY)+3}, pixelDB[2]= {floor(sourceX)+1, floor(sourceY)+3}, pixelDC[2]= {floor(sourceX)+2, floor(sourceY)+3},
            horizontalPixels[4] = {
                ((source.Get(pixelAA[0], pixelAA[1])) + 0.5 * 0.2 *((source.Get(pixelAB[0], pixelAB[1])) - (source.Get(pixelA[0], pixelA[1])) + 0.2 *(2.0*(source.Get(pixelA[0], pixelA[1])) 
                - 5.0*(source.Get(pixelAA[0], pixelAA[1])) + 4.0*(source.Get(pixelAB[0], pixelAB[1])) - (source.Get(pixelAC[0], pixelAC[1])) + 0.2 *(3.0*((source.Get(pixelAA[0], pixelAA[1]))) 
                - (source.Get(pixelAB[0], pixelAB[1]))) + (source.Get(pixelAC[0], pixelAC[1])) - (source.Get(pixelA[0], pixelA[1]))))),
                
                ((source.Get(pixelBA[0], pixelBA[1])) + 0.5 * 0.2 *((source.Get(pixelBB[0], pixelBB[1])) - (source.Get(pixelB[0], pixelB[1])) + 0.2 *(2.0*(source.Get(pixelB[0], pixelB[1])) 
                - 5.0*(source.Get(pixelBA[0], pixelBA[1])) + 4.0*(source.Get(pixelBB[0], pixelBB[1])) - (source.Get(pixelBC[0], pixelBC[1])) + 0.2 *(3.0*((source.Get(pixelBA[0], pixelBA[1]))) 
                - (source.Get(pixelBB[0], pixelBB[1]))) + (source.Get(pixelBC[0], pixelBC[1])) - (source.Get(pixelB[0], pixelB[1]))))),
                
                ((source.Get(pixelCA[0], pixelCA[1])) + 0.5 * 0.2 *((source.Get(pixelCB[0], pixelCB[1])) - (source.Get(pixelC[0], pixelC[1])) + 0.2 *(2.0*(source.Get(pixelC[0], pixelC[1])) 
                - 5.0*(source.Get(pixelCA[0], pixelCA[1])) + 4.0*(source.Get(pixelCB[0], pixelCB[1])) - (source.Get(pixelCC[0], pixelCC[1])) + 0.2 *(3.0*((source.Get(pixelCA[0], pixelCA[1]))) 
                - (source.Get(pixelCB[0], pixelCB[1]))) + (source.Get(pixelCC[0], pixelCC[1])) - (source.Get(pixelC[0], pixelC[1]))))),
                
                ((source.Get(pixelDA[0], pixelDA[1])) + 0.5 * 0.2 *((source.Get(pixelDB[0], pixelDB[1])) - (source.Get(pixelD[0], pixelD[1])) + 0.2 *(2.0*(source.Get(pixelD[0], pixelD[1])) 
                - 5.0*(source.Get(pixelDA[0], pixelDA[1])) + 4.0*(source.Get(pixelDB[0], pixelDB[1])) - (source.Get(pixelDC[0], pixelDC[1])) + 0.2 *(3.0*((source.Get(pixelDA[0], pixelDA[1]))) 
                - (source.Get(pixelDB[0], pixelDB[1]))) + (source.Get(pixelDC[0], pixelDC[1])) - (source.Get(pixelD[0], pixelD[1])))))
                },
            verticalPixel=
                    (horizontalPixels[1] + 0.5 * 0.001 *(horizontalPixels[2] - horizontalPixels[0] + 0.001 *(2.0*horizontalPixels[0] - 5.0*horizontalPixels[1] + 4.0*horizontalPixels[2] - 
                    horizontalPixels[3] + 0.001 *(3.0*(horizontalPixels[1] - horizontalPixels[2]) + horizontalPixels[3] - horizontalPixels[0]))));
            target(x,y) = verticalPixel;
            int varna = source.Get(pixelBA[0], pixelBA[1]);
            std::cout<< verticalPixel <<std::endl;
        }
    target.Save("../../output/BiCubicOutput.png");
    std::cout<<"Image Resized to " << source.GetWidth()*xScale <<"x" << source.GetHeight()*yScale << std::endl;
    return 0;
}