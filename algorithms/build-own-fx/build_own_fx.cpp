#include "../headers/Image.h"
#include <cmath>

struct threeColors
{
    double color1;
    double color2;
    double color3;
};

double rTod(double radian){
    return radian * (180.0/3.141592653589793238463);
}

double dTor(double degree){
    return degree * (3.141592653589793238463/180);
}

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

ColorImage boxSharpeningColor(ColorImage &source) {
    ColorImage target(source.GetWidth(), source.GetHeight());
    double highPassValue = 3.0*3.0, denominator = highPassValue-5.0;                       //Kernel size is 3x3
    for(int y = 0; y<target.GetHeight(); y++)
        for(int x = 0; x<target.GetWidth(); x++){
            double  sumRed = source.Get(x-1, y-1).r*-1.0 + source.Get(x, y-1).r*-1.0 + source.Get(x+1, y-1).r*-1.0 + 
                            source.Get(x-1, y).r*-1.0 + source.Get(x, y).r*highPassValue + source.Get(x+1, y).r*-1.0 + 
                            source.Get(x-1, y+1).r*-1.0 + source.Get(x, y+1).r*-1.0 + source.Get(x+1, y+1).r*-1.0,
                    sumGreen = source.Get(x-1, y-1).g*-1.0 + source.Get(x, y-1).g*-1.0 + source.Get(x+1, y-1).g*-1.0 + 
                            source.Get(x-1, y).g*-1.0 + source.Get(x, y).g*highPassValue + source.Get(x+1, y).g*-1.0 + 
                            source.Get(x-1, y+1).g*-1.0 + source.Get(x, y+1).g*-1.0 + source.Get(x+1, y+1).g*-1.0,
                    sumBlue = source.Get(x-1, y-1).b*-1.0 + source.Get(x, y-1).b*-1.0 + source.Get(x+1, y-1).b*-1.0 + 
                            source.Get(x-1, y).b*-1.0 + source.Get(x, y).b*highPassValue + source.Get(x+1, y).b*-1.0 + 
                            source.Get(x-1, y+1).b*-1.0 + source.Get(x, y+1).b*-1.0 + source.Get(x+1, y+1).b*-1.0,
            pixelBlueValue = (double)(sumBlue/denominator), pixelGreenValue = (double)(sumGreen/denominator), pixelRedValue = (double)(sumRed/denominator);
            

            if(pixelRedValue>255) pixelRedValue = 255;
            if(pixelRedValue<0) pixelRedValue = 0;

            if(pixelGreenValue>255) pixelGreenValue = 255;
            if(pixelGreenValue<0) pixelGreenValue = 0;

            if(pixelBlueValue>255) pixelBlueValue = 255;
            if(pixelBlueValue<0) pixelBlueValue = 0;


            target(x,y).r = pixelRedValue;
            target(x,y).g = pixelGreenValue;
            target(x,y).b = pixelBlueValue;
        }
        
        return target;
}

ColorImage boxSmoothingColor(ColorImage &source) {
    ColorImage target(source.GetWidth(), source.GetHeight());
    double lowPassValue = 1.0/(3.0*3.0), denominator = lowPassValue*9.0;                       //Kernel size is 3x3
    for(int y = 0; y<target.GetHeight(); y++)
        for(int x = 0; x<target.GetWidth(); x++){
            double  sumRed = source.Get(x-1, y-1).r*lowPassValue + source.Get(x, y-1).r*lowPassValue + source.Get(x+1, y-1).r*lowPassValue + 
                            source.Get(x-1, y).r*lowPassValue + source.Get(x, y).r*lowPassValue + source.Get(x+1, y).r*lowPassValue + 
                            source.Get(x-1, y+1).r*lowPassValue + source.Get(x, y+1).r*lowPassValue + source.Get(x+1, y+1).r*lowPassValue,
                    sumGreen = source.Get(x-1, y-1).g*lowPassValue + source.Get(x, y-1).g*lowPassValue + source.Get(x+1, y-1).g*lowPassValue + 
                            source.Get(x-1, y).g*lowPassValue + source.Get(x, y).g*lowPassValue + source.Get(x+1, y).g*lowPassValue + 
                            source.Get(x-1, y+1).g*lowPassValue + source.Get(x, y+1).g*lowPassValue + source.Get(x+1, y+1).g*lowPassValue,
                    sumBlue = source.Get(x-1, y-1).b*lowPassValue + source.Get(x, y-1).b*lowPassValue + source.Get(x+1, y-1).b*lowPassValue + 
                            source.Get(x-1, y).b*lowPassValue + source.Get(x, y).b*lowPassValue + source.Get(x+1, y).b*lowPassValue + 
                            source.Get(x-1, y+1).b*lowPassValue + source.Get(x, y+1).b*lowPassValue + source.Get(x+1, y+1).b*lowPassValue,
                    pixelRedValue = (double)(sumRed/denominator)+0.5, pixelGreenValue = (double)(sumGreen/denominator)+0.5, pixelBlueValue = (double)(sumBlue/denominator)+0.5;
            
            
            if(pixelGreenValue>255) pixelGreenValue = 255;
            if(pixelGreenValue<0) pixelGreenValue = 0;
            if(pixelRedValue>255) pixelRedValue = 255;
            if(pixelRedValue<0) pixelRedValue = 0;
            if(pixelBlueValue>255) pixelBlueValue = 255;
            if(pixelBlueValue<0) pixelBlueValue = 0;

            target(x,y).r = pixelRedValue;
            target(x,y).g = pixelGreenValue;
            target(x,y).b = pixelBlueValue;
            
            
        }
        return target;
}

