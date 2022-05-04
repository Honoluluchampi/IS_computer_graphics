// hge
#include <hge_component.hpp>

// lib
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

namespace hnll {

class DraggableComponent : public HgeComponent
{
  public:
    DraggableComponent(GLFWwindow* window);

  private:
    void updateComponent(float dt) override;

    bool intersectsWithClickProjection();
    void moveToClickProjection();

    // controll mouse-point binding
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    GLFWwindow* window_;
    bool bindedToMouse_ = false;
    static bool isDragging_;

    glm::vec3 position_;
    float radius = 0.3;
};

} // namespace hnll