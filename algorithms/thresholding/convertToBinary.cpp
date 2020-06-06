#include "../headers/Image.h"
#include <utility>
#include <fstream>
#include <cmath>

struct coordinates{
    int x, y;
};

struct threeColors
{
    double color1;
    double color2;
    double color3;
};

std::vector<coordinates> regions, skipList;

double rTod(double radian){
    return radian * (180.0/3.141592653589793238463);
}

double dTor(double degree){
    return degree * (3.141592653589793238463/180);
}

threeColors RGBToHSV(const threeColors RGBInput){
    threeColors HSV;
    double red = RGBInput.color1, green = RGBInput.color2, blue = RGBInput.color3;
    if(red>255) red = 255;
    else if(red<0) red = 0;
    if(green>255) green = 255;
    else if(green<0) green = 0;
    if(blue>255) blue = 255;
    else if(blue<0) blue = 0;
    
    double redD = (double) red/255, greenD = (double) green/255, blueD = (double) blue/255,
           max = std::max(redD, std::max(greenD, blueD)), min = std::min(redD, std::min(greenD, blueD)),
           delta = max - min, newH=0, newS=0, newV=0;
    if (delta == 0) newH = 0;
    else if (max == redD) newH = 60 * fmod(((greenD-blueD)/delta),6);
    else if (max == greenD) newH = 60 * (((blueD-redD)/delta)+2);
    else if (max == blueD) newH = 60 * (((redD-greenD)/delta)+4);


    if (max > 0) newS = delta/max;
    else newS=0;

    newV = max;

    HSV.color1 = newH; HSV.color2 = (double) newS*100, HSV.color3 = (double)newV*100;

    return HSV;
}

threeColors HSVToRGB(const threeColors HSVInput){
    threeColors RGBColor;
    double hue = HSVInput.color1, saturation = (double)HSVInput.color2/100, value = (double)HSVInput.color3/100;

    if(hue>360) hue = 360;
    else if(hue<0) hue = 0;

    if(saturation>100) saturation = 100;
    else if(saturation<0) saturation = 0;
    
    if(value>100) value = 100;
    else if(value<0) value = 0;

    double currentH = hue, currentS = saturation, currentV = value,
           varC = currentV * currentS,
           varX = varC * (1-fabs(fmod((currentH/60), 2)-1)),
           varM = currentV - varC, newR, newG, newB;

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
    RGBColor.color1 = (newR + varM)* 255; RGBColor.color2 = (newG + varM) * 255; RGBColor.color3 = (newB + varM) * 255;
    return RGBColor;
}

threeColors RGBtoXYZ(const threeColors RGBInput) { // Clamped
    double RValue = RGBInput.color1, GValue = RGBInput.color2, BValue = RGBInput.color3;

    if(RValue<0) RValue = 0;
    else if(RValue>255) RValue = 255;

    if(GValue<0) GValue = 0;
    else if(GValue>255) GValue = 255;

    if(BValue<0) BValue = 0;
    else if(BValue>255) BValue = 255;

    double varR = (double)RValue/255, varG = (double)GValue/255, varB = (double) BValue/255;
    threeColors XYZOutput;
    
    if(varR > 0.04045) varR = pow(((varR + 0.055 )/1.055),2.4);
    else varR = varR / 12.92;
    
    if (varG > 0.04045) varG = pow(((varG + 0.055) / 1.055), 2.4);
    else varG = varG / 12.92;

    if (varB > 0.04045) varB = pow(((varB + 0.055) / 1.055), 2.4);
    else varB = varB / 12.92;

    varR *= 100;
    varG *= 100;
    varB *= 100;

    XYZOutput.color1 = varR * 0.412453 + varG * 0.357580 + varB * 0.180423;
    XYZOutput.color2 = varR * 0.212671 + varG * 0.715160 + varB * 0.072169;
    XYZOutput.color3 = varR * 0.019334 + varG * 0.119193 + varB * 0.950227;
    
    return XYZOutput;

}

