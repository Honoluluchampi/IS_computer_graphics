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
  // create light
  auto light = std::make_unique<hnll::HgeActor>();
  auto lightComp = hnll::PointLightComponent::createPointLight
    (light->getId(), 100.f, 0.f, {.1f, 1.f, 1.f});
  lightComp->setTranslation(glm::vec3(0.f, -10.f, 0.f));
  addPointLight(light, lightComp);

  coonsSurface_ = std::make_unique<CoonsSurface>(*this);
}

void IscgApp::updateGame(float dt)
{
  coonsSurface_->updateSurface();
}

} // namespace iscg