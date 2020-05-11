#include "../headers/Image.h"
#include <cmath>

double rTod(double radian){
    return radian * (180.0/3.141592653589793238463);
}

double dTor(double degree){
    return degree * (3.141592653589793238463/180);
}

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

double* RGBtoCMYK(const int colors[3]) {
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

double* CMYKtoRGB(const double colors[4]) {
    static double RGBColorFromCMYK[3] ={255 * (1-colors[0]) * (1-colors[3]), 255 * (1-colors[1]) * (1-colors[3]), 255 * (1-colors[2]) * (1-colors[3])};
    return RGBColorFromCMYK;
}

double* RGBtoXYZ(const double colors[3]) { // Clamped
    double RValue = colors[0], GValue = colors[1], BValue = colors[2];

    if(RValue<0) RValue = 0;
    else if(RValue>255) RValue = 255;

    if(GValue<0) GValue = 0;
    else if(GValue>255) GValue = 255;

    if(BValue<0) BValue = 0;
    else if(BValue>255) BValue = 255;

    double varR = (double)RValue/255, varG = (double)GValue/255, varB = (double) BValue/255;
    static double newColors[3];
    
    if(varR > 0.04045) varR = pow(((varR + 0.055 )/1.055),2.4);
    else varR = varR / 12.92;
    
    if (varG > 0.04045) varG = pow(((varG + 0.055) / 1.055), 2.4);
    else varG = varG / 12.92;

    if (varB > 0.04045) varB = pow(((varB + 0.055) / 1.055), 2.4);
    else varB = varB / 12.92;

    varR *= 100;
    varG *= 100;
    varB *= 100;

    newColors[0] = varR * 0.412453 + varG * 0.357580 + varB * 0.180423;
    newColors[1] = varR * 0.212671 + varG * 0.715160 + varB * 0.072169;
    newColors[2] = varR * 0.019334 + varG * 0.119193 + varB * 0.950227;
    
    return newColors;
}

double* XYZtoRGB(const double colors[3]) {  // Clamped
    double XValue = colors[0], YValue = colors[1], ZValue = colors[2];

    if(XValue<0) XValue = 0;
    else if(XValue>100) XValue = 100;

    if(YValue<0) YValue = 0;
    else if(YValue>100) YValue = 100;

    if(ZValue<0) ZValue = 0;
    else if(ZValue>100) ZValue = 100;

    double varX = (double)XValue/100, varY= (double)YValue/100, varZ = (double) ZValue/100;
    static double newColors[3];

    newColors[0] = (varX *  3.240479) + (varY * -1.53715) + (varZ * -0.498535);
    newColors[1] = (varX * -0.969256) + (varY *  1.875991) + (varZ *  0.041556);
    newColors[2] = (varX *  0.055648) + (varY * -0.204043) + (varZ *  1.057311);
    
    if(newColors[0] > 0.0031308) newColors[0] = 1.055 * pow(newColors[0], (1.0/2.4))  - 0.055;
    else newColors[0] *= 12.92;
    
    if (newColors[1] > 0.0031308) newColors[1] = 1.055 * pow(newColors[1], (1.0/2.4))  - 0.055;
    else newColors[1] *= 12.92;

    if (newColors[2] > 0.0031308) newColors[2] = 1.055 * pow(newColors[2], (1.0/2.4))  - 0.055;
    else newColors[2] *= 12.92;
    
    newColors[0] *= 255+0.5;
    newColors[1] *= 255+0.5;
    newColors[2] *= 255+0.5;
    
    return newColors;
}

double* XYZtoLAB(const double colors[3]) {  // Clamped

    double XValue = colors[0], YValue = colors[1], ZValue = colors[2];

    if(XValue<0) XValue = 0;
    else if(XValue>100) XValue = 100;

    if(YValue<0) YValue = 0;
    else if(YValue>100) YValue = 100;

    if(ZValue<0) ZValue = 0;
    else if(ZValue>100) ZValue = 100;

    // White Reference Point D65: (X, Y, Z) = (0.95047, 1.0000, 1.08883)
    double XRef = 95.047, YRef = 100.0, ZRef = 108.883,
           XVar = XValue / XRef, YVar = YValue / YRef, ZVar = ZValue / ZRef;
    if (XVar > 0.008856) XVar = pow(XVar , (1.0/3)); 
    else XVar = ((903.3 * XVar) + 16)/116;

    if (YVar > 0.008856) YVar = pow(YVar , (1.0/3)); 
    else YVar = ((903.3 * YVar) + 16)/116;

    if (ZVar > 0.008856) ZVar = pow(ZVar , (1.0/3)); 
    else ZVar = ((903.3 * ZVar) + 16)/116;

    static double newColor[3] = {((116 * YVar) - 16), 500 * (XVar - YVar), 200 * (YVar - ZVar)};

    return newColor;
}

double* LABtoXYZ(const double colors[3]) {  //Clamped
    // White Reference Point D65: (X, Y, Z) = (0.95047, 1.0000, 1.08883)
    double luminance = colors[0], chroma = colors[1], hue = colors[2];

    if(luminance<0) luminance = 0;
    else if(luminance>100) luminance = 100;

    if(chroma<0) chroma = 0;
    else if(chroma>100) chroma = 100;

    if(hue<0) hue = 0;
    else if(hue>360) hue = 360;

    double XRef = 95.047, YRef = 100.0, ZRef = 108.883,
           FYVar = (luminance+16)/116, FXVar = (chroma/500.0) + FYVar, FZVar = FYVar - (hue/200), XVar, YVar, ZVar;

    if(pow(FXVar, 3) > 0.008856) XVar =  pow(FXVar, 3);
    else XVar = ((116 * FXVar) - 16) / 903.3;

    if(pow(FYVar, 3) > 0.008856) YVar =  pow(FYVar, 3);
    else YVar = ((116 * FYVar) - 16) / 903.3;

    if(pow(FZVar, 3) > 0.008856) ZVar =  pow(FZVar, 3);
    else ZVar = ((116 * FZVar) - 16) / 903.3;

    static double newColors[3] = {XVar*XRef, YVar*YRef, ZVar*ZRef};

    return newColors;
}

double* LABToLCh(const double colors[3]) {  //Clamped
    double luminance = colors[0], varA = colors[1], brightness = colors[2];

    if(brightness>128) brightness = 128;
    else if(brightness == 0) brightness = 0.0001;
    else if(brightness < -128) brightness = -128;

    if(varA<0) varA = 0;
    else if(varA>100) varA = 100;

    if(luminance<0) luminance = 0;
    else if(luminance>100) luminance = 100;

    static double newColors[3] = {luminance, sqrt(pow(varA,2)+pow(brightness,2)), atan(brightness/varA)>0?((rTod(atan(brightness/varA)))):(rTod((atan(brightness/colors[1])+360)))};
    return newColors;
}

double* LChToLAB(const double colors[3]) {  //Clamped
    double luminance = colors[0], chroma = colors[1], hue = colors[2];

    if(luminance<0) luminance = 0;
    else if(luminance>100) luminance = 100;

    if(chroma<0) chroma = 0;
    else if(chroma>100) chroma = 100;

    if(hue<0) hue = 0;
    else if(hue>360) hue = 360;

    static double newColors[3] = {luminance, chroma*cos(dTor(hue)), chroma*sin(dTor(hue))};
    return newColors;
}

double* RGBtoLCh(const double colors[3]) {
    return LABToLCh(XYZtoLAB(RGBtoXYZ(colors)));
}

int main(){
    // double rgb[] = {235, 200, 183};
    // // double *hsl = RGBtoHSL(rgb), *rgbfrom = HSLtoRGB(hsl), *cmyk = RGBtoCMYK(rgb), *rgbfromcmyk = CMYKtoRGB(cmyk), *RGBtoXYZa = RGBtoXYZ(rgb),
    // //        *XYZtoRGBa = XYZtoRGB(RGBtoXYZa), *XYZtoLABa = XYZtoLAB(RGBtoXYZa), *LABtoXYZa = LABtoXYZ(XYZtoLABa), *LABtoLCHa = LABToLCh(XYZtoLABa),
    // //        *LCHtoLABa = LChToLAB(LABtoLCHa);
    // std::cout<< "Original ---> R: " << rgb[0] << " \t\tG: " << rgb[1] << " \t\tB: " << rgb[2] << std::endl;
    // // std::cout<< "Converted --> X: "<< RGBtoXYZa[0] << " \tY: " <<RGBtoXYZa[1]<< " \tZ: " <<RGBtoXYZa[2]<<std::endl;
    // // std::cout<< "Converted --> L*: "<< XYZtoLABa[0] << " \tA*: " <<XYZtoLABa[1]<< " \tB*: " <<XYZtoLABa[2]<<std::endl;
    // // std::cout<< "Converted --> L*: "<< LABtoLCHa[0] << " \tC: " <<LABtoLCHa[1]<< " \th: " <<LABtoLCHa[2]<<std::endl;
    // // std::cout<< "Reverted ---> L*: "<< LCHtoLABa[0] << " \tA*: " <<LCHtoLABa[1]<< " \tB*: " <<LCHtoLABa[2]<<std::endl;
    // double *valueME = LABToLCh(XYZtoLAB(RGBtoXYZ(rgb)));
    // std::cout<< "Converted --> L*: "<< valueME[0] << " \tC: " <<valueME[1]<< " \th: " <<valueME[2]<<std::endl;


    ColorImage source;
    source.Load("../../images/house.png");
    ColorImage target(source.GetWidth(), source.GetHeight());

    for(int x = 0; x < target.GetWidth(); x++)
        for(int y = 0; y < target.GetHeight(); y++){
            int red = source.Get(x,y).r, green = source.Get(x,y).g, blue = source.Get(x,y).b; double colors[] = {red, green, blue};
            double *rgbToLCh = RGBtoLCh(colors);
            std::cout<< "Converted --> L*: "<< rgbToLCh[0] << " \tC: " <<rgbToLCh[1]<< " \th: " <<rgbToLCh[2]<<std::endl;
            // target(x,y).r = backRGB[0];
            // target(x,y).g = backRGB[1];
            // target(x,y).b = backRGB[2];
        }

    // target.Save("../../output/LChOutput.png");
}