#ifndef PROFILECPU_H
#define PROFILECPU_H

#include <ProfileInterface.h>
#include <string>

class ProfileCpu : public ProfileInterface {
	
public:

	virtual void StartRegion(const std::string &name);
	virtual void EndRegion();

	virtual ~ProfileCpu() {}

};

#endif