threeColors XYZtoRGB(const threeColors XYZInput) {  // Clamped
    double XValue = XYZInput.color1, YValue = XYZInput.color2, ZValue = XYZInput.color3;

    if(XValue<0) XValue = 0;
    else if(XValue>100) XValue = 100;

    if(YValue<0) YValue = 0;
    else if(YValue>100) YValue = 100;

    if(ZValue<0) ZValue = 0;
    else if(ZValue>100) ZValue = 100;

    double varX = (double)XValue/100, varY= (double)YValue/100, varZ = (double) ZValue/100;
    threeColors RGBOutput;

    RGBOutput.color1 = (varX *  3.240479) + (varY * -1.53715) + (varZ * -0.498535);
    RGBOutput.color2 = (varX * -0.969256) + (varY *  1.875991) + (varZ *  0.041556);
    RGBOutput.color3 = (varX *  0.055648) + (varY * -0.204043) + (varZ *  1.057311);
    
    if(RGBOutput.color1 > 0.0031308) RGBOutput.color1 = 1.055 * pow(RGBOutput.color1, (1.0/2.4))  - 0.055;
    else RGBOutput.color1 *= 12.92;
    
    if (RGBOutput.color2 > 0.0031308) RGBOutput.color2 = 1.055 * pow(RGBOutput.color2, (1.0/2.4))  - 0.055;
    else RGBOutput.color2 *= 12.92;

    if (RGBOutput.color3 > 0.0031308) RGBOutput.color3 = 1.055 * pow(RGBOutput.color3, (1.0/2.4))  - 0.055;
    else RGBOutput.color3 *= 12.92;
    
    RGBOutput.color1 *= 255+0.5;
    RGBOutput.color2 *= 255+0.5;
    RGBOutput.color3 *= 255+0.5;
    
    return RGBOutput;
}

threeColors XYZtoLAB(const threeColors XYZInput) {  // Clamped

    double XValue = XYZInput.color1, YValue = XYZInput.color2, ZValue = XYZInput.color3;

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

    threeColors LABOutput = {((116 * YVar) - 16), 500 * (XVar - YVar), 200 * (YVar - ZVar)};

    return LABOutput;
}

threeColors LABtoXYZ(const threeColors LABInput) {  //Clamped
    // White Reference Point D65: (X, Y, Z) = (0.95047, 1.0000, 1.08883)
    double luminance = LABInput.color1, AVar = LABInput.color2, BVar = LABInput.color3;

    if(luminance<0) luminance = 0;
    else if(luminance>100) luminance = 100;

    if(AVar <  -128) AVar = -128;
    else if(AVar>128) AVar = 128;

    if(BVar < -128) BVar = -128;
    else if(BVar > 128) BVar = 128;

    double XRef = 95.047, YRef = 100.0, ZRef = 108.883,
           FYVar = (luminance+16)/116, FXVar = (AVar/500.0) + FYVar, FZVar = FYVar - (BVar/200), XVar, YVar, ZVar;

    if(pow(FXVar, 3) > 0.008856) XVar =  pow(FXVar, 3);
    else XVar = ((116 * FXVar) - 16) / 903.3;

    if(pow(FYVar, 3) > 0.008856) YVar =  pow(FYVar, 3);
    else YVar = ((116 * FYVar) - 16) / 903.3;

    if(pow(FZVar, 3) > 0.008856) ZVar =  pow(FZVar, 3);
    else ZVar = ((116 * FZVar) - 16) / 903.3;

    threeColors XYZOutput = {XVar*XRef, YVar*YRef, ZVar*ZRef};

    return XYZOutput;
}

threeColors LABToLCh(const threeColors LABInput) {  //Clamped
    double luminance = LABInput.color1, varA = LABInput.color2, brightness = LABInput.color3;

    if(brightness>128) brightness = 128;
    else if(brightness == 0) brightness = 0.0001;
    else if(brightness < -128) brightness = -128;

    if(varA<-128) varA = -128;
    else if(varA == 0) varA = 0.0001;
    else if(varA>128) varA = 128;

    if(luminance<0) luminance = 0;
    else if(luminance>100) luminance = 100;

    threeColors LChOutput = {luminance, sqrt(pow(varA,2)+pow(brightness,2)), atan(brightness/varA)>0?rTod(atan(brightness/varA)):rTod(atan(brightness/varA))+180};
    return LChOutput;
}

threeColors LChToLAB(const threeColors LChInput) {  //Clamped
    double luminance = LChInput.color1, chroma = LChInput.color2, hue = LChInput.color3;

    if(luminance<0) luminance = 0;
    else if(luminance>100) luminance = 100;

    if(chroma<0) chroma = 0;
    else if(chroma>200) chroma = 200;

    if(hue<0) hue = 0;
    else if(hue>360) hue = 360;

    threeColors LABOutput = {luminance, chroma*cos(dTor(hue)), chroma*sin(dTor(hue))};
    return LABOutput;
}

