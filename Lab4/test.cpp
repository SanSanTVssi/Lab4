#include "pch.h"
#include <vld.h>

using namespace my_std;
using namespace std;

TEST(universalStrign_construct, construct_by_rvalue) {
	auto instance = universalStrign<wchar_t>(10, 'A');
	auto instance2 = std::move(instance);
	EXPECT_EQ(instance.size(), 0);
	EXPECT_EQ(instance2.size(), 10);
}

TEST(universalStrign_construct, construct_by_emptyValues) {
	auto instance = universalStrign<wchar_t>(std::size_t(10));
	EXPECT_EQ(instance.size(), 10);
}

TEST(universalStrign_construct, construct_by_char_arr) {
	const char* text = "TestTest";
	auto instance = universalStrign<char>(text);
	for (int i = 0; i < instance.size(); i++)
	{
		EXPECT_EQ(instance[i], text[i]);
	}

	auto instance2 = universalStrign<char>(text, text + 4);
	EXPECT_EQ(instance2.size(), 4);
	for (int i = 0; i < instance.size(); i++)
	{
		EXPECT_EQ(instance[i], text[i]);
	}
}

TEST(universalStrign_simple_functions, is_string_empty) {
	auto instance = universalStrign<char>();
	EXPECT_TRUE(instance.isEmpty());
}

TEST(universalStrign_push_functions, push_back_and_front) {
	auto instance = universalStrign<char>();
	instance.push_back('A');
	EXPECT_EQ('A', instance[0]);
	instance.push_front('B');
	EXPECT_EQ('B', instance[0]);
}

TEST(universalStrign_pop_functions, pop_back_and_front) {
	auto instance = universalStrign<char>();
	instance.push_back('A');
	instance.push_back('B');
	instance.pop_back();
	EXPECT_THROW(instance[1], std::exception);
	instance.push_back('B');
	instance.pop_front();
	EXPECT_EQ('B', instance[0]);
}

TEST(universalStrign_pop_functions, clear) {
	auto instance = universalStrign<char>("TextText");
	instance.clear();
	EXPECT_TRUE(instance.isEmpty());
	EXPECT_EQ(instance.size(), 0);
}

TEST(universalStrign_complicative_functions, splits) {
	auto instance = universalStrign<char>("TextTextText");
	auto instance2 = instance.split(4);
	for (size_t i = 0; i < instance2.size(); i++)
	{
		EXPECT_EQ(instance2[i], instance[i + 4]);
	}
}

TEST(universalStrign_Functor, static_polimorf_in_place) {
	auto instance = universalStrign<char>("TextTextText");
	auto instance_temp = instance;
	instance.transform([](char value)->char {
		return value + 2;
		});
	for (size_t i = 0; i < instance.size(); i++)
	{
		EXPECT_EQ(instance_temp[i] + 2, instance[i]);
	}

	struct otherFunctor {
		char operator()(char value) { return value + 3; }
	};
	instance.transform<otherFunctor>();
	for (size_t i = 0; i < instance.size(); i++)
	{
		EXPECT_EQ(instance_temp[i] + 5, instance[i]);
	}
}

TEST(universalStrign_Functor, dynamic_polimorf_in_place) {
	auto instance = universalStrign<char>("TextTextText");
	auto instance_temp = instance;

	struct otherFunctor : public ITransformer<char> {
	public:
		char operator()(char value) { return value + 3; }
	} functor;

	instance.transformDyn(&functor);
	for (size_t i = 0; i < instance.size(); i++)
	{
		EXPECT_EQ(instance_temp[i] + 3, instance[i]);
	}
}

TEST(universalStrign_Functor, static_polimorf_new_string) {
	auto instance = universalStrign<char>("TextTextText");
	auto instance2 = transform(instance, 
		[](char value)->char {
			return value + 2;
		});
	for (size_t i = 0; i < instance.size(); i++)
	{
		EXPECT_EQ(instance[i] + 2, instance2[i]);
	}

	struct otherFunctor {
		char operator()(char value) { return value + 3; }
	};
	instance2 = transform<otherFunctor>(instance);
	for (size_t i = 0; i < instance.size(); i++)
	{
		EXPECT_EQ(instance2[i], instance[i] + 3);
	}
}

TEST(universalStrign_Functor, dynamic_polimorf_new_string) {
	auto instance = universalStrign<char>("TextTextText");

	struct otherFunctor : public ITransformer<char> {
		char operator()(char value) { return value + 3; }
	} functor;

	auto instance2 = transformDyn(instance, &functor);
	for (size_t i = 0; i < instance.size(); i++)
	{
		EXPECT_EQ(instance2[i], instance[i] + 3);
	}
}