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
    return QRectF(x + xIdx * xStep, y + yIdx * yStep, width, height);
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

  virtual bool CanContainMultipleRectangles() const { return true; }

  virtual void SetXStep(qreal inStep) { xStep = inStep; }
  virtual qreal GetXStep() const { return xStep; }

  virtual void SetYStep(qreal inStep) { yStep = inStep; }
  virtual qreal GetYStep() const { return yStep; }

  virtual void SetTotalRectangles(int total) { totalRectangles = total; }
  virtual int GetTotalRectangles() const { return totalRectangles; }
  
  virtual void SetRowSize(int size) { rowSize = size; }
  virtual int GetRowSize() const { return rowSize; }

protected:
  qreal xStep;
  qreal yStep;
  int totalRectangles;
  int rowSize;
};

#endif