threeColors RGBToLCh(const threeColors colors) {
    return LABToLCh(XYZtoLAB(RGBtoXYZ(colors)));
}

threeColors LChToRGB(const threeColors color) {
    return XYZtoRGB(LABtoXYZ(LChToLAB(color)));
}

double adaptiveThresholdValue(GrayscaleImage& source){
    double average=0, averageTrue=0, averageFalse=0, threshold;
    
    for(int y = 0; y<source.GetHeight(); y++)
        for(int x = 0; x < source.GetWidth(); x++)
            average += source(x,y);
    average /= source.GetHeight()*source.GetWidth();

    for(int y = 0; y<source.GetHeight(); y++)
        for(int x = 0; x < source.GetWidth(); x++)
            if(source.Get(x,y)>average)
                averageTrue += source.Get(x,y);

    for(int y = 0; y<source.GetHeight(); y++)
        for(int x = 0; x < source.GetWidth(); x++)
            if(source.Get(x,y)<average)
                averageFalse += source.Get(x,y);

    averageTrue /= source.GetHeight()*source.GetWidth();
    averageFalse /= source.GetHeight()*source.GetWidth();
    threshold = (averageTrue+averageFalse)/2;

    return threshold;
}

GrayscaleImage imageToBinary(GrayscaleImage &source, int threshold = 0, int invert = 0) { // Fixed Threshold when a value is selected & 0 for no invert
    GrayscaleImage target(source.GetWidth(), source.GetHeight());
    int upper = 255, lower = 0;
    
    if(!threshold) threshold = adaptiveThresholdValue(source);
    if(invert) std::swap(upper, lower);

    for(int y = 0; y<target.GetHeight(); y++)
        for(int x = 0; x < target.GetWidth(); x++)
            if(source.Get(x,y)>threshold) target(x,y) = upper;
            else target(x,y) = lower;

    return target;
}

void logicalOperation(GrayscaleImage &source0, GrayscaleImage &source1, const std::string &output, int adaptive = 0, int operationType = 0) {
    int height = source0.GetHeight() < source1.GetHeight()? source0.GetHeight(): source1.GetHeight(),
        width = source0.GetWidth() < source1.GetWidth()? source1.GetWidth(): source1.GetWidth();
    GrayscaleImage binarySource0, binarySource1,target(width,height);
    
    binarySource0 = imageToBinary(source0, adaptive, 0);
    binarySource1 = imageToBinary(source1, adaptive, 0);
    
    if(operationType)
        for(int y = 0; y<target.GetHeight(); y++)
            for(int x = 0; x<target.GetWidth(); x++)
                target(x,y) = binarySource0.Get(x,y) & binarySource1.Get(x,y);
    else
        for(int y = 0; y<target.GetHeight(); y++)
            for(int x = 0; x<target.GetWidth(); x++)
                target(x,y) = binarySource0.Get(x,y) | binarySource1.Get(x,y);

    target.Save(output);
}

bool checkList(std::vector<coordinates> &list, int x, int y){
    for(int i = 0; i<list.size(); i++){
        if(list[i].x == x && list[i].y == y) return true;
    }
    return false;
}

void labelCheck(const GrayscaleImage& source, int x, int y){
    int up = source.Get(x,y+1), down = source.Get(x,y-1), left = source.Get(x-1,y), right = source.Get(x+1,y), center = source.Get(x,y);
    if(up == 255 && !checkList(skipList, x, y+1)){
        regions.push_back({x,y+1});
        skipList.push_back({x,y+1});
        labelCheck(source, x, y+1);
    }
    if(down == 255 && !checkList(skipList, x, y-1)){
        regions.push_back({x,y-1});
        skipList.push_back({x,y-1});
        labelCheck(source, x, y-1);
    }
    if(left == 255 && !checkList(skipList, x-1,y)){
        regions.push_back({x-1,y});
        skipList.push_back({x-1,y});
        labelCheck(source, x-1,y);
    }
    if(right == 255 && !checkList(skipList, x+1,y)){
        regions.push_back({x+1,y});
        skipList.push_back({x+1,y});
        labelCheck(source, x+1,y);
    }
}

