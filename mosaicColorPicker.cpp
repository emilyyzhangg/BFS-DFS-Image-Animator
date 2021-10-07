#include "mosaicColorPicker.h"

MosaicColorPicker::MosaicColorPicker(int mosaicwidth, PNG& inputimage)
{
  width = mosaicwidth;
  img = inputimage;
  mosaicimg = inputimage;
}

HSLAPixel MosaicColorPicker::operator()(point p)
{
  unsigned int left = width * (p.x / width);
  unsigned int upper = width * (p.y / width);

  double avgH = 0;
  double avgS = 0;
  double avgL = 0;
  double avgA = 0;

  int pixelCounter = 0;

  if (upper < img.height() && left < img.width()) {
    for (unsigned int i = upper; i < upper + width && i < img.height(); i++) {
      for (unsigned int j = left; j < left + width && j < img.width(); j++) {
        HSLAPixel *pixel = img.getPixel(j, i);
        avgH += pixel->h;
        avgS += pixel->s;
        avgL += pixel->l;
        avgA += pixel->a;

        pixelCounter++;
      }
    }
  }

  avgH = avgH / pixelCounter;
  avgS = avgS / pixelCounter;
  avgL = avgL / pixelCounter;
  avgA = avgA / pixelCounter; 

  HSLAPixel * pixel = mosaicimg.getPixel(p.x, p.y);

  pixel->h = (double) avgH;
  pixel->s = (double) avgS;
  pixel->l = (double) avgL;
  pixel->a = (double) avgA; 
 
  return * pixel;
}
