/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipuig-pa <ipuig-pa@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 12:25:11 by ipuig-pa          #+#    #+#             */
/*   Updated: 2025/06/19 14:22:06 by ipuig-pa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <list>
#include <chrono>

class PmergeMe
{
private:
	std::vector<int>	_vec;
	std::list<int>		_list;
	int					_JacobSeq[2];
	double				_vecSortTime;
	double				_listSortTime;

	//include time for vec and for list

	//utils
	int		_checkValidNum(std::string arg);
	int		_findNextJacobsthal(void);

	//vector
	void	_v_parse(std::vector<int> &vec, int ac, char **av);
	void	_v_sort(std::vector<int> &vec);
	void	_v_createSortedPairs(std::vector<int> &vec, std::vector<std::pair<int, int>> &pairVec);
	void	_v_createAChain(std::vector<int> &aChain, std::vector<std::pair<int, int>> &pairVec);
	void	_v_createBChain(std::vector<int> &bChain, std::vector<int> &aChain, std::vector<std::pair<int, int>> &pairVec);
	void	_v_createMainChain(std::vector<int> &aChain, std::vector<int> &bChain, std::vector<int> &mainChain);
	void	_v_binaryInsert(std::vector<int> &aChain, std::vector<int> &bChain, std::vector<int> &mainChain);
	void	_v_binarySearch(std::vector<int> &aChain, int val, size_t start, size_t end);

	//list
	void	_l_parse(std::list<int> &list, int ac, char **av);
	void	_l_sort(std::list<int> &list);
	void	_l_createSortedPairs(std::list<int> &list, std::list<std::pair<int, int>> &pairList);
	void	_l_createAChain(std::list<int> &aChain, std::list<std::pair<int, int>> &pairList);
	void	_l_createBChain(std::list<int> &bChain, std::list<int> &aChain, std::list<std::pair<int, int>> &pairList);
	void	_l_createMainChain(std::list<int> &aChain, std::list<int> &bChain, std::list<int> &mainChain);
	void	_l_binaryInsert(std::list<int> &aChain, std::list<int> &bChain, std::list<int> &mainChain);
	void	_l_binarySearch(std::list<int> &mainChain, int val, size_t start, size_t end);

public:
	PmergeMe();
	PmergeMe(const PmergeMe &other);
	PmergeMe &operator=(const PmergeMe &other);
	~PmergeMe();

	//vector
	void	PmergeMeVector(int ac, char** av);

	//list
	void	PmergeMeList(int ac, char** av);

	//utils
	void	printTempVec(std::vector<int> &vec);
	void	printTempList(std::list<int> &list);
	void	printVec();
	void	resetJacob();
	double	getVecTime();
	double	getListTime();

	//exceptions
	class NonPositiveException : public std::exception{
		public:
			NonPositiveException();
			NonPositiveException(std::string_view msg);
			const char* what() const noexcept override;
	
		private:
			const std::string _msg;
		};
	
		class AlreadySortedException : public std::exception{
		public:
			AlreadySortedException(std::string_view msg);
			const char* what() const noexcept override;
	
		private:
			const std::string _msg;
		};
};
