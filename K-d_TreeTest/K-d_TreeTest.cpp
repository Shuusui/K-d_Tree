#include "pch.h"
#include "CppUnitTest.h"
#include "Kd_Tree.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace KdTreeTest
{
	TEST_CLASS(KdTreeTest)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Kd_Tree<int> kdTree = Kd_Tree<int>(2);
			Kd_Point<int> basePoint = Kd_Point<int>(2, 5, 7);
			kdTree.Add(basePoint);
		}
	};
}
