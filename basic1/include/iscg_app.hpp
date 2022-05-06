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

    // create each mid points and move them to bezier curve
    void createMidControllPoints(int pointCount = 4);

    std::vector<s_ptr<ControllPoint>> basePoints_;
    std::vector<u_ptr<BezierCurve>> bezierCurves_;

    u_ptr<DragManager> dragManager_;

    // point param
    glm::vec3 pointColor_ = {.1f, 1.f, 1.f};
    float pointRadius_ = 0.05f;
};

} // namespace iscg