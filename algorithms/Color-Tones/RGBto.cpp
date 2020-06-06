#include "../headers/Image.h"
#include <cmath>

struct threeColors
{
    double color1;
    double color2;
    double color3;
};

struct fourColors{
    double color1;
    double color2;
    double color3;
    double color4;
};


double rTod(double radian){
    return radian * (180.0/3.141592653589793238463);
}

double dTor(double degree){
    return degree * (3.141592653589793238463/180);
}

threeColors RGBtoHSL(const threeColors RGBInput){
    static double HSLColor[3];
    threeColors HSL;
    double redD = (double) RGBInput.color1/255, greenD = (double) RGBInput.color2/255, blueD = (double) RGBInput.color3/255,
           max = std::max(redD, std::max(greenD, blueD)), min = std::min(redD, std::min(greenD, blueD)),
           delta = max - min, newH, newS, newL;
    if (delta == 0) newH = 0;
    else if (max == redD) newH = 60 * fmod(((greenD-blueD)/delta),6);
    else if (max == greenD) newH = 60 * (((blueD-redD)/delta)+2);
    else if (max == blueD) newH = 60 * (((redD-greenD)/delta)+4);

    newL = (max+min)/2.0;

    if (delta == 0) newS = 0;
    else newS = (double) delta/(1-(fabs(2*newL)-1));

    HSL.color1 = newH+0.5; HSL.color2 = newS, HSL.color3 = newL;

    return HSL;
}

threeColors HSLtoRGB(const threeColors HSLInput){
    threeColors RGBColor;
    double currentH = HSLInput.color1,currentS = HSLInput.color2, currentL = HSLInput.color3,
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
    RGBColor.color1 = (newR + varM)* 255 + 0.5; RGBColor.color2 = (newG + varM) * 255 + 0.5; RGBColor.color3 = (newB + varM) * 255 + 0.5;
    return RGBColor;
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

fourColors RGBtoCMYK(const threeColors RGBInput) {
    fourColors CMYKColor;
    double redD = (double) RGBInput.color1/255, greenD = (double) RGBInput.color2/255, blueD = (double) RGBInput.color3/255,
           max = std::max(redD, std::max(greenD, blueD)), 
           newC, newM, newY, newK;
    newK = (double)1 - max;
    newC = (double)(1-redD-newK)/(1-newK);
    newM = (double)(1-greenD-newK)/(1-newK);
    newY = (double)(1-blueD-newK)/(1-newK);
    CMYKColor.color1 = newC+0.005; CMYKColor.color2 = newM+0.005; CMYKColor.color3 = newY+0.005; CMYKColor.color4 = newK+0.005;

    return CMYKColor;
}

threeColors CMYKtoRGB(const fourColors CMYKInput) {
    threeColors RGBColorFromCMYK ={255 * (1-CMYKInput.color1) * (1-CMYKInput.color4) + 0.5, 255 * (1-CMYKInput.color2) * (1-CMYKInput.color4) + 0.5, 255 * (1-CMYKInput.color3) * (1-CMYKInput.color4) + 0.5};
    return RGBColorFromCMYK;
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

int main(){
    // threeColors RGB = {165, 202, 153};
    // threeColors hsl = RGBtoHSL(RGB), hsl2rgb = HSLtoRGB(hsl); fourColors cmyk = RGBtoCMYK(RGB);
    // threeColors cmyk2rgb = CMYKtoRGB(cmyk), rgb2xyz = RGBtoXYZ(RGB), xyz2rgb = XYZtoRGB(rgb2xyz), xyz2lab = XYZtoLAB(rgb2xyz), lab2xyz = LABtoXYZ(xyz2lab), lab2lch = LABToLCh(xyz2lab), lch2lab = LChToLAB(lab2lch);
    // std::cout<< "Original ---> R: " << RGB.color1 << " \t\tG: " << RGB.color2 << " \t\tB: " << RGB.color3 << std::endl;
    // std::cout<< "Converted --> X: "<< rgb2xyz.color1 << " \tY: " << rgb2xyz.color2 << " \tZ: " << rgb2xyz.color3 <<std::endl;
    // std::cout<< "Converted---> L: "<< xyz2lab.color1 << " \tA: " << xyz2lab.color2 << " \tB: " << xyz2lab.color3 <<std::endl;
    // std::cout<< "Converted---> L: "<< lab2lch.color1 << " \tC: " << lab2lch.color2 << " \th: " << lab2lch.color3 <<std::endl;
    // std::cout<< "Reverted ---> L: "<< lch2lab.color1 << " \tA: " << lch2lab.color2 << " \tB: " << lch2lab.color3 <<std::endl;
    // std::cout<< "Converted --> L*: "<< XYZtoLABa[0] << " \tA*: " <<XYZtoLABa[1]<< " \tB*: " <<XYZtoLABa[2]<<std::endl;
    // std::cout<< "Converted --> L*: "<< LABtoLCHa[0] << " \tC: " <<LABtoLCHa[1]<< " \th: " <<LABtoLCHa[2]<<std::endl;
    // std::cout<< "Reverted ---> L*: "<< LCHtoLABa[0] << " \tA*: " <<LCHtoLABa[1]<< " \tB*: " <<LCHtoLABa[2]<<std::endl;
    // std::cout<< "Converted --> L*: "<< valueME[0] << " \tC: " <<valueME[1]<< " \th: " <<valueME[2]<<std::endl;


    ColorImage source;
    source.Load("../../images/house.png");
    ColorImage target(source.GetWidth(), source.GetHeight());

    for (int y = 0; y < target.GetHeight(); y++)
        for (int x = 0; x < target.GetWidth(); x++){
            double red = source.Get(x,y).r, green = source.Get(x,y).g, blue = source.Get(x,y).b; threeColors colors = {red, green, blue},
            rgb2lch = RGBToLCh(colors);
            rgb2lch.color1 *= 0.5;
            rgb2lch.color2 *= 0.5;
            rgb2lch.color3 *= 0.5;
            threeColors lch2rgb = LChToRGB(rgb2lch);
            target(x,y).r = lch2rgb.color1;
            target(x,y).g = lch2rgb.color2;
            target(x,y).b = lch2rgb.color3;
        }

    target.Save("../../output/LChOutput.png");

}