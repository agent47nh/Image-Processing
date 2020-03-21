#include "Image.h"

int main(){
    GrayscaleImage source, target(600,600);
    source.Load("read.png");
    float rowFactor= ((source.GetWidth()-1)/target.GetWidth()), colFactor= ((source.GetHeight()-1)/target.GetHeight());
    std::cout<<"Row Factor: " << rowFactor << "\nColumn Factor: " << colFactor << std::endl;
    for(int y=0;y<target.GetHeight();y++)
        for(int x=0;x<target.GetWidth();x++)
            target(x,y) = source(x*rowFactor, y*colFactor);

    target.Save("joshi.png");
    return 0;
}