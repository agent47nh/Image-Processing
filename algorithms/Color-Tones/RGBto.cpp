#include "../headers/Image.h"
#include <cmath>

double* RGBtoHSL(const int colors[3]){
    static double HSLColor[3];
    double redD = (double) colors[0]/255, greenD = (double) colors[1]/255, blueD = (double) colors[2]/255,
           max = std::max(redD, std::max(greenD, blueD)), min = std::min(redD, std::min(greenD, blueD)),
           delta = max - min, newH, newS, newL;
    if (delta == 0) newH = 0;
    else if (max == redD) newH = 60 * fmod(((greenD-blueD)/delta),6);
    else if (max == greenD) newH = 60 * (((blueD-redD)/delta)+2);
    else if (max == blueD) newH = 60 * (((redD-greenD)/delta)+4);

    newL = (max+min)/2.0;

    if (delta == 0) newS = 0;
    else newS = (double) delta/(1-(fabs(2*newL)-1));

    HSLColor[0] = newH+0.5; HSLColor[1] = newS, HSLColor[2] = newL;

    return HSLColor;
}

double* HSLtoRGB(const double colors[3]){
    static double RGBColor[3];
    double currentH = colors[0],currentS = colors[1], currentL = colors[2],
           varC = (1-fabs((2*currentL)-1)) * currentS,
           varX = varC * (1-fabs(fmod((currentH/60), 2)-1)),
           varM = currentL - (varC/2), newR, newG, newB;

    if (currentH >= 0 && currentH < 60){
        newR = varC;
        newG = varX;
        newB = 0;
    } else if (currentH >= 60 && currentH < 120) {
        newR = varX;
        newG = varC;
        newB = 0;
    } else if (currentH >=120 && currentH < 180) {
        newR = 0;
        newG = varC;
        newB = varX;
    } else if (currentH >=180 && currentH < 240) {
        newR = 0;
        newG = varX;
        newB = varC;
    } else if (currentH >=240 && currentH < 300) {
        newR = varX;
        newG = 0;
        newB = varC;
    } else if (currentH >=300 && currentH < 360) {
        newR = varC;
        newG = 0;
        newB = varX;
    }
    RGBColor[0] = (newR + varM)* 255; RGBColor[1] = (newG + varM) * 255; RGBColor[2] = (newB + varM) * 255;
    return RGBColor;
}

double* RGBtoCMYK(const int colors[3]){
    static double CMYKColor[4];
    double redD = (double) colors[0]/255, greenD = (double) colors[1]/255, blueD = (double) colors[2]/255,
           max = std::max(redD, std::max(greenD, blueD)), 
           newC, newM, newY, newK;
    newK = (double)1 - max;
    newC = (double)(1-redD-newK)/(1-newK);
    newM = (double)(1-greenD-newK)/(1-newK);
    newY = (double)(1-blueD-newK)/(1-newK);
    CMYKColor[0] = newC+0.005; CMYKColor[1] = newM+0.005; CMYKColor[2] = newY+0.005; CMYKColor[3] = newK+0.005;

    return CMYKColor;
}

double* CMYKtoRGB(const double colors[4]){
    static double RGBColorFromCMYK[3] ={255 * (1-colors[0]) * (1-colors[3]), 255 * (1-colors[1]) * (1-colors[3]), 255 * (1-colors[2]) * (1-colors[3])};
    return RGBColorFromCMYK;
}

int main(){
    int rgb[] = {235, 200, 183};
    double* hsl = RGBtoHSL(rgb), *rgbfrom = HSLtoRGB(hsl), *cmyk = RGBtoCMYK(rgb), *rgbfromcmyk = CMYKtoRGB(cmyk);
    std::cout<< "R: " << rgb[0] << " G: " << rgb[1] << " B: " << rgb[2] << std::endl;
    std::cout<< "H: " << (int)hsl[0] << " S: " << hsl[1]*100 << "% L: " << hsl[2]*100 <<"%" << std::endl;
    std::cout<< "R: " << (int)rgbfrom[0] << " G: " << (int)rgbfrom[1] << " B: " << (int)rgbfrom[2] << std::endl;
    std::cout<< "C: " << (int)(cmyk[0]*100) << "% M: " << (int)(cmyk[1]*100) << "% Y: " << (int)(cmyk[2]*100) << "% K: " << (int)(cmyk[3]*100) <<"%" << std::endl;
    std::cout<< "R: " << (int)rgbfromcmyk[0] << " G: " << (int)rgbfromcmyk[1] << " B: " << (int)rgbfromcmyk[2] << std::endl;
}