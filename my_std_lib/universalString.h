#pragma once

#include <vector>
#include <algorithm>
#include <iostream>
#include <concepts>
#include <functional>

namespace my_std {

	template <typename value>
	class ITransformer {
	public:
		virtual value operator()(value) = 0;
	};

	/*template<typename T>
	concept charType =
		requires(T a) {
			{ a } -> std::convertible_to<bool>;
			{ a } -> std::convertible_to<int>;
			{ a } -> std::convertible_to<unsigned>;
			{ a } -> std::convertible_to<char>;
			{ a } -> std::convertible_to<wchar_t>;
	};*/

	template <class charT>
	class universalStrign {
		template <typename value>
		class defaultTransformer : public ITransformer<value> {
		private:
			std::function<value(value)> _functor;

		public:
			defaultTransformer(std::function<value(value)> Functor) : _functor(Functor) {}

			value operator()(value val) override { return _functor(val); }
		};
	public:
		universalStrign() : _size(0), data() { }

		universalStrign(charT value) : _size(1), data(value) { }

		[[maybe_unused]] universalStrign(std::size_t, charT);

		[[maybe_unused]] universalStrign(std::size_t);

		universalStrign(universalStrign&) = default;

		universalStrign(universalStrign&&) noexcept;

		universalStrign(const charT*);

		universalStrign(const charT*, const charT*);

		template <class OtherCharT>
		universalStrign(universalStrign<OtherCharT>&);

		universalStrign& operator=(universalStrign&) = default;

		universalStrign operator=(universalStrign&&) noexcept;

		std::size_t size() const { return _size; }

		bool isEmpty() const  { return data.empty(); }

		void pop_front();

		void pop_back();

		void push_back(charT);

		void push_front(charT);

		void clear() { data.clear(); _size = 0; }

		charT& operator[](std::size_t);

		charT operator[](std::size_t) const;

		universalStrign split(std::size_t index) {
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
		

		///>
		friend universalStrign operator+(universalStrign, universalStrign);

		///>
		template <typename T>
		friend universalStrign operator*(universalStrign, T);

		///>
		friend bool operator==(universalStrign, universalStrign);

		///>
		friend bool operator!=(universalStrign, universalStrign);

		///>
		friend bool operator>=(universalStrign, universalStrign);

		///>
		friend bool operator>(universalStrign, universalStrign);

		///>
		friend bool operator<=(universalStrign, universalStrign);

		///>
		friend bool operator<(universalStrign, universalStrign);

		//> На месте

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
		//<

		//> Создание новой строки

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
		//<

		///>
		friend universalStrign make_string(const charT*);

		///>
		friend universalStrign make_string(const charT*, const charT*);

		///>
		void scan();

	private:
		std::size_t _size;
		std::vector<charT> data;
	};

	template<class charT>
	universalStrign<charT>::universalStrign(std::size_t size, charT value) : universalStrign()
	{
		for (std::size_t i = 0; i < size; i++)
		{
			universalStrign<charT>::push_back(value);
		}
	}

	template<class charT>
	universalStrign<charT>::universalStrign(std::size_t size) : universalStrign()
	{
		for (std::size_t i = 0; i < size; i++)
		{
			universalStrign<charT>::push_back(charT());
		}
	}

	template<class charT>
	inline universalStrign<charT>::universalStrign(universalStrign&& other) noexcept
		: _size(other._size), data(std::move(other.data)) { other._size = 0; }

	template<class charT>
	universalStrign<charT>::universalStrign(const charT* Array) : universalStrign()
	{
		for (std::size_t i = 0; Array[i] != 0; i++)
		{
			universalStrign<charT>::push_back(Array[i]);
		}
	}

	template<class charT>
	universalStrign<charT>::universalStrign(const charT* Array, const charT* ArrayEnd) : universalStrign()
	{
		for (std::size_t i = 0; Array + i != ArrayEnd; i++)
		{
			universalStrign<charT>::push_back(Array[i]);
		}
	}

	template<class charT>
	universalStrign<charT> universalStrign<charT>::operator=(universalStrign<charT>&& other) noexcept
	{
		_size = other._size;
		data = std::move(other.data);
		other._size = 0;
		return *this;
	}

	template<class charT>
	void universalStrign<charT>::pop_front()
	{
		if (_size) {
			std::size_t size = _size;
			charT temp = data[0];
			std::size_t i;
			for (i = 0, size--; i < size; i++) data[i] = data[i + 1];
			data[size] = temp;
			data.pop_back();
			_size--;
		}
	}

	template<class charT>
	void universalStrign<charT>::pop_back()
	{
		if (_size) {
			data.pop_back();
			_size--;
		}
	}

	template<class charT>
	void universalStrign<charT>::push_back(charT value)
	{
		data.push_back(value);
		_size++;
	}

	template<class charT>
	void universalStrign<charT>::push_front(charT value)
	{
		data.reserve(1);
		std::size_t size = _size;
		charT temp = data[--size];
		while (size > 0) {
			data[size--] = data[size - 1];
		}
		data.push_back(temp);
		data[0] = value;
		_size++;
	}

	template<class charT>
	charT& universalStrign<charT>::operator[](std::size_t index)
	{
		if (index < _size) {
			return data[index];
		}
		else {
			throw std::out_of_range("Out of range error [universalStrign<charT>::operator[]]");
		}
	}
	template<class charT>
	charT universalStrign<charT>::operator[](std::size_t index) const
	{
		if (index < _size) {
			return data[index];
		}
		else {
			throw std::out_of_range("Out of range error [universalStrign<charT>::operator[]]");
		}
	}
}