#pragma once

#include <functional> // std::less
#include <iterator>	  // std::iterator_traits

namespace sort
{

	// This is C++ magic which will allows our function
	// to default to using a < b if the comparator arg
	// is unspecified. It uses defines std::less<T>
	// for the iterator's value_type.
	//
	// For example: if you have a vector<float>, the
	// iterator's value type will be float. std::less
	// will select the < for sorting floats as the
	// default comparator.
	template <typename RandomIter>
	using less_for_iter = std::less<typename std::iterator_traits<RandomIter>::value_type>;

	/* Efficiently swap two items - use this to implement your sorts */
	template <typename T>
	// the & symbol is to 
	void swap(T &a, T &b) noexcept
	{
		T temp = std::move(a);
		a = std::move(b);
		b = std::move(temp);
	}

	template <typename RandomIter, typename Comparator = less_for_iter<RandomIter>>
	void bubble(RandomIter begin, RandomIter end, Comparator comp = Comparator{})
	{
		// Random access iterators have the same traits you defined in the Vector class
		// For instance, difference_type represents an iterator difference
		// You may delete the types you don't use to remove the compiler warnings
		using _it = std::iterator_traits<RandomIter>;
		using difference_type = typename _it::difference_type;
		using value_type = typename _it::value_type;
		using reference = typename _it::reference;
		using pointer = typename _it::pointer;

		// the best case runtime for bubble sort is O(n) and the worst case is O(n^2)
		// conduct bubble sort using given iterators
		// the number of comparisons in the best case is 
		difference_type n = end - begin;
		for (difference_type i = 0; i < n - 1; i++)
		{
			
			for (difference_type j = 0; j < n - i - 1; j++)
			{
				reference left = *(begin + j);
				reference right = *(begin + j + 1);
				if (comp(right, left))
				{
					swap(left, right);
				}
			}
		}
	}

	template <typename RandomIter, typename Comparator = less_for_iter<RandomIter>>
	void insertion(RandomIter begin, RandomIter end, Comparator comp = Comparator{})
	{
		// how insertion works: loop through array and shift elements left until they are in correct position
		using _it = std::iterator_traits<RandomIter>;

		using difference_type = typename _it::difference_type;
		using reference = typename _it::reference;
		difference_type n = end - begin;
		// the number of comparisons in the best case is 
		for(difference_type i = 1; i < n; i++) {
			
			for(difference_type j = i; j > 0; j--) {
				reference left = *(begin + j -1);
				reference right = *(begin + j);
				if(comp(right, left)) {
					swap(left, right);
				} else {
					break;
				}
			}

		}


		// for(RandomIter i = begin+1; i != end; i++) {
		// 	for(RandomIter j = i; j != begin; j--) {
		// 		if(comp(*(j), *(j-1))) {
		// 			swap(*(j), *(j-1));
		// 		} else
		// 			break;
		// 	}
		// }
	}

	template <typename RandomIter, typename Comparator = less_for_iter<RandomIter>>
	void selection(RandomIter begin, RandomIter end, Comparator comp = Comparator{})
	{
		using _it = std::iterator_traits<RandomIter>;
		using difference_type = typename _it::difference_type;
		using reference = typename _it::reference;
		using pointer = typename _it::pointer;

		// code for selection sort
		difference_type n = end - begin;
		for (difference_type i = 0; i < n; i++)
		{
			difference_type min = i;
			for (difference_type j = i + 1; j < n; j++)
			{
				if (comp(*(begin + j), *(begin + min)))
				{
					min = j;
				}
			}

			swap(*(begin + i), *(begin + min));
		}
	}
}