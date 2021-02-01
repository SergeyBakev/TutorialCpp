#include "stdafx.h"
#include "GCContext.h"

namespace Common
{
	namespace Graphic
	{
		GCContext::GCContext()
		{

		}

		GCContext::~GCContext()
		{

		}

		void GCContext::Add(GraphicElementPtr el)
		{
			elements_.push_back(el);
		}

		void GCContext::Render()
		{
			for (const auto el : elements_)
				el->Draw();
		}

		void GCContext::Clear()
		{
			elements_.clear();
		}
		void GCContext::ForEach(std::function<void(GraphicElementPtr)> action)
		{
			for (const auto el : elements_)
				action(el);
		}
	}
}
