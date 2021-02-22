#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace GUnitTests
{
	using namespace Common;

	

	TEST_CLASS(GBoundingBoxTest) 
	{
	public:
		TEST_CLASS_INITIALIZE(Init)
		{
			int z = 3;
		}
		
		TEST_METHOD(Initialize_Bbox)
		{
			//arrange
			glm::vec3 _min(0, 1, 1);
			glm::vec3 _max(1, 1, 1);
			GBoundingBox bbox;

			//act
			bbox.Min(_min);
			bbox.Max(_max);

			//asset
			Assert::IsTrue(bbox.Min() == _min);
			Assert::IsTrue(bbox.Max() == _max);

		}

		TEST_METHOD(Is_BBox_Valid)
		{
			//arrange
			glm::vec3 _min(0, 0, 0);
			glm::vec3 _max(1, 1, 1);
			GBoundingBox bbox(_min,_max);

			//act
			bool valid = bbox.IsValid();

			//asset
			Assert::IsTrue(valid);
		}

		TEST_METHOD(Is_BBox_Not_Valid)
		{
			//arrange
			glm::vec3 _min(2, 1, 1);
			glm::vec3 _max(1, 1, 1);
			GBoundingBox bbox(_min, _max);

			//act
			bool valid = bbox.IsValid();

			//asset
			Assert::IsFalse(valid);
		}

		TEST_METHOD(Union_BBox_When_Max_More_Than_Src_BBox)
		{
			//arrange
			glm::vec3 _min(0, 0, 0);
			glm::vec3 _max(1, 1, 1);

			glm::vec3 _min2(2, 2, 2);
			glm::vec3 _max2(3, 3, 3);

			GBoundingBox bbox(_min, _max);
			GBoundingBox bbox2(_min2, _max2);

			//act
			bool valid = bbox.Union(bbox2);

			//asset
			Assert::IsTrue(valid);
			Assert::IsTrue(bbox.Min() == _min);
			Assert::IsTrue(bbox.Max() == _max2);
		}

		TEST_METHOD(Union_BBox_When_Min_More_Than_Src_BBox)
		{
			//arrange
			glm::vec3 _min(0, 0, 0);
			glm::vec3 _max(1, 1, 1);

			glm::vec3 _min2(-1, -1, -1);
			glm::vec3 _max2(0, 0, 0);

			GBoundingBox bbox(_min, _max);
			GBoundingBox bbox2(_min2, _max2);

			//act
			bool valid = bbox.Union(bbox2);

			//asset
			Assert::IsTrue(valid);
			Assert::IsTrue(bbox.Min() == _min2);
			Assert::IsTrue(bbox.Max() == _max);
		}

		TEST_METHOD(Union_BBox_When_Max_And_Max_More_Than_Src_BBox)
		{
			//arrange
			glm::vec3 _min(0, 0, 0);
			glm::vec3 _max(1, 1, 1);

			glm::vec3 _min2(-1, -1, -1);
			glm::vec3 _max2(3, 3, 3);

			GBoundingBox bbox(_min, _max);
			GBoundingBox bbox2(_min2, _max2);

			//act
			bool valid = bbox.Union(bbox2);

			//asset
			Assert::IsTrue(valid);
			Assert::IsTrue(bbox.Min() == _min2);
			Assert::IsTrue(bbox.Max() == _max2);
		}
	};
}
