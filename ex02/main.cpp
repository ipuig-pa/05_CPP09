/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipuig-pa <ipuig-pa@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 12:25:24 by ipuig-pa          #+#    #+#             */
/*   Updated: 2025/06/19 16:22:56 by ipuig-pa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include <iostream>


int main(int ac, char **av)
{
	try
	{
		if (ac < 2)
			throw std::runtime_error("Error: incorrect number of arguments.");

		PmergeMe	pmrg;
		pmrg.PmergeMeVector(ac, av);
		pmrg.PmergeMeList(ac, av);

		//print initial int
		std::cout << "Before: ";
		for (int i = 1; i < ac - 1; ++i)
		{
			std::cout << av[i] << ", ";
		}
		std::cout << av[ac - 1] << std::endl;
	
		//print sorted int
		std::cout << "After: ";
		pmrg.printVec();

		//print time with vector
		std::cout << "Time to process a range of " << ac - 1 << " elements with std::vector : " << std::fixed
		<< std::setprecision(2) << pmrg.getVecTime() << " us" << std::endl;
		//print time with list
		std::cout << "Time to process a range of " << ac - 1 << " elements with std::list : " << std::fixed
		<< std::setprecision(2) << pmrg.getListTime() << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}