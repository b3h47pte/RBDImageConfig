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
    assert(parts.size() >= 8);
    try {
      Rectangle::FromSplitString(parts);
      xStep = std::stod(parts[4]);
      yStep = std::stod(parts[5]);
      totalRectangles = std::stoi(parts[6]);
      rowSize = std::stoi(parts[7]);
    } catch (...) {
      qDebug() << "Error: Creating a multi-rectangle.";
    } 
  }

protected:
  qreal xStep;
  qreal yStep;
  int totalRectangles;
  int rowSize;
};

#endif
