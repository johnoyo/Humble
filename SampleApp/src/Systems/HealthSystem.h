#pragma once

namespace HBL 
{
	class HealthSystem final : public ISystem 
	{
	public:
		virtual void Start() override
		{
			FUNCTION_PROFILE();
		}
		virtual void Run(float dt) override
		{
			Registry::Get().View<Component::Health>().ForEach([&](Component::Health& health)
			{
				//std::cout << "Entity health: " << h.health << "\n";
			}).Run();
		}
		virtual void Clear() override { }
	};

}