#include "../headers/Image.h"
#include <cmath>

int main(){
    GrayscaleImage source;
    source.Load("../../images/barbara.png");
    int bitCount[255]={}, pixelCount=0; float probabilityDF[255], PDFChecker=0, cumulativeDF[255]={}, CDFChecker;
    for (int y = 0; y < source.GetHeight()+1; y++)
        for(int x = 0; x < source.GetWidth()+1; x++){
            int currentPixel = source.Get(x,y);
            // std::cout<<"Current Position: (" << x << ", " << y << ") = " << currentPixel << std::endl;
            bitCount[currentPixel-1]++;
        }
    for(int x = 0; x<255;x++) pixelCount+=bitCount[x];
    for(int x = 0; x<255;x++) probabilityDF[x] = (float)bitCount[x]/pixelCount;
    for(int x = 0; x<255;x++) PDFChecker += probabilityDF[x];
    for(int x = 0; x<255;x++) cumulativeDF[x] = probabilityDF[x]+cumulativeDF[x-1];
    std::cout<<"Pixel Count: "<<pixelCount<<std::endl;
    std::cout<<"PDF Sum:"<<PDFChecker<<std::endl;
    std::cout<<"CDF Sum:"<<cumulativeDF[254]<<std::endl;
    return 0;
}