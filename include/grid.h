#ifndef GRID_H
#define GRID_H

template <class T>
class Grid {

public:

  virtual ~Grid() {};

  virtual int GetNumberOfGridPoints() const =0;
  virtual T GetCellVolume() const =0;

};

#endif
