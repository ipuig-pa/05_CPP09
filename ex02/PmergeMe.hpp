/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipuig-pa <ipuig-pa@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 12:25:11 by ipuig-pa          #+#    #+#             */
/*   Updated: 2025/06/18 15:57:40 by ipuig-pa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <list>

class PmergeMe
{
private:
	std::vector<int>	_vec;
	std::list<int>		_list;
	int					_JacobSeq[2];
	//include time for vec and for list

	//utils
	int		_checkValidNum(std::string arg);
	int		_findNextJacobsthal(void);

	//vector
	void	_v_parse(std::vector<int> &vec, int ac, char **av);
	bool	_v_checkSorted(const std::vector<int> &vec);
	void	_v_sort(std::vector<int> &vec);
	void	_v_createSortedPairs(std::vector<int> &vec, std::vector<std::pair<int, int>> &pairVec);
	void	_v_createMainChain(std::vector<int> &mainChain, std::vector<std::pair<int, int>> &pairVec);
	void	_v_binaryInsert(std::vector<int> &mainChain, std::vector<std::pair<int, int>> &pairVec);
	void	_v_binarySearch(std::vector<int> &mainChain, int val, size_t start, size_t end);

	//list


public:
	PmergeMe();
	PmergeMe(const PmergeMe &other);
	PmergeMe &operator=(const PmergeMe &other);
	~PmergeMe();

	//vector
	void	PmergeMeVector(int ac, char** av);

	//list
	// void	PmergeMeList(int ac, char** av);

	//utils
	void	printTempVec(std::vector<int> &vec);
	void	printVec();
	void	resetJacob();

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
