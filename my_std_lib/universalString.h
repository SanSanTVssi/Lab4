#pragma once

#include <vector>
#include <algorithm>
#include <iostream>
#include <concepts>
#include <functional>
#include <string>
#include "framework.h"

namespace my_std {

	template <class charT>
	using DefaultContainer = std::vector<charT>;

	template <class value>
	class ITransformer {
	public:
		virtual value operator()(value) = 0;
	};

	template <class charT, class Container = DefaultContainer<charT>>
	class universalStrign {
		template <class value>
		class defaultTransformer : public ITransformer<value> {
		private:
			std::function<value(value)> _functor;

		public:
			defaultTransformer(std::function<value(value)> Functor) : _functor(Functor) {}

			value operator()(value val) override { return _functor(val); }
		};

		bool static Comparator(universalStrign<charT>& str1, universalStrign<charT>& str2,
			std::function<bool(charT, charT)> predicat) {
			std::size_t size = std::min(str1.size(), str2.size()) + 1;
			for (size_t i = 0; i < size; i++)
			{
				if (!predicat(str1.data[i], str2.data[i])) {
					return false;
				}
			}
			return true;
		}

	public:
		universalStrign() : _size(0), data() { data.push_back(charT()); }

		universalStrign(charT value) : universalStrign() { 
			universalStrign::push_back(value);
		}

		[[maybe_unused]] universalStrign(std::size_t size, charT value = charT()) : universalStrign() {
			for (std::size_t i = 0; i < size; i++)
			{
				universalStrign<charT>::push_back(value);
			}
		}

		universalStrign(universalStrign&) = default;

		universalStrign(universalStrign&&) noexcept;

		universalStrign(const charT*);

		universalStrign(const charT*, const charT*);

		template <class OtherCharT>
		universalStrign(universalStrign<OtherCharT>&);

		universalStrign& operator=(universalStrign&) = default;

		universalStrign operator=(universalStrign&&) noexcept;

		~universalStrign() { clear(); }

		std::size_t size() const { return _size; }

		bool isEmpty() const  { return !_size; }

		void pop_front();

		void pop_back();

		void push_back(charT);

		void push_back(const universalStrign&);

		void push_front(charT);

		void clear() { data.clear(); _size = 0; }

		charT& operator[](std::size_t);

		charT operator[](std::size_t) const;

		universalStrign split(std::size_t index) const {
			if (index < _size) {
				auto result = universalStrign<charT>();
				for (size_t i = index; i < _size; i++)
				{
					result.push_back(data[i]);
				}
				return universalStrign(result);
			}
			else {
				throw std::out_of_range("Out of range error [universalStrign<charT> universalStrign<charT>::split]");
			}
		}
		
		friend universalStrign operator+(universalStrign string1, universalStrign string2) {
			auto result = universalStrign<charT>();
			for (size_t i = 0; i < string1.size(); i++)
			{
				result.push_back(string1[i]);
			}
			for (size_t i = 0; i < string2.size(); i++)
			{
				result.push_back(string2[i]);
			}
			return universalStrign(result);
		}

		friend universalStrign operator*(universalStrign string, std::size_t times) {
			auto result = string;

			for (size_t i = 0; i < times - 1; i++)
			{
				result.push_back(string);
			}
			return universalStrign(result);
		}

		friend bool operator==(universalStrign& string1, universalStrign& string2) {
			auto equal = [](charT val1, charT val2)->bool { return val1 == val2; };
			return universalStrign<charT>::Comparator(string1, string2, equal);
		}

		friend bool operator!=(universalStrign& string1, universalStrign& string2) {
			return !operator==(string1, string2);
		}

		friend bool operator>=(universalStrign& string1, universalStrign& string2) {
			auto bigger_or_equal = [](charT val1, charT val2)->bool { return val1 >= val2; };
			return universalStrign<charT>::Comparator(string1, string2, bigger_or_equal);
		}

		friend bool operator>(universalStrign& string1, universalStrign& string2) {
			return operator>=(string1, string2) && operator!=(string1, string2);
		}

		friend bool operator<=(universalStrign& string1, universalStrign& string2) {
			return !operator>(string1, string2);
		}

		friend bool operator<(universalStrign& string1, universalStrign& string2) {
			return !operator>=(string1, string2);
		}

		template <class Functor = defaultTransformer<charT>>
		void transform(Functor functor = Functor()) {
			for (size_t i = 0; i < _size; i++)
			{
				data[i] = functor(data[i]);
			}
		}

		void transformDyn(ITransformer<charT>* functor) {
			for (size_t i = 0; i < _size; i++)
			{
				data[i] = functor->operator()(data[i]);
			}
		}

