#include "../headers/Image.h"
#include <utility>

struct coordinates{
    int x, y;
};

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

bool checkList(std::vector<coordinates> list, int x, int y){
    for(int i = 0; i<list.size(); i++){
        if(list[i].x == x && list[i].y == y) return true;
    }
    return false;
}

void labelCheck(const GrayscaleImage& source, std::vector<coordinates> &regions, std::vector<coordinates> &skip, int x, int y){
    int up = source.Get(x,y+1), down = source.Get(x,y-1), left = source.Get(x-1,y), right = source.Get(x+1,y), center = source.Get(x,y);

    if(center<255) return;
    else {
        if(!checkList(skip, x,y)){
            skip.push_back({x,y});
            regions.push_back({x,y});
        }
        if(down && !checkList(skip, x,y-1)){
            if(!checkList(skip, x,y-1)){
                skip.push_back({x,y-1});
                regions.push_back({x,y-1});
            }
            labelCheck(source, regions, skip, x,y-1);
        }
        // if(left){
        //     point = {x-1,y};
        //     if(!checkList(skip, x-1,y)){
        //         skip.push_back(point);
        //         regions.push_back(point);
        //     }
        //     labelCheck(source, regions, skip, x-1,y);
        // }
        // if(right){
        //     point = {x+1,y};
        //     if(!checkList(skip, x+1,y)){
        //         skip.push_back(point);
        //         regions.push_back(point);
        //     }
        //     labelCheck(source, regions, skip, x+1,y);
        // }

    }
}

std::vector<coordinates> labelRegions(GrayscaleImage &source){
    std::vector<coordinates> regions, open, skipList;
    source = imageToBinary(source, 60, 1);
    // open.push_back({1,1});
    // if(open[0].x == 1 && open[0].y == 1) {
    //     std::cout<<"EXIT NOW"<<std::endl;
    //     return open;
    // }
    for(int yPix = 0; yPix<4; yPix++){
        for(int x=0; x<source.GetWidth(); x++){
            int pixel = source.Get(x,yPix);
            regions.push_back({x, yPix+1});
            std::cout<<"Points: X: " << pixel << " Y:" << regions[x].y << std::endl;
            
        }
    }
    return regions;
}

int main(){
    GrayscaleImage source0, source1, target;
    source0.Load("../../images/barbara.png");
    source1.Load("../../images/Peppers2.png");
    target = imageToBinary(source0, 0, 1);                        // pass 1 for invert in the last argument
    target.Save("../../output/fixedThreshold.png");
    logicalOperation(source0, source1, "../../output/logicalOperation.png", 128);
    std::vector<coordinates> checker = labelRegions(source0);
    // for(int i=0; i<checker.size(); i++){
    //         std::cout<<"Points: X: " << checker[i].x << " Y:" << checker[i].y << std::endl;
    // }
    return 0;
}