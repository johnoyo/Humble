#pragma once

#include "UUID.h"
#include "IEntity.h"
#include "Registry.h"
#include "Components.h"

#include "../HumbleAPI.h"
#include "../Utilities.h"

#include <iostream>
#include <execution>
#include <algorithm>
#include <functional>
#include <unordered_map>

namespace HBL 
{
	class ISystem;

	template<typename T>
	class IView
	{
	public:

		IView(const IView&) = delete;

		static IView& Get()
		{
			static IView instance;
			return instance;
		}

		IView& ForEach(std::function<void(T&)> func)
		{
			m_FunctionView = func;
			return *this;
		}

		void Run()
		{
			for (auto&& [uuid, component] : Registry::Get().GetArray<T>()) 
			{
				m_FunctionView(component);
			}
		}

		void Scedule()
		{
			auto& array = Registry::Get().GetArray<T>();

			std::for_each(
				std::execution::par,
				array.begin(),
				array.end(),
				[&](auto&& item)
				{
					m_FunctionView(item);
				});
		}

	private:
		IView() { };

		std::function<void(T&)> m_FunctionView = nullptr;
	};
}