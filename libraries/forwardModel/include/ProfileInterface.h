#ifndef PROFILEINTERFACE_H
#define PROFILEINTERFACE_H

#include <string>

class ProfileInterface
{
	
public:

  virtual void StartRegion(const std::string &name) = 0;
  virtual void EndRegion() = 0;
 
  virtual ~ProfileInterface() {}

};

#endif
