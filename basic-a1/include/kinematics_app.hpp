#pragma once

#include <bone.hpp>

// hge
#include <hge_game.hpp>

namespace iscg {

class ktApp : public hnll::HgeGame
{
public:
  ktApp();
private:
  s_ptr<Bone> createBone();
  void updateGame(float dt) override;
  void updateGameImgui() override;

  void computeIk();

  std::vector<s_ptr<Bone>> bones_;
};

} // namespace iscg