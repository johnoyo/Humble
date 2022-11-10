#pragma once

namespace HBL 
{
	class HealthSystem final : public IRegistrySystem 
	{
	public:
		virtual void Start() override
		{
			FUNCTION_PROFILE();
			Filter<Component::Health>();
		}
		virtual void Run(float dt) override
		{
			ForEach([&](IEntity& entt)
			{
				Component::Health& h = Globals::s_Registry.GetComponent<Component::Health>(entt);
				//std::cout << "Entity health: " << h.health << "\n";
			});
		}
		virtual void Clear() override { }
	};

}