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
