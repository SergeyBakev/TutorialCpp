#pragma once
#include "Graphic.h"

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
			void Render();
			void Clear();

			void ForEach(std::function<void(GraphicElementPtr)> action);

			bool Empty() const { return elements_.empty(); }

		private:
			std::vector<GraphicElementPtr> elements_;
		};
	}
}