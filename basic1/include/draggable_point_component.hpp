#include <hge_component.hpp>

// lib
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

namespace hnll {

class DraggableComponent : HgeComponent
{
  public:
    DraggableComponent(GLFWwindow* window);

    void updateComponent(float dt) override;

  private:
    bool intersectsWithClickProjection();
    void moveToClickProjection();

    // controll mouse-point binding
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    glm::vec3 position_;
    GLFWwindow* window_;
    bool bindedToMouse_ = false;
    static bool isDragging_;
};

} // namespace hnll