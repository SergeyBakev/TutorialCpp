#pragma once
#include "Graphic.h"
#include "GBoundingBox.h"

namespace Common
{
	namespace Graphic
	{
		class GCContext
		{
		public:
			GCContext();
			~GCContext();

			void Add(GraphicElementPtr el);
			GraphicElementPtr Remove(GraphicElementPtr el);
			void Render();
			void Clear();
			GBoundingBox GetBBox() const;

			void ForEach(std::function<void(GraphicElementPtr)> action);

			bool Empty() const { return elements_.empty(); }

		private:
			std::vector<GraphicElementPtr> elements_;
		};
	}
}