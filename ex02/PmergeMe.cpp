/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipuig-pa <ipuig-pa@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 12:25:14 by ipuig-pa          #+#    #+#             */
/*   Updated: 2025/06/19 16:08:52 by ipuig-pa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

/*-------------------------CONSTRUCTORS / DESTRUCTORS-------------------------*/

PmergeMe::PmergeMe()
	:_vec(), _list(), _JacobSeq{1, 1}, _vecSortTime(0.00), _listSortTime(0.00)
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
	auto start = std::chrono::high_resolution_clock::now();

	_vec.reserve(ac -1);
	_v_parse(_vec, ac, av);
	if (std::is_sorted(_vec.begin(), _vec.end()))
		throw PmergeMe::AlreadySortedException("Error: sequence is already sorted");
	_v_sort(_vec);
	if (!std::is_sorted(_vec.begin(), _vec.end()))
		throw std::runtime_error("Error: algorithm did not work");
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration<double, std::micro>(end - start);
	_vecSortTime = duration.count();
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

void	PmergeMe::_v_sort(std::vector<int> &vec)
{
	std::cout << "Sorting: " << std::endl;

	printTempVec(vec);

	if (std::is_sorted(vec.begin(), vec.end()))
		return ;

	//create pairs of sorted numbers
	std::vector<std::pair<int, int>>	pairVec;
	pairVec.reserve(vec.size() / 2);
	_v_createSortedPairs(vec, pairVec);

	std::cout << "Sorted pairs: " << std::endl;
	for (size_t i = 0; i < pairVec.size(); ++i)
		std::cout << pairVec[i].second << ", ";
	std::cout << std::endl;

	//creating "A chain" with bigger elements of each pair
	std::cout << "creating A chain" << std::endl;
	std::vector<int> aChain;
	aChain.reserve(vec.size() / 2);
	_v_createAChain(aChain, pairVec);
	printTempVec(aChain);

	//sroting "A chain" with the bigger elements of each pair (recursion)
	_v_sort(aChain);
	printTempVec(aChain);

	//creating "B chain", witht the smallest elements of each pair in the specific order
	std::cout << "creating B chain" << std::endl;
	std::vector<int> bChain;
	if (vec.size() % 2 != 0)
	{
		bChain.reserve((vec.size() / 2) + 1);
		std::cout << "capacity: " << bChain.capacity();
		std::cout << "size: " << bChain.size();
		bChain.push_back(vec[vec.size() - 1]);
	}
	else
		bChain.reserve(vec.size() / 2);
	std::cout << "creating b chain: " << std::endl;
	_v_createBChain(bChain, aChain, pairVec);
	std::cout << "capacity: " << bChain.capacity();
	std::cout << "size: " << bChain.size();
	printTempVec(bChain);

	//create main chain in original vector
	_v_createMainChain(aChain, bChain, vec);

	//insert smallest elements of each pair into the main chain
	_v_binaryInsert(aChain, bChain, vec);
	printTempVec(vec); // move to main, call it somehow

	resetJacob();
	std::cout << "one round of sorting FINISHED" << std::endl;
}

void	PmergeMe::_v_createSortedPairs(std::vector<int> &vec, std::vector<std::pair<int, int>> &pairVec)
{
	for (size_t i = 0; i < vec.size() - 1; i += 2)
	{
		pairVec.push_back({std::min(vec[i], vec[i + 1]), std::max(vec[i], vec[i + 1])});
	}
}

void	PmergeMe::_v_createAChain(std::vector<int> &aChain, std::vector<std::pair<int, int>> &pairVec)
{
	for (size_t i = 0; i < pairVec.size(); ++i)
	{
		aChain.push_back(pairVec[i].second);
		std::cout << "adding to main chain: " << pairVec[i].second << std::endl;
	}
	printTempVec(aChain);
}

void	PmergeMe::_v_createBChain(std::vector<int> &bChain, std::vector<int> &aChain, std::vector<std::pair<int, int>> &pairVec)
{
	for (size_t i = 0; i < aChain.size(); ++i)
	{
		for (size_t j = 0; j < pairVec.size(); ++j)
		{
			if (aChain[i] == pairVec[j].second)
			{
				bChain.emplace(bChain.begin() + i, pairVec[j].first);
				pairVec[j] = {0, 0};
			}
		}
	}
}

