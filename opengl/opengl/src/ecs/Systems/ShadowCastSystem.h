#pragma once
#include "../Header.h"
#include "../Utilities.h"
#include "RenderingSystem.h"

 /*
  * Let O be the position of the player
  * Let E be the vertex position of the current quad
  * Let R be the radious the casting
  * base_ang = atan2f(E.y - O.y, E.x - O.x)
  * shadow = ( R * cos(base_ang), R * sin(base_ang))
  *
 */

class ShadowCastSystem {
public:
    void Start(glm::vec4 shadow_color, glm::vec3 player_position, VertexBuffer& buffer, RenderingSystem& rend);
    void Run(glm::vec3 player_position, VertexBuffer& buffer, RenderingSystem& rend);
    void Clear();
};
