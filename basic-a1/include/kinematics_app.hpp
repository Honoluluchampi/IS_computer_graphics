#pragma once

#include <bone.hpp>

// hge
#include <hge_game.hpp>

namespace iscg {

class ktApp : public hnll::HgeGame
{
private:
  void updateGame(float dt) override;
};

} // namespace iscg