void	PmergeMe::_v_createMainChain(std::vector<int> &aChain, std::vector<int> &bChain, std::vector<int> &mainChain)
{
	//adding smallest element of first pair into the main chain
	std::cout << "adding to main chain: " << bChain[0] << std::endl;
	mainChain[0] = bChain[0];

	//adding sorted biggest elements of each pair into the main chain
	for (size_t i = 0; i < aChain.size(); ++i)
	{
		mainChain[i + 1] = aChain[i];
	}
	for (size_t i = aChain.size() ; i < mainChain.size(); ++i)
	{
		mainChain.erase(mainChain.begin() + aChain.size() + 1, mainChain.end()); 
	}
}

void	PmergeMe::_v_binaryInsert(std::vector<int> &aChain, std::vector<int> &bChain, std::vector<int> &mainChain)
{
	std::cout << "binary insertion" << std::endl;
	size_t index;
	while (_JacobSeq[1] < static_cast<int>(bChain.size()))
	{
		std::cout << "main chain size: " << mainChain.size() << " Main chain capacity: " << mainChain.capacity() << std::endl;
		printTempVec(mainChain);
		index = _findNextJacobsthal() - 1;
		if (index > bChain.size() - 1)
			index = bChain.size() - 1;
		auto it = mainChain.end();
		if (index < aChain.size())
		{
			std::cout << "index added: " << index << std::endl;
			it = std::find(mainChain.begin(), mainChain.end(), aChain[index]);
		}
		_v_binarySearch(mainChain, bChain[index], 0, it - mainChain.begin());
		for (int i = index - 1; i > _JacobSeq[0] - 1; i--)
		{
			std::cout << "index added: " << i << std::endl;
			auto it = std::find(mainChain.begin(), mainChain.end(), aChain[i]);
			_v_binarySearch(mainChain, bChain[i], 0, it - mainChain.begin());
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

void	PmergeMe::PmergeMeList(int ac, char** av)
{
	auto start = std::chrono::high_resolution_clock::now();

	_l_parse(_list, ac, av);
	if (std::is_sorted(_list.begin(), _list.end()))
		throw PmergeMe::AlreadySortedException("Error: sequence is already sorted");
	_l_sort(_list);
	if (!std::is_sorted(_list.begin(), _list.end()))
		throw std::runtime_error("Error: algorithm did not work");
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration<double, std::micro>(end - start);
	_listSortTime = duration.count();
}

void	PmergeMe::_l_parse(std::list<int> &list, int ac, char **av)
{
	std::cout << "parsing" << std::endl;
	for (int i = 1; i < ac; ++i)
	{
		try
		{
			list.push_back(_checkValidNum((av[i])));
		}
		catch (const std::exception& e)
		{
			throw std::runtime_error("Error: argument \"" + std::string(av[i]) + "\": " + e.what());
		}
	}
}

void	PmergeMe::_l_sort(std::list<int> &list)
{
	std::cout << "Sorting: " << std::endl;

	printTempList(list);

	if (std::is_sorted(list.begin(), list.end()))
		return ;

	//create pairs of sorted numbers
	std::list<std::pair<int, int>>	pairList;
	_l_createSortedPairs(list, pairList);

	std::cout << "Sorted pairs: " << std::endl;
	for (auto it = pairList.begin(); it != pairList.end(); ++it)
		std::cout << (*it).second << ", ";
	std::cout << std::endl;

	//creating "A chain" with bigger elements of each pair
	std::cout << "creating A chain" << std::endl;
	std::list<int> aChain;
	_l_createAChain(aChain, pairList);
	printTempList(aChain);

	//sroting "A chain" with the bigger elements of each pair (recursion)
	_l_sort(aChain);
	printTempList(aChain);

	//creating "B chain", witht the smallest elements of each pair in the specific order
	std::cout << "creating B chain" << std::endl;
	std::list<int> bChain;
	_l_createBChain(bChain, aChain, pairList);
	if (list.size() % 2 != 0)
	{
		bChain.push_back(list.back());
	}
	printTempList(bChain);

	//create main chain in original list
	_l_createMainChain(aChain, bChain, list);

	std::cout << "Main chain: " << std::endl;
	printTempList(list);

	//insert smallest elements of each pair into the main chain
	_l_binaryInsert(aChain, bChain, list);
	printTempList(list); // move to main, call it somehow
	
	resetJacob();
	
	std::cout << "one round of sorting FINISHED" << std::endl;
}

void	PmergeMe::_l_createSortedPairs(std::list<int> &list, std::list<std::pair<int, int>> &pairList)
{
	std::cout << "creating pairs" << std::endl;
	for (auto it = list.begin(); it != list.end(); std::advance(it, 2))
	{
		if (std::next(it) != list.end())
			pairList.push_back({std::min(*it, *(std::next(it))), std::max(*it, *(std::next(it)))});
		else
			break;
	}
}

void	PmergeMe::_l_createAChain(std::list<int> &aChain, std::list<std::pair<int, int>> &pairList)
{
	for (auto it = pairList.begin(); it != pairList.end(); ++it)
	{
		aChain.push_back((*it).second);
		std::cout << "adding to main chain: " << (*it).second << std::endl;
	}
	printTempList(aChain);
}

void	PmergeMe::_l_createBChain(std::list<int> &bChain, std::list<int> &aChain, std::list<std::pair<int, int>> &pairList)
{
	for (auto it = aChain.begin(); it != aChain.end(); ++it)
	{
		for (auto it2 = pairList.begin(); it2 != pairList.end(); ++it2)
		{
			if (*it == (*it2).second)
			{
				bChain.push_back((*it2).first);
				*it2 = {0, 0};
			}
		}
	}
}

void	PmergeMe::_l_createMainChain(std::list<int> &aChain, std::list<int> &bChain, std::list<int> &mainChain)
{
	//adding smallest element of first pair into the main chain
	std::cout << "adding to main chain: " << bChain.front() << std::endl;
	mainChain.resize(0);
	mainChain.push_back(bChain.front());

	//adding sorted biggest elements of each pair into the main chain
	for (auto it = aChain.begin(); it != aChain.end(); ++it)
	{
		std::cout << "adding to main chain: " << *it << std::endl;
		mainChain.push_back(*it);
	}
}

void	PmergeMe::_l_binaryInsert(std::list<int> &aChain, std::list<int> &bChain, std::list<int> &mainChain)
{
	std::cout << "binary insertion" << std::endl;
	size_t index;
	while (_JacobSeq[1] < static_cast<int> (bChain.size()))
	{
		std::cout << "main chain size: " << mainChain.size() << std::endl;
		printTempList(mainChain);
		index = _findNextJacobsthal();
		if (index > bChain.size())
			index = bChain.size();
		std::cout << "index added (Jacobsthal): " << index << std::endl;
		
		size_t end = 1;
		if (index > aChain.size())
			end = mainChain.size() + 1;
		else
		{
			auto it_a_index = aChain.begin();
			for (size_t i = 1; i < index; ++i)
				it_a_index = std::next(it_a_index);
			
			for (auto it = mainChain.begin(); it != mainChain.end(); ++it)
			{
				if (*it == *it_a_index)
					break;
				end++;
			}
		}

		auto it_b_index = bChain.begin();
		for (size_t i = 1; i < index; ++i)
			it_b_index = std::next(it_b_index);
		_l_binarySearch(mainChain, *it_b_index, 1, end);
		for (int i = index - 1; i > _JacobSeq[0]; i--)
		{
			auto it_a_index = aChain.begin();
			for (int j = 1; j < i; ++j)
				it_a_index = std::next(it_a_index);
			std::cout << "index added (filling): " << i << std::endl;
			size_t end = 1;
			for (auto it = mainChain.begin(); it != mainChain.end(); ++it)
			{
				if (*it == *it_a_index)
					break;
				end++;
			}
			auto it_b_index = bChain.begin();
			for (int j = 1; j < i; ++j)
				it_b_index = std::next(it_b_index);
			_l_binarySearch(mainChain, *it_b_index, 1, end);
		}
	}
}

void	PmergeMe::_l_binarySearch(std::list<int> &mainChain, int val, size_t start, size_t end)
{
	std::cout << "binary search " << val << " in ";
	printTempList(mainChain);
	std::cout << "from " << start << " to " << end << std::endl;
	if (end == start) 
	{
		auto it = mainChain.begin();
		for (size_t i = 1; i < start; ++i)
			it = std::next(it);
		mainChain.insert(it, val);
		return ;
	}
	size_t comp_i = start + ((end - start) / 2);

	auto comp_it = mainChain.begin();
	for (size_t i = 1; i < comp_i; ++i)
		comp_it = std::next(comp_it);

	std::cout << "comparing " << val << " with " << *comp_it << std::endl;

	if (!((*comp_it) > val))
		start = comp_i + 1;
	else if (!((*comp_it) < val))
		end = comp_i;
	_l_binarySearch(mainChain, val, start, end);
}

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

void	PmergeMe::printTempList(std::list<int> &list)
{
	for (auto it = list.begin(); it != list.end(); ++it)
	{
		std::cout << *it;
		if (std::next(it) != list.end())
			std::cout << ", ";
		else
			std::cout << std::endl;
	}
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

double	PmergeMe::getVecTime()
{
	return (_vecSortTime);
}

double	PmergeMe::getListTime()
{
	return (_listSortTime);
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
