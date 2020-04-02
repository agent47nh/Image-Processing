#include "../headers/Image.h"
#include <cmath>

int main(){
    GrayscaleImage source;
    source.Load("../../images/barbara.png");
    int bits[255]={}, count=0; float probabilityDF[255], PDFChecker=0, cumulativeDF[255]={}, CDFChecker;
    for (int y = 0; y < source.GetHeight()+1; y++)
        for(int x = 0; x < source.GetWidth()+1; x++){
            int currentPixel = source.Get(x,y);
            // std::cout<<"Current Position: (" << x << ", " << y << ") = " << currentPixel << std::endl;
            bits[currentPixel-1]++;
        }
    for(int x = 0; x<255;x++) count+=bits[x];
    for(int x = 0; x<255;x++) probabilityDF[x] = (float)bits[x]/count;
    for(int x = 0; x<255;x++) PDFChecker= probabilityDF[x]+ PDFChecker;
    for(int x = 0; x<255;x++) cumulativeDF[x] = probabilityDF[x]+cumulativeDF[x-1];
    std::cout<<"Pixel Count: "<<count<<std::endl;
    std::cout<<"PDF Sum:"<<PDFChecker<<std::endl;
    std::cout<<"CDF Sum:"<<cumulativeDF[254]<<std::endl;
    return 0;
}