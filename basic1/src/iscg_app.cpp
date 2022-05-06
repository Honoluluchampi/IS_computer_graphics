#include <iscg_app.hpp>
#include <drag_manager.hpp>

// hge
#include <hge_components/point_light_component.hpp>

namespace iscg {

IscgApp::IscgApp() : HgeGame("coons surface")
{
    // init camera transform
  Transform transform{};
  transform.translation_m = {0.f, -10.f, -12.f};
  transform.rotation_m = {glm::radians(-45.f), 0.f, 0.f};
  setCameraTransform(transform);

  createActor();
}

void IscgApp::createActor()
{
  coonsSurface_ = std::make_unique<CoonsSurface>(*this);
}

void IscgApp::updateGame(float dt)
{
  coonsSurface_->updateDragManager();
}

} // namespace iscg