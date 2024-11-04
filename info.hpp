#ifndef INFO_HPP
#define INFO_HPP
#include "commands.hpp"
#include <string>

class info: public Command{
  private:
    std::string query;
    
  public:
    int init (std::string query);
    int execute () override;
    void display() override;
};

#endif
