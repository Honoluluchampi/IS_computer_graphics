#include <draggable_component.hpp>
#include <drag_manager.hpp>

// std
#include <iostream>

struct ImGui_ImplGlfw_Data;
static ImGui_ImplGlfw_Data* ImGui_ImplGlfw_GetBackendData();


namespace iscg {

DraggableComponent::DraggableComponent(DragManager& owner, Transform& transform) 
  : HgeComponent(), owner_(owner), transform_(transform)
{
  // TODO : add this to owner safely
}

void DraggableComponent::updateComponent(float dt)
{
  if (isBindedToMouse_)
    moveToClickProjection();
}

void DraggableComponent::moveToClickProjection()
{
  
}

} // namespace hnll