std::vector<coordinates> labelRegions(GrayscaleImage &source){
    std::vector<coordinates> startPoints;
    skipList.clear(); regions.clear();
    source = imageToBinary(source);
    for(int y = 0; y<source.GetHeight(); y++)
        for(int x = 0; x<source.GetWidth(); x++){
            if(source.Get(x,y)==255 && !checkList(skipList, x, y)) {
                skipList.push_back({x,y});
                labelCheck(source, x, y);
                startPoints.push_back({x,y});
            }
        }
    for(int i=0; i < startPoints.size(); i++)
        std::cout<<"X: " << startPoints[i].x << " Y: " << startPoints[i].y <<std::endl;
    return startPoints; 
}

void LChSegmentation(ColorImage& source, const std::string &output, double lightness, double chroma, double hue, double toLight=0, double toChroma=0, double toHue=0){
    ColorImage target(source.GetWidth(), source.GetHeight());

    for(int y=0; y<target.GetHeight(); y++)
        for(int x=0; x<target.GetWidth(); x++){
            double red = source.Get(x,y).r, green = source.Get(x,y).g, blue = source.Get(x,y).b, threshold = 2;
            threeColors RGB = {red, green, blue}, LCh = RGBToLCh(RGB);
            if(LCh.color1 < lightness + threshold && LCh.color1 > lightness - threshold) LCh.color1 = toLight;
            if(LCh.color2 < chroma + threshold && LCh.color2 > chroma - threshold) LCh.color2 = toChroma;
            if(LCh.color3 < hue + threshold && LCh.color3 > hue - threshold) LCh.color3 = toHue;
            RGB = LChToRGB(LCh);
            target(x,y).r = RGB.color1;
            target(x,y).g = RGB.color2;
            target(x,y).b = RGB.color3;
        }
    
    target.Save(output);
}

void HSVSegmentation(ColorImage& source, const std::string &output, double hue, double saturation, double value, double toHue=0, double toSaturation=0, double toValue=0){
    ColorImage target(source.GetWidth(), source.GetHeight());

    for(int y=0; y<target.GetHeight(); y++)
        for(int x=0; x<target.GetWidth(); x++){
            double red = source.Get(x,y).r, green = source.Get(x,y).g, blue = source.Get(x,y).b, threshold = 1;
            threeColors RGB = {red, green, blue}, HSV = RGBToHSV(RGB);
            if(HSV.color1 < hue + threshold && HSV.color1 > hue - threshold) HSV.color1 = toHue;
            if(HSV.color2 < saturation + threshold && HSV.color2 > saturation - threshold) HSV.color2 = toSaturation;
            if(HSV.color3 < value + threshold && HSV.color3 > value - threshold) HSV.color3 = toValue;
            RGB = HSVToRGB(HSV);
            target(x,y).r = RGB.color1;
            target(x,y).g = RGB.color2;
            target(x,y).b = RGB.color3;
        }
    
    target.Save(output);
}

void dilation(GrayscaleImage& source, const std::string &output){
    GrayscaleImage target = imageToBinary(source);
    int kernel[2][2] = {{1,0},{0,0}};
    for(int y = 0; y<target.GetHeight(); y++)
        for(int x = 0; x<target.GetWidth(); x++){
            if(target.Get(x,y)==255 && kernel[0][0]==1) {
                target(x+kernel[0][0], y+kernel[0][1]) = 255;
                target(x+kernel[1][0], y+kernel[1][1]) = 255;
            }
        }
    target.Save(output);
}

int main(){
    GrayscaleImage source0, source1;
    source0.Load("../../images/5boxes.png");
    GrayscaleImage target=source0;
    ColorImage source3,source4;
    dilation(source0, "../../output/dilation.png");
    source3.Load("../../images/segmentMe.png");
    source4.Load("../../images/segmentMe1.png");
    HSVSegmentation(source4, "../../output/HSVSegment.png", 349.4, 17.8, 74.9, 338, 91.7, 47.1);
    LChSegmentation(source3, "../../output/LChSegment.png", 30, 80, 110, 68.6, 14.2, 4.3);
  
    source1.Load("../../images/Peppers2.png");
    target = imageToBinary(source0, 0, 1);                        // pass 1 for invert in the last argument
    target.Save("../../output/fixedThreshold.png");
    logicalOperation(source0, source1, "../../output/logicalOperation.png", 128);
    
    
    
    // std::vector<coordinates> startPoints = labelRegions(source0), endPoints;

    return 0;
}