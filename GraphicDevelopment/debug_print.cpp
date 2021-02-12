#include "stdafx.h"




void print(glm::mat4& m)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            std::cout << m[i][j] << "\t\t";
        }
        std::cout << std::endl;
    }
}

void print(glm::vec4& vec)
{
    for (int i = 0; i < 4; i++)
    {
        std::cout << vec[i] << "\t\t";
    }
}
void print(glm::vec3& vec)
{
    for (int i = 0; i < 3; i++)
    {
        std::cout << vec[i] << "\t\t";
    }
}