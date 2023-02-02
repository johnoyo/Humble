#pragma once

#include "../Utilities.h"
#include "../HumbleAPI.h"
#include "../ECS/ISystem.h"
#include "../ECS/IEntity.h"
#include "../ECS/Registry.h"
#include "../ECS/Components.h"

#include "../Managers/SceneManager.h"

 /*
  * Let O be the position of the player
  * Let E be the vertex position of the current quad
  * Let R be the radious the casting
  * base_ang = atan2f(E.y - O.y, E.x - O.x)
  * shadow = ( R * cos(base_ang), R * sin(base_ang))
  *
 */

namespace HBL 
{
    class HBL_API ShadowCastSystem final : public ISystem
    {
    public:
        virtual void Start() override;
        virtual void Run(float dt) override;
        virtual void Clear() override;
    };

}
