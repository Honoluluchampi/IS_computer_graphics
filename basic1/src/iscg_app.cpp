#include <iscg_app.hpp>
#include <draggable_component.hpp>

namespace iscg {
void IscgApp::createActor()
{
  auto actor = std::make_unique<hnll::HgeActor>();
  actor->addUniqueComponent(std::make_unique<hnll::DraggableComponent>(this->glfwWindow_m));
  addActor(actor);
}
} // namespace iscg