#pragma once
#include <memory>
namespace Common
{
	namespace Graphic
	{
		namespace detail
		{
			template <class T> std::shared_ptr<T> gobject_to_ptr(T& obj)
			{
				return std::make_shared<T>(obj);
			}
		}
	}
}
