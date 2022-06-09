#include <memory>
#include <iostream>

class widget
{
  float w = 1.0f;
};

class gadget : public widget
{
  float g = 2.f;
};

int main()
{
  // std::shared_ptr<widget> ptr = std::make_shared<widget>(new gadget());
  // std::shared_ptr<gadget> ptr2 = ptr;

  std::shared_ptr<gadget> ptr3 = std::make_shared<gadget>();
  std::shared_ptr<widget> ptr4 = ptr3;

  std::cout << ptr3.use_count() << " " << ptr4.use_count() << std::endl;
}