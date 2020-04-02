#include "../headers/Image.h"
#include <cmath>

bool histogramEqualization(const GrayscaleImage& source, GrayscaleImage& target, const std::string outputFilePath) {
    int bitCount[255] = {}; 
    float probabilityDF[255] = {}, PDFChecker = 0, cumulativeDF[255] = {}, pixelCount=0, xCDF[255] = {}, flooredxCDF[255] = {};
    for (int y = 0; y < source.GetHeight()+1; y++)
        for(int x = 0; x < source.GetWidth()+1; x++){
            int currentPixel = source.Get(x,y);
            bitCount[currentPixel-1]++;
        }
    
    for(int x = 0; x<255;x++) pixelCount+=bitCount[x];
    for(int x = 0; x<255;x++) probabilityDF[x] = (float)bitCount[x]/pixelCount;
    for(int x = 0; x<255;x++) PDFChecker += probabilityDF[x];
    for(int x = 0; x<255;x++) cumulativeDF[x] = probabilityDF[x]+cumulativeDF[x-1];
    if(!PDFChecker) return false;
    for(int x = 0; x<255;x++) {
        xCDF[x] = cumulativeDF[x] * 255;
        flooredxCDF[x] = floor(xCDF[x]);
    }
    for (int y = 0; y < source.GetHeight()+1; y++)
        for(int x = 0; x < source.GetWidth()+1; x++){
            int currentPixel = source.Get(x,y);
            target(x,y) = xCDF[currentPixel];
        }
    target.Save(outputFilePath);
    return true;
}

int main() {
    GrayscaleImage source0, source1, source2, source3;    

    source0.Load("../../images/barbara.png");
    source1.Load("../../images/elaine.512.png");
    source2.Load("../../images/4.2.02.png");
    source3.Load("../../images/boat.512.png");

    GrayscaleImage target0(source0.GetWidth(), source0.GetHeight()), 
                target1(source1.GetWidth(), source1.GetHeight()), 
                target2(source2.GetWidth(), source2.GetHeight()),
                target3(source3.GetWidth(), source3.GetHeight());

    if(histogramEqualization(source0, target0, "../../output/histogramEQ00.png"));
    else std::cout<<"PDF Check failed"<<std::endl;

    if(histogramEqualization(source1, target1, "../../output/histogramEQ01.png"));
    else std::cout<<"PDF Check failed"<<std::endl;

    if(histogramEqualization(source2, target2, "../../output/histogramEQ02.png"));
    else std::cout<<"PDF Check failed"<<std::endl;

    if(histogramEqualization(source3, target3, "../../output/histogramEQ03.png"));
    else std::cout<<"PDF Check failed"<<std::endl;

    return 0;
}