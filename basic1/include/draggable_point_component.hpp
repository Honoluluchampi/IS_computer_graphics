#include <hge_component.hpp>

// lib
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

namespace hnll {

class DraggableComponent : HgeComponent
{
  public:
    DraggableComponent(GLFWwindow* window) : HgeComponent(), window_(window)
    {}

    void updateComponent(float dt) override;
    
  private:
    glm::vec3 position_;
    GLFWwindow* window_;
};

} // namespace hnll