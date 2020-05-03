#include "../headers/Image.h"

int boxSmoothing(GrayscaleImage &source, const std::string &output) {
    GrayscaleImage target(source.GetWidth(), source.GetHeight());
    double lowPassValue = 1.0/(3.0*3.0), denominator = lowPassValue*9.0;                       //Kernel size is 3x3
    for(int y = 0; y<target.GetHeight(); y++)
        for(int x = 0; x<target.GetWidth(); x++){
            double pixelA = source.Get(x-1, y-1)*lowPassValue, pixelB = source.Get(x, y-1)*lowPassValue, pixelC = source.Get(x+1, y-1)*lowPassValue,
                   pixelD = source.Get(x-1, y)*lowPassValue, pixelE = source.Get(x, y)*lowPassValue, pixelF = source.Get(x+1, y)*lowPassValue,
                   pixelG = source.Get(x-1, y+1)*lowPassValue, pixelH = source.Get(x, y+1)*lowPassValue, pixelI = source.Get(x+1, y+1)*lowPassValue,
                   sum = pixelA + pixelB + pixelC + 
                         pixelD + pixelE + pixelF + 
                         pixelG + pixelH + pixelI,
                   pixelValue = (double)(sum/denominator)+0.5;
            if(pixelValue>255) pixelValue = 255;
            if(pixelValue<0) pixelValue = 0;
            target(x,y) = pixelValue;
        }
        target.Save(output);
        return 1;
}

int main() {
    GrayscaleImage source;
    source.Load("../../images/7.1.01.png");
    boxSmoothing(source, "../../output/lowPassOutput.png");
    return 0;
}