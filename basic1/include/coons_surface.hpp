#pragma once

#include <bezier_curve.hpp>
#include <drag_manager.hpp>

// hge
#include <hge_actor.hpp>

namespace iscg {

// forward declaration
class IscgApp;

class CoonsSurface : hnll::HgeActor
{
  public:
    CoonsSurface(IscgApp& iscgApp);

    inline void updateDragManager()
    { dragManager_->update(0.f); }

  private:
    IscgApp& iscgApp_;
    u_ptr<DragManager> dragManager_;

    std::vector<u_ptr<BezierCurve>> bezierCurves_;
    std::vector<s_ptr<ControllPoint>> basePoints_;

    // create each mid points and move them to bezier curve
    void createMidControllPoints(int pointCount = 4);
    void addControllPoint(s_ptr<ControllPoint>& controllPoint);

    // point param
    glm::vec3 pointColor_ = {1.f, .1f, .1f};
    float pointRadius_ = 0.05f;

    // curves param
    // number of vertices between two controll point
    int dividingCount_ = 10;
};

} // namespace