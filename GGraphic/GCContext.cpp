#include "pch.h"
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

		GraphicElementPtr GCContext::Remove(GraphicElementPtr el)
		{
			if (el == nullptr)
				return nullptr;

			auto it = std::remove_if(std::begin(elements_), std::end(elements_), [&el](const GraphicElementPtr& shape)
				{
					if (el.get() == shape.get())
						return true;
					return false;
				}
			);

			if (it != std::end(elements_))
				elements_.erase(it, std::end(elements_));
			
			return el;
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
		GBoundingBox GCContext::GetBBox() const
		{
			GBoundingBox bbox;
			for (const auto& el : elements_)
			{
				GBoundingBox bb = el->GetBBox();
					if(bb.IsValid())
						bbox.Union(bb);
			}
			return bbox;
		}
		void GCContext::ForEach(std::function<void(GraphicElementPtr)> action)
		{
			for (const auto el : elements_)
				action(el);
		}
	}
}
