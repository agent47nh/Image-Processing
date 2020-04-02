#include "../headers/Image.h"

int findMaxPixel(const GrayscaleImage& source){
    int max=0;
    for(int x=0; x<source.GetWidth(); x++)
        for(int y=0; y<source.GetHeight(); y++){
            int currentPixel = source.Get(x,y);
            if(currentPixel>max) max = currentPixel;
        }
    return max;
}

int findMinPixel(const GrayscaleImage& source){
    int min=255;
    for(int x=0; x<source.GetWidth(); x++)
        for(int y=0; y<source.GetHeight(); y++){
            int currentPixel = source.Get(x,y);
            if(currentPixel<min) min = currentPixel;
        }
    return min;
}

bool contrastStretching(const GrayscaleImage& source, std::string outputFilePath) {
    GrayscaleImage target(source.GetWidth(), source.GetHeight());
    int minChannel= findMaxPixel(source), maxChannel=findMinPixel(source);
    for(int x = 0; x<source.GetWidth(); x++)
        for(int y = 0; y<source.GetHeight(); y++)
            target(x,y) = ((source(x,y) - minChannel) * (255/(maxChannel-minChannel))) + 0.5;

    target.Save(outputFilePath);
    return true;
}

int main() {
    GrayscaleImage source;
    source.Load("../../images/5.1.09.png");
    contrastStretching(source, "../../output/contrastStretch.png");
    return 0;
}