// Basic description of class
#pragma once

namespace basin
{

class Derived : public Base
{
  public:
    Derived();
    void virtualMethod() override;

    type getVariable() const;
    void setVariable(const type& variable);

  private:
    type _variable;
};

}  // basin