GrayscaleImage boxSmoothing(GrayscaleImage &source) {
    GrayscaleImage target(source.GetWidth(), source.GetHeight());
    double lowPassValue = 1.0/(3.0*3.0), denominator = lowPassValue*9.0;                       //Kernel size is 3x3
    for(int y = 0; y<target.GetHeight(); y++)
        for(int x = 0; x<target.GetWidth(); x++){
            double  sum = source.Get(x-1, y-1)*lowPassValue + source.Get(x, y-1)*lowPassValue + source.Get(x+1, y-1)*lowPassValue + 
                            source.Get(x-1, y)*lowPassValue + source.Get(x, y)*lowPassValue + source.Get(x+1, y)*lowPassValue + 
                            source.Get(x-1, y+1)*lowPassValue + source.Get(x, y+1)*lowPassValue + source.Get(x+1, y+1)*lowPassValue,
                    pixelValue = (double)(sum/denominator)+0.5;
            
            if(pixelValue>255) pixelValue = 255;
            if(pixelValue<0) pixelValue = 0;

            target(x,y) = pixelValue;
        }
        return target;
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

GrayscaleImage adjustGamma(GrayscaleImage& source,const double gamma) {
    GrayscaleImage target(source.GetWidth(), source.GetHeight());
    for(int x=0;x<source.GetWidth();x++)
        for(int y=0;y<source.GetHeight();y++)
            target(x,y) = 255 * pow(((double)source(x,y) / 255), gamma);
    return target;
}

GrayscaleImage contrastStretching(const GrayscaleImage& source, double level=1) {
    GrayscaleImage target(source.GetWidth(), source.GetHeight());
    int minChannel= findMaxPixel(source), maxChannel=findMinPixel(source);
    for(int x = 0; x<source.GetWidth(); x++)
        for(int y = 0; y<source.GetHeight(); y++)
            target(x,y) = (((source(x,y) - minChannel) * (255/(maxChannel-minChannel))) * level) + 0.5;

    return target;
}

GrayscaleImage sketchFX(ColorImage& source, int controlPixel=100, int blackPoint=50, int whitePoint=255){
	ColorImage target;
    target = boxSmoothingColor(source);
	controlPixel = controlPixel>255?255:controlPixel;
	controlPixel = controlPixel<0?0:controlPixel;
    for(int i=0; i<20; i++) target = boxSmoothingColor(target);
    for(int y = 0; y<target.GetHeight(); y++)
        for(int x = 0; x<target.GetWidth(); x++){
            threeColors rgb = {target.Get(x,y).r, target.Get(x,y).g, target.Get(x,y).b}, toHSL = RGBtoHSL(rgb), toRGB;
            toHSL.color3 *=0.1;
            toRGB = HSLtoRGB(toHSL);

            target(x,y).r = toRGB.color1>255?255:toRGB.color1;
			target(x,y).r = toRGB.color1<0?0:toRGB.color1;
            target(x,y).g = toRGB.color2>255?255:toRGB.color2;
			target(x,y).g = toRGB.color2<0?0:toRGB.color2;
            target(x,y).b = toRGB.color3>255?255:toRGB.color3;
			target(x,y).b = toRGB.color3<0?0:toRGB.color3;
        }
    target = boxSharpeningColor(source);
    for(int i=0; i<1; i++) target = boxSharpeningColor(target);
    GrayscaleImage target0 = target;
	target0 = adjustGamma(target0,-0.1);
    for(int y = 0; y<target0.GetHeight(); y++)
        for(int x = 0; x<target0.GetWidth(); x++){
            target0(x,y) = target0.Get(x,y)<controlPixel?blackPoint:whitePoint;
        }
	// target0 = boxSmoothing(target0);
	return target0;
}

int main(){
    ColorImage source;
    GrayscaleImage target;
    source.Load("../../images/london.png");
	target = sketchFX(source,50,0, 220);
    target.Save("../../output/build_own_fxRGB.png");
    return 0;
}