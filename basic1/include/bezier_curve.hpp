#pragma once

#include <hge_actor.hpp>
#include <draggable_component.hpp>
#include <hge_components/point_light_component.hpp>

namespace iscg {

class ControllPoint : public hnll::HgeActor
{
  public:
    ControllPoint(const glm::vec3& position = {0.f, 0.f, 0.f}, const glm::vec3& color = {.1f, 1.f, 1.f}, float radius = 0.05);
    
    // getter
    s_ptr<DraggableComponent>& dragComp() { return dragComp_; }
    s_ptr<hnll::PointLightComponent>& lightComp() { return lightComp_; }
    glm::vec3& getTranslation() 
    { return lightComp_->getTransform().translation_m; }

  private:
    s_ptr<DraggableComponent> dragComp_;
    s_ptr<hnll::PointLightComponent> lightComp_;
};

class BezierCurve : public hnll::HgeActor
{
  public:
    // takes head and tail (create them before call this ctor)
    BezierCurve(s_ptr<ControllPoint>& head, s_ptr<ControllPoint>& tail);

    template<class CP>
    void addMidControllPoint(CP&& cp)
    { midControllPoints_.emplace_back(std::forward<CP>(cp)); }

    void clearMidControllPoint()
    { midControllPoints_.clear(); }

    void recreateCurve();
    
    // getter
    static int getControllPointCount() { return controllPointCount_; }
    static int getDividingCount() { return dividingCount_; }
    std::vector<glm::vec3>& getLinePointPositions() { return positions_; }

  private:
    void updateActor(float dt) override {}

    // TODO : bind to imgui
    static int controllPointCount_;
    static int dividingCount_;

    // share head and tail with other curves
    s_ptr<ControllPoint> head_;
    s_ptr<ControllPoint> tail_;
    // other than head and tail
    std::vector<s_ptr<ControllPoint>> midControllPoints_;
    // position of the curve point
    std::vector<glm::vec3> positions_;
};

} // namespace hnll