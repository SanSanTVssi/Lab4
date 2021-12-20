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

	auto instance2 = universalStrign<char>(text, text + 3);
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
	instance.push_front('C');
	EXPECT_EQ('C', instance[0]);
	EXPECT_EQ('B', instance[1]);
	EXPECT_EQ('A', instance[2]);

	auto instance2 = universalStrign<char>();
	instance2.push_back(instance);
	EXPECT_TRUE(instance == instance2);
}

TEST(universalStrign_pop_functions, pop_back_and_front) {
	auto instance = universalStrign<char>();
	instance.push_back('A');
	instance.push_back('B');
	instance.push_back('C');
	instance.pop_back();
	EXPECT_THROW(instance[2], std::exception);
	EXPECT_EQ(instance[1], 'B');
	instance.push_back('C');
	instance.pop_front();
	EXPECT_EQ('B', instance[0]);
	EXPECT_EQ('C', instance[1]);
	instance.pop_front();
	EXPECT_EQ('C', instance[0]);
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
	auto to_compare = universalStrign<char>("TextTextText");
	instance.transform([](char value)->char {
		return value + 2;
		});
	for (size_t i = 0; i < instance.size(); i++)
	{
		EXPECT_EQ(to_compare[i] + 2, instance[i]);
	}

	struct otherFunctor {
		char operator()(char value) { return value + 3; }
	};
	instance.transform<otherFunctor>();
	for (size_t i = 0; i < instance.size(); i++)
	{
		EXPECT_EQ(to_compare[i] + 5, instance[i]);
	}
}

TEST(universalStrign_Functor, dynamic_polimorf_in_place) {
	auto instance = universalStrign<char>("TextTextText");
	auto to_compare = universalStrign<char>("TextTextText");

	struct otherFunctor : public ITransformer<char> {
	public:
		char operator()(char value) { return value + 3; }
	} functor;

	instance.transformDyn(&functor);
	for (size_t i = 0; i < instance.size(); i++)
	{
		EXPECT_EQ(to_compare[i] + 3, instance[i]);
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

TEST(universalStrign_compare, equal) {
	auto instance = universalStrign<char>("TextTextText");
	auto instance2 = universalStrign<char>("TextTextText");
	auto instance3 = universalStrign<char>("TextTextTextText");
	auto instance4 = universalStrign<char>("Text");
	EXPECT_TRUE(instance == instance2);
	EXPECT_FALSE(instance == instance3);
	EXPECT_FALSE(instance == instance4);
}

TEST(universalStrign_compare, not_equal) {
	auto instance = universalStrign<char>("TextTextText");
	auto instance2 = universalStrign<char>("TextTextText");
	auto instance3 = universalStrign<char>("TextTextTextText");
	auto instance4 = universalStrign<char>("Text");
	EXPECT_FALSE(instance != instance2);
	EXPECT_TRUE(instance != instance3);
	EXPECT_TRUE(instance != instance4);
}

TEST(universalStrign_compare, bigger) {
	auto instance = universalStrign<char>("ABC");
	auto instance2 = universalStrign<char>("BCD");
	EXPECT_TRUE(instance2 >= instance);
	EXPECT_TRUE(instance2 > instance);
}

TEST(universalStrign_compare, less) {
	auto instance = universalStrign<char>("ABC");
	auto instance2 = universalStrign<char>("BCD");
	EXPECT_TRUE(instance <= instance2);
	EXPECT_TRUE(instance < instance2);
}

TEST(universalStrign, concate) {
	auto instance = universalStrign<char>("ABC");
	auto instance2 = universalStrign<char>("BCD");
	auto compare = universalStrign<char>("ABCBCD");
	auto compare2 = universalStrign<char>("ABCA");
	auto compare3 = universalStrign<char>("AABC");
	auto result = instance + instance2;
	auto result2 = instance + 'A';
	auto result3 = 'A' + instance;
	EXPECT_TRUE(result == compare);
	EXPECT_TRUE(result2 == compare2);
	EXPECT_TRUE(result3 == compare3);
}

TEST(universalStrign, multiply) {
	auto instance = universalStrign<char>("ABC") * 3;
	universalStrign<char> result = "ABCABCABC";
	EXPECT_TRUE(result == instance);
}

TEST(universalStrign, make_string) {
	auto arr = "TEXTtext";
	auto str1 = make_string(arr);
	universalStrign<char> to_compare1 = arr;
	universalStrign<char> to_compare2 = universalStrign<char>(arr, arr+4);
	EXPECT_TRUE(str1 == to_compare1);
	auto str2 = make_string(arr, arr+4);

}

TEST(universalStrign, input_output_from_console) {
	auto instance = universalStrign<char>("ABC_");
	std::cin >> instance;
	std::cout << instance << std::endl;
}

TEST(universalStrign, convert) {
	universalStrign<wchar_t> instance (L"ABC_");
	auto instance_char = convert<char, wchar_t>(instance);
	std::cout << instance_char << std::endl;
	std::cout << typeid(instance_char).name() << std::endl;
}

TEST(universalStrign, stringOfstring_test) {
	universalStrign<char> instance("ABC_");
	universalStrign<universalStrign<char>> instanceStr;
	universalStrign<universalStrign<char>> instanceStr2(instance);
	instanceStr.push_back(instance);
	instanceStr.push_back(instance);
	instanceStr2.push_back(instance);
	EXPECT_EQ(instanceStr.size(), 2);
	EXPECT_TRUE(instanceStr == instanceStr2);
}