#include "../headers/Image.h"
#include <cmath>

bool adjustBrightness(ColorImage& source,const int brightness,const std::string outputFilePath){
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

bool adjustGamma(ColorImage& source,const double gamma,const std::string outputFilePath){
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

bool adjustContrast(ColorImage& source,const double contrast,const std::string outputFilePath){
    ColorImage target(source.GetWidth(), source.GetHeight());
    for(int x=0;x<source.GetWidth();x++)
        for(int y=0;y<source.GetHeight();y++){
            target(x,y).r = contrast * (source(x,y).r-((double)255/2)) + (255/2);
            target(x,y).g = contrast * (source(x,y).g-((double)255/2)) + (255/2);
            target(x,y).b = contrast * (source(x,y).b-((double)255/2)) + (255/2);
        }
    target.Save(outputFilePath);
    return true;
}

int main() {
    ColorImage source;
    source.Load("../../images/4.2.02.png");
    adjustBrightness(source, -50, "../../output/brighter.png");
    adjustGamma(source, 5, "../../output/gamma.png");
    adjustGamma(source, 2, "../../output/contrast.png");
    return 0;
}