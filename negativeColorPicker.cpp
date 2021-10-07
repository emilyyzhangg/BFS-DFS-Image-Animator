#include "negativeColorPicker.h"

NegativeColorPicker::NegativeColorPicker(PNG& inputimg)
{
  img = inputimg;
}

HSLAPixel NegativeColorPicker::operator()(point p)
{
  
  HSLAPixel * pixel = img.getPixel(p.x, p.y);
  double hue = ((int) pixel->h + 180) % 360;
  double sat = pixel->s;
  double lum = 1 - pixel->l;
    
  HSLAPixel result = *pixel;

  result.h = hue;
  result.s = sat;
  result.l = lum;
  result.a = 1.0;
  
  return result;
}
