#include <gtest/gtest.h>
#include <funzel/Tensor.hpp>

using namespace funzel;

#if 0
TEST(BLASTensor, Test)
{
	auto ones = Tensor::ones({3, 3, 3});

	ones[2][2][2].ritem<float>() = 5;
	auto f = ones[0][0][0].item<float>();


	f = ones[2][2][2].item<float>();

	std::cout << "Value: " << f << std::endl;
}
#endif

TEST(CPUTensorTest, PrintDevices)
{
	funzel::PrintDevices();
}

TEST(CPUTensorTest, Init)
{
	auto ones = Tensor::ones({3, 3, 3});
	EXPECT_EQ(ones.strides, Shape({36, 12, 4}));
	EXPECT_EQ(ones.shape, Shape({3, 3, 3}));
}

TEST(CPUTensorTest, ToString)
{
	auto ones = Tensor::ones({20, 1});
	std::cout << ones << std::endl;

	ones = Tensor::ones({20, 20});
	std::cout << ones << std::endl;
}

TEST(CPUTensorTest, Transpose)
{
	auto v = Tensor::zeros({3, 3});

	v[{1, 0}] = 1;

	EXPECT_EQ((v[{1, 0}].item<float>()), 1.0f);
	EXPECT_EQ(v.strides, Shape({12, 4}));
	EXPECT_EQ(v.shape, Shape({3, 3}));

	v = v.transpose();

	EXPECT_EQ((v[{0, 1}].item<float>()), 1.0f);
	EXPECT_EQ(v.strides, Shape({4, 12}));
	EXPECT_EQ(v.shape, Shape({3, 3}));
}

TEST(CPUTensorTest, Index)
{
	auto ones = Tensor::ones({3, 3, 3});

	EXPECT_EQ(0, ones.offset);
	EXPECT_EQ(0, ones[0].offset);
}

TEST(CPUTensorTest, Unravel)
{
	auto v = Tensor::zeros({3, 3});
	v[{1, 0}] = 1;

	auto vtrans = v.transpose();
	EXPECT_TRUE(!vtrans.isContiguous());
	EXPECT_TRUE(vtrans.unravel().isContiguous());
}

TEST(CPUTensorTest, Reshape)
{
	auto v = Tensor::zeros({3, 3});
	v[{1, 0}] = 1;

	EXPECT_THROW(v.reshape({27, 2}), std::runtime_error);

	v.reshape_({9});

	EXPECT_EQ(v.shape, Shape{9});
}

TEST(CPUTensorTest, Assign)
{
	Tensor a = Tensor::zeros({3, 3});
	Tensor b = Tensor::ones(3);


	Tensor subset = a[1];
	a[1].set(b);
	
	std::cout << a.cpu() << std::endl;
}

TEST(CPUTensorTest, Linspace)
{
	Tensor a = funzel::linspace(Tensor({3}, {0.0f, 0.0f, 0.0f}), Tensor({3}, {1.0f, 2.0f, 3.0f}), 50);
	std::cout << a << std::endl;
}

TEST(CPUTensorTest, TrimDimensions)
{
	Tensor a = Tensor::empty({3, 1, 1, 3, 1, 1});

	a.trimDimensions();
	EXPECT_EQ(a.shape, (Shape({3, 1, 3, 1})));
}

#define CommonTest TensorTest
#define TestDevice ""
#include "../../Common.hpp"
