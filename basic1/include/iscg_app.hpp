#pragma once

#include <drag_manager.hpp>
#include <bezier_curve.hpp>

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
    
    void addControllPoint(s_ptr<ControllPoint>& controllPoint);
    
  private:
    void updateGame(float dt) override;
    void createActor() override;

    u_ptr<DragManager> dragManager_;
};

} // namespace iscg