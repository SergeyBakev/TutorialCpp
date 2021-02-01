#pragma once
namespace Common
{
	namespace Mathematic
	{
		float DotProduct(const glm::vec3& v1, const glm::vec3& v2);

		float ModuleOfVector(const glm::vec3& v);

		float AngleBeetwnVector(const glm::vec3& v1, const glm::vec3& v);

		glm::vec3 MakeVector(const glm::vec3& start, const glm::vec3& end);


		//glm::vec3 ChangeBasis(const glm::vec3& v, const glm::vec3& e1, const glm::vec3& e2, const glm::vec3& e3)
		//{   
		//    Eigen::Matrix3f matrix;
		//    matrix.setZero();
		//    size_t offset = sizeof(float) * 3;
		//    const float* p1 = glm::value_ptr(e1);
		//    memcpy(matrix.data(), p1, offset);
		//    const float* p2 = glm::value_ptr(e2);
		//    memcpy(matrix.data() + 3, p2, offset);
		//    const float* p3 = glm::value_ptr(e3);
		//    memcpy(matrix.data() + 2 * 3, p3, offset);
		//    if (matrix.determinant() != 0)
		//    {
		//        Eigen::Vector3f a;
		//        a.setZero();
		//        const float* p4 = glm::value_ptr(v);
		//        memcpy(a.data(), p4, offset);
		//        std::cout << a << std::endl;
		//        Eigen::Vector3f solve = matrix.colPivHouseholderQr().solve(a);
		//        std::cout << solve << std::endl;
		//        glm::vec3 ret(0);
		//        memcpy(solve.data(), glm::value_ptr(ret), 3);
		//        return ret;
		//    }
		//
		//    return{};
		//}
	}
}