		template <class Functor = defaultTransformer<charT>>
		friend universalStrign transform(universalStrign& other, Functor functor = Functor()) {
			auto result = universalStrign<charT>();
			for (size_t i = 0; i < other.size(); i++)
			{
				result.push_back(functor(other[i]));
			}
			return universalStrign(result);
		}

		friend universalStrign transformDyn(universalStrign& other, 
			ITransformer<charT>* functor) {
			auto result = universalStrign<charT>();
			for (size_t i = 0; i < other.size(); i++)
			{
				result.push_back(functor->operator()(other[i]));
			}
			return universalStrign(result);
		}

		friend std::istream& operator>>(std::istream& input, universalStrign& value) {
			std::string temp;
			try {
				input >> temp;
			}
			catch (const std::exception& ex) {
				throw ex;
			}
			for (auto item : temp) {
				value.push_back(static_cast<charT>(item));
			}
			return input;
		}

		friend std::ostream& operator<<(std::ostream& out, universalStrign& value) {
			for (size_t i = 0; i < value.size(); i++)
			{
				try {
					out << value[i];
				}
				catch (const std::exception& ex) {
					throw ex;
				}
			}
			return out;
		}

	private:
		std::size_t _size;
		Container data;
	};

	template<class charT, class Container>
	inline universalStrign<charT, Container>::universalStrign(universalStrign&& other) noexcept
		: _size(other._size), data(std::move(other.data)) { other._size = 0; }

	template<class charT, class Container>
	universalStrign<charT, Container>::universalStrign(const charT* Array) : universalStrign()
	{
		for (std::size_t i = 0; Array[i] != 0; i++)
		{
			universalStrign<charT, Container>::push_back(Array[i]);
		}
	}

	template<class charT, class Container>
	universalStrign<charT, Container>::universalStrign(const charT* Array, const charT* ArrayEnd) : universalStrign()
	{
		for (std::size_t i = 0; Array + i != ArrayEnd; i++) {
			universalStrign<charT>::push_back(Array[i]);
		}
		universalStrign<charT>::push_back(*ArrayEnd);
	}

	template<class charT, class Container>
	universalStrign<charT, Container> universalStrign<charT, Container>::operator=(universalStrign<charT, Container>&& other) noexcept
	{
		_size = other._size;
		data = std::move(other.data);
		other._size = 0;
		return *this;
	}

	template<class charT, class Container>
	void universalStrign<charT, Container>::pop_front()
	{
		if (_size) {
			std::size_t size = _size;
			charT temp = data[0];
			std::size_t i;
			for (i = 0, size--; i < size; i++) data[i] = data[i + 1];
			data[size] = temp;
			universalStrign<charT>::pop_back();
		}
	}

	template<class charT, class Container>
	void universalStrign<charT, Container>::pop_back()
	{
		if (_size) {
			data.pop_back();
			data.pop_back();
			data.push_back(charT());
			_size--;
		}
	}

	template<class charT, class Container>
	void universalStrign<charT, Container>::push_back(charT value)
	{
		data[_size] = value;
		data.push_back(charT());
		_size++;
	}

	template<class charT, class Container>
	void universalStrign<charT, Container>::push_back(const universalStrign<charT, Container>& other)
	{
		data.pop_back();
		for (size_t i = 0; i < other.size(); i++)
		{
			data.push_back(other.data[i]);
		}
		data.push_back(charT());
		_size += other.size();
	}

	template<class charT, class Container>
	void universalStrign<charT, Container>::push_front(charT value)
	{
		std::size_t size = _size;
		charT temp = data[--size];
		while (size > 0) {
			data[size--] = data[size - 1];
		}
		universalStrign<charT>::push_back(temp);
		data[0] = value;
	}

	template<class charT, class Container>
	charT& universalStrign<charT, Container>::operator[](std::size_t index)
	{
		if (index < _size) {
			return data[index];
		}
		else {
			throw std::out_of_range("Out of range error [universalStrign<charT>::operator[]]");
		}
	}
	template<class charT, class Container>
	charT universalStrign<charT, Container>::operator[](std::size_t index) const
	{
		if (index < _size) {
			return data[index];
		}
		else {
			throw std::out_of_range("Out of range error [universalStrign<charT>::operator[]]");
		}
	}

	template<class charT, class Container = DefaultContainer<charT>>
	universalStrign<charT, Container> make_string(const charT* value) { return universalStrign<charT, Container>(value); }

	template<class charT, class Container = DefaultContainer<charT>>
	static universalStrign<charT, Container> make_string(const charT* begin, const charT* end) { return universalStrign<charT, Container>(begin, end); }

	template <class T, class U>
	universalStrign<T> convert(universalStrign<U>& str) {
		universalStrign<T> result;
		for (size_t i = 0; i < str.size(); i++) {
			result.push_back(static_cast<T>(str[i]));
		}
		return result;
	}
}