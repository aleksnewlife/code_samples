#pragma once

#include <initializer_list>
#include <vector>


/*
Алгори́тм Нарайа́ны — нерекурсивный алгоритм, генерирующий по данной перестановке следующую за ней перестановку (в лексикографическом порядке). Придуман индийским математиком Пандитом Нарайаной[en] в XIV веке.

Если алгоритм Нарайаны применить в цикле к исходной последовательности из {\displaystyle n}n элементов {\displaystyle a_{1},a_{2},...,a_{n}}{\displaystyle a_{1},a_{2},...,a_{n}}, упорядоченных так, что {\displaystyle a_{1}\leqslant a_{2}\leqslant ...\leqslant a_{n}}{\displaystyle a_{1}\leqslant a_{2}\leqslant ...\leqslant a_{n}}, то он сгенерирует все перестановки элементов множества {\displaystyle \{a_{1},a_{2},...,a_{n}\}}{\displaystyle \{a_{1},a_{2},...,a_{n}\}} в лексикографическом порядке.

Другой особенностью алгоритма является то, что необходимо запоминать только один элемент перестановки.

Алгоритм
Шаг 1: найти такой наибольший {\displaystyle j}j, для которого {\displaystyle a_{j}<a_{j+1}}{\displaystyle a_{j}<a_{j+1}}.
Шаг 2: увеличить {\displaystyle a_{j}}a_{j}. Для этого надо найти наибольшее {\displaystyle l>j}{\displaystyle l>j}, для которого {\displaystyle a_{l}>a_{j}}{\displaystyle a_{l}>a_{j}}. Затем поменять местами {\displaystyle a_{j}}a_{j} и {\displaystyle a_{l}}{\displaystyle a_{l}}.
Шаг 3: записать последовательность {\displaystyle a_{j+1},...,a_{n}}{\displaystyle a_{j+1},...,a_{n}} в обратном порядке.
*/


template<typename TItem>
class NarayanAlgo
{
public:
	using TAlgoData = std::vector<TItem>;

	NarayanAlgo();
	NarayanAlgo(std::initializer_list<TItem> items);
	~NarayanAlgo();

	void append(std::initializer_list<TItem> items);

	bool doStep(int count = 1);

	const TAlgoData& getData() const { return _items; }

private:
	TAlgoData _items;
};

template<typename TItem>
inline NarayanAlgo<TItem>::NarayanAlgo()
{
}

template<typename TItem>
inline NarayanAlgo<TItem>::NarayanAlgo(std::initializer_list<TItem> items)
	: _items(items)
{
}

template<typename TItem>
inline NarayanAlgo<TItem>::~NarayanAlgo()
{
}

template<typename TItem>
inline void NarayanAlgo<TItem>::append(std::initializer_list<TItem> items)
{
	_items.insert(std::end(_items), std::begin(items), std::end(items));
}

template<typename TItem>
inline bool NarayanAlgo<TItem>::doStep(int count)
{
	if (_items.empty())
		return false;

	while (count)
	{
		auto sz = (int)_items.size();

		int J;
		for (J = sz - 2; J >= 0 && _items[J] >= _items[J + 1]; --J)
		{
		}

		if (J == -1)
			return false;

		int L ;
		for (L = sz - 1; _items[J] >= _items[L]; --L)
		{
		}

		std::swap(_items[J], _items[L]);

		J++;
		for (int i = 0; i < (sz - J + 1) >> 1; ++i)
		{
			std::swap(_items[J + i], _items[sz - i - 1]);
		}

		--count;
	}

	return true;
}
