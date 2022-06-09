#pragma once

#include <bone.hpp>
#include <drag_manager.hpp>
// TODO : separate ControllPoint from bezierCurve
#include <bezier_curve.hpp>

// hge
#include <hge_game.hpp>

namespace iscg {

class ktApp : public hnll::HgeGame
{
public:
  ktApp();
private:
  s_ptr<Bone> createBone(s_ptr<Bone> parent = nullptr);
  void updateGame(float dt) override;
  void updateGameImgui() override;

  void addBone();
  void deleteBone();

  void computeIk();

  std::vector<s_ptr<Bone>> bones_;
  s_ptr<Bone> wholeHead_;
  s_ptr<Bone> wholeTail_;

  u_ptr<DragManager> dragManager_;
  s_ptr<ControllPoint> controllPoint_;
  glm::vec3 controllPointColor_ = {.1f, .9f, .1f};
  float controllPointRadius_ = 0.15f;
  bool bindToControllPoint_ = false;
};

} // namespace iscg