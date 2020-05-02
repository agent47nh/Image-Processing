#include "../headers/Image.h"

int bandPass(GrayscaleImage &source, const std::string &output) {
    GrayscaleImage target(source.GetWidth(), source.GetHeight());
    double bandPassValue = 3.0*3.0-1, denominator = bandPassValue-9.0;                       //Kernel size is 3x3
    for(int y = 1; y<target.GetHeight()-1; y++)
        for(int x = 1; x<target.GetWidth()-1; x++){
            double pixelA = source.Get(x-1, y-1)*-1.0, pixelB = source.Get(x, y-1)*-1.0, pixelC = source.Get(x+1, y-1)*-1.0,
                   pixelD = source.Get(x-1, y)*-1.0, pixelE = source.Get(x, y)*bandPassValue, pixelF = source.Get(x+1, y)*-1.0,
                   pixelG = source.Get(x-1, y+1)*-1.0, pixelH = source.Get(x, y+1)*-1.0, pixelI = source.Get(x+1, y+1)*-1.0,
                   sum = pixelA + pixelB + pixelC + 
                         pixelD + pixelE + pixelF + 
                         pixelG + pixelH + pixelI,
                   pixelValue = (double)(sum/denominator);
            if(pixelValue>255) pixelValue = 255;
            if(pixelValue<0) pixelValue = 0;
            target(x,y) = pixelValue;
        }
        target.Save(output);
        return 1;
}

int main() {
    GrayscaleImage source;
    source.Load("../../images/7.1.05.png");
    bandPass(source, "../../output/bandPassOutput.png");
    return 0;
}