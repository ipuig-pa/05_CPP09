/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipuig-pa <ipuig-pa@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 12:25:14 by ipuig-pa          #+#    #+#             */
/*   Updated: 2025/06/18 16:34:06 by ipuig-pa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

/*-------------------------CONSTRUCTORS / DESTRUCTORS-------------------------*/

PmergeMe::PmergeMe()
	:_vec(), _list(), _JacobSeq{1, 1}
{
}

PmergeMe::PmergeMe(const PmergeMe &other)
{
	*this = other;
}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
	if (this != &other)
	{
		this->_vec = other._vec;
		this->_list = other._list;
	}
	return (*this);
}

PmergeMe::~PmergeMe()
{
}

/*-------------------------VECTOR---------------------------------------------*/

void	PmergeMe::PmergeMeVector(int ac, char** av)
{
	_vec.reserve(ac -1);
	_v_parse(_vec, ac, av);
	if (_v_checkSorted(_vec))
		throw PmergeMe::AlreadySortedException("Error: sequence is already sorted");
	_v_sort(_vec);
}

void	PmergeMe::_v_parse(std::vector<int> &vec, int ac, char **av)
{
	std::cout << "parsing" << std::endl;
	for (int i = 1; i < ac; ++i)
	{
		try
		{
			vec.push_back(_checkValidNum((av[i])));
		}
		catch (const std::exception& e)
		{
			throw std::runtime_error("Error: argument \"" + std::string(av[i]) + "\": " + e.what());
		}
	}
}

bool	PmergeMe::_v_checkSorted(const std::vector<int> &vec)
{
	for (size_t i = 0; i < vec.size() - 1; ++i)
	{
		if (vec[i] > vec[i + 1])
			return (false);
	}
	return (true);
}

void	PmergeMe::_v_sort(std::vector<int> &vec)
{
	std::cout << "Sorting: " << std::endl;

	for (size_t i = 0; i < vec.size(); ++i)
		std::cout << vec[i] << ", ";
	std::cout << std::endl;

	if (_v_checkSorted(vec))
		return ;

	//create pairs of sorted numbers
	std::vector<std::pair<int, int>>	pairVec;
	pairVec.reserve(vec.size() / 2);
	_v_createSortedPairs(vec, pairVec);

	std::cout << "Sorted pairs: " << std::endl;
	for (size_t i = 0; i < pairVec.size(); ++i)
		std::cout << pairVec[i].second << ", ";
	std::cout << std::endl;

	//creating main chain with bigger elements of each pair
	std::vector<int> mainChain;
	mainChain.reserve(vec.size());
	_v_createMainChain(mainChain, pairVec);
	printTempVec(mainChain);

	//sroting main chain with the bigger elements of each pair (recursion)
	_v_sort(mainChain);
	printTempVec(mainChain);

	//adding smallest element of first pair into the main chain
	// auto it = std::find(pairVec.begin(), pairVec.end(), mainChain[0]);
	// 	_v_binarySearch(mainChain, pairVec[index].first, 0, it - mainChain.begin());
	std::cout << "adding to main chain: " << pairVec[0].first << std::endl;
	mainChain.emplace(mainChain.begin(), pairVec[0].first); //ERROR!!! not pair[0] but pair of the first element once sorted

	//insert smallest elements of each pair into the main chain
	_v_binaryInsert(mainChain, pairVec);
	if (vec.size() % 2 != 0)
		_v_binarySearch(mainChain, vec[vec.size() - 1], 0, mainChain.end() - mainChain.begin());
	printTempVec(mainChain); // move to main, call it somehow
	std::cout << "one round of sorting FINISHED" << std::endl;
	if (!_v_checkSorted(mainChain))
		throw std::runtime_error("Error: algorithm did not work");
	vec = mainChain;
}

void	PmergeMe::_v_createSortedPairs(std::vector<int> &vec, std::vector<std::pair<int, int>> &pairVec)
{
	for (size_t i = 0; i < vec.size() - 1; i += 2)
	{
		pairVec.push_back({std::min(vec[i], vec[i + 1]), std::max(vec[i], vec[i + 1])});
	}
}

