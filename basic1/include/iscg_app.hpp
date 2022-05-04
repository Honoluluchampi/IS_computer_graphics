#pragma once

#include <drag_manager.hpp>

// hnll
#include <hge_game.hpp>

// IS computer graphics
namespace iscg {

class IscgApp : public hnll::HgeGame
{
  public:
    IscgApp() : HgeGame("parametric surface")
    {
      createActor();
    }
  private:
    void createActor() override;
    
    u_ptr<hnll::HgeActor> dragManager_;
};

} // namespace iscg