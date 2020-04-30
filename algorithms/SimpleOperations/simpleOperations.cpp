#include "../headers/Image.h"
#include <cmath>

int *findMaxPixel(const ColorImage& source){
    int maxRed=0, maxGreen=0, maxBlue=0;
    static int max[3];
    for(int x=0; x<source.GetWidth(); x++)
        for(int y=0; y<source.GetHeight(); y++){
            int currentPixelRed = source.Get(x,y).r, currentPixelGreen = source.Get(x,y).g, currentPixelBlue = source.Get(x,y).b;
            if(currentPixelRed>maxRed) maxRed = currentPixelRed;
            if(currentPixelRed>maxGreen) maxGreen = currentPixelGreen;
            if(currentPixelRed>maxBlue) maxBlue = currentPixelBlue;
        }
    max[0] = maxRed;
    max[1] = maxGreen;
    max[2] = maxBlue;
    return max;
}

bool adjustBrightness(ColorImage& source,const int brightness,const std::string outputFilePath) {
    ColorImage target(source.GetWidth(), source.GetHeight());
    int threshold = 255 - brightness;
    if(brightness<0) threshold = 0 - brightness;
    for(int x=0;x<source.GetWidth();x++)
        for(int y=0;y<source.GetHeight();y++){
            if (brightness<0){
                if(source(x,y).r <= threshold) target(x,y).r = 0;
                else target(x,y).r = source(x,y).r + brightness;

                if(source(x,y).g <= threshold) target(x,y).g = 0;
                else target(x,y).g = source(x,y).g + brightness;

                if(source(x,y).b <= threshold) target(x,y).b = 0;
                else target(x,y).b = source(x,y).b + brightness;
            } else {
                if(source(x,y).r <= threshold) target(x,y).r = source(x,y).r + brightness;
                else target(x,y).r = 255;

                if(source(x,y).g <= threshold) target(x,y).g = source(x,y).g + brightness;
                else target(x,y).g = 255;

                if(source(x,y).b <= threshold) target(x,y).b = source(x,y).b + brightness;
                else target(x,y).b = 255;
            }
        }

    target.Save(outputFilePath);
    return true;
}

bool adjustGamma(ColorImage& source,const double gamma,const std::string outputFilePath) {
    ColorImage target(source.GetWidth(), source.GetHeight());
    for(int x=0;x<source.GetWidth();x++)
        for(int y=0;y<source.GetHeight();y++){
            target(x,y).r = 255 * pow(((double)source(x,y).r / 255), gamma);
            target(x,y).g = 255 * pow(((double)source(x,y).g / 255), gamma);
            target(x,y).b = 255 * pow(((double)source(x,y).b / 255), gamma);
        }
    target.Save(outputFilePath);
    return true;
}

bool adjustContrast(ColorImage& source,double contrast,const std::string outputFilePath) {
    ColorImage target(source.GetWidth(), source.GetHeight());
    int *max = findMaxPixel(source);
    std::cout << max[0] << ", " << max[1] << ", " << max[2] << std::endl;
    double maxRed = (double)max[0]/2, maxGreen = (double)max[1]/2, maxBlue = (double) max[2]/2;
    if(contrast < 0) contrast = 0;
    for(int x=0;x<source.GetWidth();x++)
        for(int y=0;y<source.GetHeight();y++){
            int redChannel = (contrast*(source.Get(x,y).r)-ceil(maxRed)) + ceil(maxRed),
            greenChannel = (contrast*(source.Get(x,y).g)-ceil(maxGreen)) + ceil(maxGreen),
            blueChannel = (contrast*(source.Get(x,y).b)-ceil(maxBlue)) + ceil(maxBlue);
            if(redChannel>255) redChannel = 255;
            else if(redChannel<0) redChannel = 0;
            if(greenChannel>255) greenChannel = 255;
            else if(greenChannel<0) greenChannel = 0;
            if(blueChannel>255) blueChannel = 255;
            else if(blueChannel<0) greenChannel = 0;
            target(x,y).r = redChannel;
            target(x,y).g = greenChannel;
            target(x,y).b = blueChannel;
            target(x,y).a = 255;
        }
    target.Save(outputFilePath);
    return true;
}

bool invertImage(ColorImage& source, const std::string outputFilePath) {
    ColorImage target(source.GetWidth(), source.GetHeight());
    for(int x=0; x<source.GetWidth(); x++)
        for(int y=0; y<source.GetHeight(); y++){
            target(x,y).r = 255 - source(x,y).r;
            target(x,y).g = 255 - source(x,y).g;
            target(x,y).b = 255 - source(x,y).b;
        }
    target.Save(outputFilePath);
    return true;
}

int main() {
    ColorImage source;
    source.Load("../../images/read.png");
    std::cout<< sizeof(source) << std::endl;
    // std::cout<<"here0" << std::endl;
    adjustBrightness(source, -50, "../../output/brighter.png");
    // std::cout<<"here1" << std::endl;
    adjustGamma(source, 5, "../../output/gamma.png");
    // std::cout<<"here2" << std::endl;
    adjustContrast(source, 10, "../../output/contrast.png");
    invertImage(source, "../../output/invert.png");
    return 0;
}