#include <kinematics_app.hpp>

namespace iscg {

ktApp::ktApp() : HgeGame("inward kinematics")
{
  // init camera transform
  Transform transform{};
  transform.translation_m = {0.f, 0.f, -10.f};
  setCameraTransform(transform);

  // init drag manager
  dragManager_ = std::make_unique<DragManager>(glfwWindow_m, *upCamera_m);

  // init drag point
  controllPoint_ = std::make_shared<ControllPoint>(glm::vec3{0.f, -5.f, 0.f}, controllPointColor_, controllPointRadius_);
  addPointLightWithoutOwner(controllPoint_->lightComp());
  dragManager_->addDragComps(controllPoint_->dragComp());

  auto lightActor = createActor();
  auto lightComp = hnll::PointLightComponent::createPointLight(lightActor->getId(), 10.f, 0.f, {1.f, 1.f, 1.f});
  lightComp->setTranslation(glm::vec3(0.f, -10.f, 0.f));
  addPointLight(lightActor, lightComp);
  // root
  bones_.emplace_back(createBone());
  // first
  bones_.emplace_back(createBone());
  bones_[1]->getRenderableComponent()->setTranslation(glm::vec3(0.f, 2.f, 0.f));
}

s_ptr<Bone> ktApp::createBone()
{
  auto bone = hnll::HgeGame::createActor<Bone>();
  auto boneModel = getHveModel("bone");
  auto boneModelComp = std::make_shared<hnll::MeshComponent>(bone->getId(), boneModel);
  bone->addRenderableComponent(boneModelComp);

  return bone;
}

void ktApp::updateGame(float dt)
{
  dragManager_->update(0.f);
}

void ktApp::computeIk()
{
  for(auto i = bones_.rbegin(), e = bones_.rend(); i != e; ++i) {

  }
}

void ktApp::updateGameImgui()
{
  ImGui::Button("create bone");
}

} // namespace iscg