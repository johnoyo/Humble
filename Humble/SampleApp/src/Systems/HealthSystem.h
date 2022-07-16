#pragma once

namespace HBL 
{
	class HealthSystem final : public ISystem 
	{
	public:
		virtual void Start() override
		{
			FUNCTION_PROFILE();
			Filter(Globals::entities, "Health");
		}
		virtual void Run() override
		{
			For_Each([&](IEntity& entt)
			{
				Component::Health& h = GET_MY_COMPONENT(Health, entt);
				//std::cout << "Entity health: " << h.health << "\n";
			});
		}
		virtual void Clear() override { }
	};

}