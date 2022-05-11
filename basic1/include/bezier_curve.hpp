#pragma once

#include <draggable_component.hpp>

// hge
#include <hge_actor.hpp>
#include <hge_components/point_light_component.hpp>
#include <hge_components/line_component.hpp>

namespace iscg {

// forward declaration
class IscgApp;

class ControllPoint : public hnll::HgeActor
{
  public:
    ControllPoint(const glm::vec3& position = {0.f, 0.f, 0.f}, const glm::vec3& color = {.1f, 1.f, 1.f}, float radius = 0.05);
    // for creating centroid (create base points first)
    // takes CoonsSurface::basePoints_
    ControllPoint(const std::vector<s_ptr<ControllPoint>>& basePoints, const glm::vec3& color, float radius);
    
    // getter
    s_ptr<DraggableComponent>& dragComp() { return dragComp_; }
    s_ptr<hnll::PointLightComponent>& lightComp() { return lightComp_; }
    glm::vec3& getTranslation() 
    { return lightComp_->getTransform().translation_m; }

  private:
    s_ptr<DraggableComponent> dragComp_;
    s_ptr<hnll::PointLightComponent> lightComp_;
    bool isCentroid_ = false;
};

class BezierCurve : public hnll::HgeActor
{
  public:
    // takes head and tail (create them before call this ctor)
    BezierCurve(s_ptr<ControllPoint>& head, s_ptr<ControllPoint>& tail);

    template<class CP>
    void addMidControllPoint(CP&& cp)
    { midControllPoints_.emplace_back(std::forward<CP>(cp)); }
    void recreateControllLine(IscgApp& app, glm::vec3& color, float radius);

    void clearMidControllPoint() { midControllPoints_.clear(); }
    void clearControllLine() { controllLines_.clear(); }

    void recreateCurve();
    
    // getter
    static int getControllPointCount() { return controllPointCount_; }
    static int getDividingCount() { return dividingCount_; }
    // to bind to imgui
    static int& getControllPointCountRef() { return controllPointCount_; }
    static int& getDividingCountRef() { return dividingCount_; }
    std::vector<glm::vec3>& getLinePointPositions() { return positions_; }
    std::vector<s_ptr<ControllPoint>>& getMidControllPointsRef() { return midControllPoints_; }
    std::vector<s_ptr<hnll::LineComponent>>& getControllLinesRef() { return controllLines_; }

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
    std::vector<s_ptr<hnll::LineComponent>> controllLines_;
    // position of the curve point
    std::vector<glm::vec3> positions_;
};

} // namespace hnll