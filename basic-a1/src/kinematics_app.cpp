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
  controllPoint_ = std::make_shared<ControllPoint>(glm::vec3{0.f, 0.f, 0.f}, controllPointColor_, controllPointRadius_);
  addPointLightWithoutOwner(controllPoint_->lightComp());
  dragManager_->addDragComps(controllPoint_->dragComp());

  auto lightActor = createActor();
  auto lightComp = hnll::PointLightComponent::createPointLight(lightActor->getId(), 10.f, 0.f, {1.f, 1.f, 1.f});
  lightComp->setTranslation(glm::vec3(0.f, -10.f, 0.f));
  addPointLight(lightActor, lightComp);
  // root
  bones_.emplace_back(createBone());
  wholeTail_ = bones_[0];
  // first
  bones_.emplace_back(createBone(bones_[0]));
  wholeHead_ = bones_[1];
}

s_ptr<Bone> ktApp::createBone(s_ptr<Bone> parent)
{
  auto bone = hnll::HgeGame::createActor<Bone>(parent);
  auto boneModel = getHveModel("bone");
  auto boneModelComp = std::make_shared<hnll::MeshComponent>(bone->getId(), boneModel);
  bone->addRenderableComponent(boneModelComp);

  if (parent)
    bone->alignToParent();

  return bone;
}

void ktApp::updateGame(float dt)
{
  dragManager_->update(0.f);

  static bool changedCache = false;
  if (dragManager_->isChanged()) changedCache = true;
  if (bindToControllPoint_ && changedCache) {
    computeIk();
    changedCache = false;
  }
}

void ktApp::computeIk()
{
  for(auto i = bones_.rbegin(), e = bones_.rend(); i != e; ++i) {
    (*i)->updateIk(controllPoint_->getTranslation(), wholeHead_->head());
  }
}

void ktApp::updateGameImgui()
{
  ImGui::Begin("config");

  if (ImGui::Button("create bone")) {
    addBone();
  }
  if (ImGui::Button("delete bone")) {
    // deleteBone();
  }
  ImGui::Checkbox("bind to controll point", &bindToControllPoint_);

  ImGui::End();
}

void ktApp::addBone()
{

}

void ktApp::deleteBone()
{
  if (bones_.size() == 1)
    std::cout << "cannot delete root!" << std::endl;
  else {
    bones_[bones_.size() - 1]->setActorState(hnll::HgeActor::state::DEAD);
    bones_.erase(bones_.end() - 1);
  }
}

} // namespace iscg