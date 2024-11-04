#ifndef COMMANDS_HPP
#define COMMANDS_HPP
class Command {
  public:
    virtual int execute()  = 0;
    virtual void display() = 0;
};

#endif