void	PmergeMe::_v_createMainChain(std::vector<int> &mainChain, std::vector<std::pair<int, int>> &pairVec)
{
	for (size_t i = 0; i < pairVec.size(); ++i)
	{
		mainChain.push_back(pairVec[i].second);
		std::cout << "adding to main chain: " << pairVec[i].second << std::endl;
	}
	printTempVec(mainChain);
}

void	PmergeMe::_v_binaryInsert(std::vector<int> &mainChain, std::vector<std::pair<int, int>> &pairVec)
{
	std::cout << "binary insertion" << std::endl;
	size_t index;
	while (mainChain.size() != mainChain.capacity() && mainChain.size() != mainChain.capacity() - 1)
	{
		printTempVec(mainChain);
		index = _findNextJacobsthal() - 1;
		if (index > (mainChain.size() / 2))
			index = mainChain.size() / 2;
		std::cout << "index added: " << index << std::endl;
		auto it = std::find(mainChain.begin(), mainChain.end(), pairVec[index].second);
		_v_binarySearch(mainChain, pairVec[index].first, 0, it - mainChain.begin());
		for (int i = index - 1; i > _JacobSeq[0] - 1; i--)
		{
			std::cout << "index added: " << i << std::endl;
			auto it = std::find(mainChain.begin(), mainChain.end(), pairVec[i].second);
			_v_binarySearch(mainChain, pairVec[i].first, 0, it - mainChain.begin());
		}
	}
}

void	PmergeMe::_v_binarySearch(std::vector<int> &mainChain, int val, size_t start, size_t end)
{
	std::cout << "binary search " << val << " in ";
	printTempVec(mainChain);
	std::cout << "from " << start << " to " << end << std::endl;
	if (end == start) 
	{
		mainChain.emplace(mainChain.begin() + start, val);
		return ;
	}
	size_t i = start + ((end - start) / 2);

	std::cout << "comparing " << val << " with " << mainChain[i] << std::endl;

	if (!(mainChain[i] > val))
		start = i + 1;
	else if (!(mainChain[i] < val))
		end = i;
	_v_binarySearch(mainChain, val, start, end);
}

/*-------------------------LIST-----------------------------------------------*/



/*-------------------------UTILS----------------------------------------------*/

int	PmergeMe::_checkValidNum(std::string arg)
{
	try
	{
		int num = std::stoi(arg);
		if (num <= 0)
			throw PmergeMe::NonPositiveException("not a valid positive number.");
		return (num);
	}
	catch(const std::exception& e)
	{
		throw;
	}
}

int	PmergeMe::_findNextJacobsthal(void)
{
	int temp = _JacobSeq[1];
	_JacobSeq[1] = _JacobSeq[1] + 2 * _JacobSeq[0];
	_JacobSeq[0] = temp;
	return (_JacobSeq[1]);
}

void	PmergeMe::printTempVec(std::vector<int> &vec)
{
	for (size_t i = 0; i < vec.size() - 1; ++i)
	{
		std::cout << vec[i] << ", ";
	}
	std::cout << vec[vec.size() - 1] << std::endl;
}

void	PmergeMe::printVec()
{
	for (size_t i = 0; i < _vec.size() - 1; ++i)
	{
		std::cout << _vec[i] << ", ";
	}
	std::cout << _vec[_vec.size() - 1] << std::endl;
}

void	PmergeMe::resetJacob()
{
	_JacobSeq[0] = 1;
	_JacobSeq[1] = 1;
}

/*-------------------------EXCEPTIONS-----------------------------------------*/

PmergeMe::NonPositiveException::NonPositiveException()
	: _msg("")
{
}

PmergeMe::NonPositiveException::NonPositiveException(std::string_view msg)
	: _msg(msg)
{
}

const char* PmergeMe::NonPositiveException::what() const noexcept
{
	return _msg.c_str();
}


PmergeMe::AlreadySortedException::AlreadySortedException(std::string_view msg)
	: _msg(msg)
{
}

const char* PmergeMe::AlreadySortedException::what() const noexcept
{
	return _msg.c_str();
}
