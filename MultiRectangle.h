#ifndef _MULTIRECTANGLE_H
#define _MULTIRECTANGLE_H

#include "Rectangle.h"
#include <assert.h>

class MultiRectangle: public Rectangle {
public:
  virtual QRectF ToQRectF(int index = 0) const {
    assert(index < totalRectangles);
    int xIdx = index % rowSize;
    int yIdx = index / rowSize;
    return QRectF(x, y, width + xIdx * xStep, height + yIdx * yStep);
  }

  virtual std::string ToString() const {
    std::stringstream ss;
    ss << x << " " << y << " " << width << " " << height << " " << xStep << " " << yStep << " " << totalRectangles << " " << rowSize;
    return ss.str();
  }

  virtual void FromSplitString(std::vector<std::string>& parts) {
    assert(parts.size() == 4);
    try {
      x = std::stod(parts[0]);
      y = std::stod(parts[1]);
      width = std::stod(parts[2]);
      height = std::stod(parts[3]);
    } catch (...) {
      qDebug() << "Error: Creating a rectangle with more than 4 elements";
    } 
  }

protected:
  qreal xStep;
  qreal yStep;
  int totalRectangles;
  int rowSize;
};

#endif
