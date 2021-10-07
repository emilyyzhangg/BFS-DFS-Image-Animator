#include "softBorderColorPicker.h"

SoftBorderColorPicker::SoftBorderColorPicker(unsigned int width, PNG& inputimage, double tol)
{
    borderwidth = width;
    img = inputimage;
    tolerance = tol;
}

HSLAPixel SoftBorderColorPicker::operator()(point p)
{
    centre ctr = p.c;
    HSLAPixel *imgPixel;
    HSLAPixel *point = img.getPixel(p.x, p.y);
    HSLAPixel result = *point;

    double avgH = 0;
    double avgA = 0;
    double avgS = 0;
    double avgL = 0;

    bool outsideTol = false;
    int pixelCount = 0;


    for (int x = 0; x < (int) img.width(); x++) {
        for (int y = 0; y < (int) img.height(); y++) {
            if (((p.x - x) * (p.x - x)) + ((p.y - y) * (p.y - y)) <= (borderwidth * borderwidth)) {
                imgPixel = img.getPixel(x, y);

                avgH += imgPixel->h;
                avgS += imgPixel->s;
                avgL += imgPixel->l;
                avgA += imgPixel->a;

                pixelCount++;

                if (ctr.color.dist(*imgPixel) > tolerance) {
                    outsideTol = true;
                }
            }
        }     
    } 


    if (outsideTol) {
        result.h = avgH / pixelCount;
        result.s = avgS / pixelCount;
        result.l = avgL / pixelCount;
        result.a = avgA / pixelCount;
    }
    
    return result;
}

