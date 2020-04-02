#include "headers/Image.h"
#include <math.h>

double cubicInterpol(double pixel[4], double x){
    return ((-0.5 * pixel[0]) + (1.5 * pixel[1]) - (1.5 * pixel[2]) + (0.5 * pixel[3])) * pow(x, 3) 
    + (pixel[0] - (2.5 * pixel[1]) + (2 * pixel[2]) - (0.5 * pixel[3])) * pow(x,2)
    + ((-0.5*pixel[0])+(0.5*pixel[2])) * x + pixel[1];
}

int bicubicInterpolation(GrayscaleImage& source, const float xScale, const float yScale, const std::string& output){
    GrayscaleImage target(source.GetWidth()*xScale, source.GetHeight()*yScale);
    const double xScaleFactor = (double)source.GetWidth()/target.GetWidth(), yScaleFactor = (double)source.GetHeight()/target.GetHeight();
    for(int y=0; y<target.GetHeight(); y++)
        for(int x=0; x<target.GetWidth(); x++){
            double sourceX = xScaleFactor*x, sourceY = yScaleFactor*y,xCoefficient = (xScaleFactor * x) - floor(sourceX), yCoefficient = (yScaleFactor * y) - floor(sourceY),
            pixelA= source.Get(floor(sourceX), floor(sourceY)), pixelAA= source.Get(floor(sourceX)+1, floor(sourceY)), pixelAB= source.Get(floor(sourceX)+2, floor(sourceY)), pixelAC= source.Get(floor(sourceX)+3, floor(sourceY)),
            pixelB= source.Get(floor(sourceX), floor(sourceY)+1), pixelBA= source.Get(floor(sourceX)+1, floor(sourceY)+1), pixelBB= source.Get(floor(sourceX)+2, floor(sourceY)+1), pixelBC= source.Get(floor(sourceX)+3, floor(sourceY)+1),
            pixelC= source.Get(floor(sourceX), floor(sourceY)+2), pixelCA= source.Get(floor(sourceX)+1, floor(sourceY)+2), pixelCB= source.Get(floor(sourceX)+2, floor(sourceY)+2), pixelCC= source.Get(floor(sourceX)+3, floor(sourceY)+2),
            pixelD= source.Get(floor(sourceX), floor(sourceY)+3), pixelDA= source.Get(floor(sourceX)+1, floor(sourceY)+3), pixelDB= source.Get(floor(sourceX)+2, floor(sourceY)+3), pixelDC= source.Get(floor(sourceX)+3, floor(sourceY)+3),
            rowA[4]= {pixelA, pixelAA, pixelAB, pixelAC}, rowB[4]= {pixelB, pixelBA, pixelBB, pixelBC}, rowC[4]= {pixelC, pixelCA, pixelCB, pixelCC}, rowD[4]= {pixelD, pixelDA, pixelDB, pixelDC},
            horizontalPixels[4] = {
                cubicInterpol(rowA, yCoefficient), cubicInterpol(rowB, yCoefficient), cubicInterpol(rowC, yCoefficient), cubicInterpol(rowD, yCoefficient)
            };
            target(x,y) = cubicInterpol(horizontalPixels, xCoefficient);
        }
    target.Save(output);
    return 1;
}

int bilinearInterpolation(GrayscaleImage &source, const float xScale, const float yScale, const std::string &output) {
    GrayscaleImage target(source.GetWidth() * xScale, source.GetHeight() * yScale);
    const double xScaleFactor = (double)source.GetWidth() / target.GetWidth(), yScaleFactor = (double)source.GetHeight() / target.GetHeight();
    for (int y = 0; y < target.GetHeight(); y++)
        for (int x = 0; x < target.GetWidth(); x++)
        {
            double sourceX = xScaleFactor * x, sourceY = yScaleFactor * y, k = sourceX - floor(sourceX), j = 1 - k,
                   pixelA[2] = {floor(sourceX), floor(sourceY)}, pixelB[2] = {(floor(sourceX)) + 1, floor(sourceY)},
                   pixelC[2] = {floor(sourceX), (floor(sourceY)) + 1}, pixelD[2] = {(floor(sourceX)) + 1, (floor(sourceY)) + 1},
                   y1 = (j * source.Get(pixelA[0], pixelA[1])) + (k * source.Get(pixelB[0], pixelB[1])),
                   y2 = (j * source.Get(pixelC[0], pixelC[1])) + (k * source.Get(pixelD[0], pixelD[1]));
            target(x, y) = (k * y1) + (j * y2);
        }

    target.Save(output);
    return 1;
}

int nearestInterpolation(GrayscaleImage &source, const float xScale, const float yScale, const std::string &output) {
    GrayscaleImage target(source.GetWidth() * xScale, source.GetHeight() * yScale);
    const float xScaleFactor = (float)source.GetWidth() / target.GetWidth(), yScaleFactor = (double)source.GetHeight() / target.GetHeight();
    for (int y = 0; y < target.GetHeight(); y++)
        for (int x = 0; x < target.GetWidth(); x++)
            target(x, y) = source(x * xScaleFactor, y * yScaleFactor);
    target.Save(output);
    return 1;
}

int main() {
    float xScale = 0.5, yScale = 0.5;
    GrayscaleImage source;
    source.Load("../images/barbara.png");
    if (nearestInterpolation(source, xScale, yScale, "../output/nnOutput.png"))
        std::cout << "Image Resized to " << source.GetWidth() * xScale << "x" << source.GetHeight() * yScale << " using Nearest Neighbor." << std::endl;
    else
        std::cout << "Image could not be resized." << std::endl;

    if (bicubicInterpolation(source, xScale, yScale, "../output/BiCubicOutput.png"))
        std::cout << "Image Resized to " << source.GetWidth() * xScale << "x" << source.GetHeight() * yScale << " using Bicubic Interpolation." << std::endl;
    else
        std::cout << "Image could not be resized." << std::endl;

    if (bilinearInterpolation(source, xScale, yScale, "../output/BilinearOutput.png"))
        std::cout << "Image Resized to " << source.GetWidth() * xScale << "x" << source.GetHeight() * yScale << " using Bilinear Interpolation." << std::endl;
    else
        std::cout << "Image could not be resized." << std::endl;

    return 0;
}