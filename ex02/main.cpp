/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipuig-pa <ipuig-pa@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 12:25:24 by ipuig-pa          #+#    #+#             */
/*   Updated: 2025/06/18 15:21:57 by ipuig-pa         ###   ########.fr       */
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
		std::cout << "pmergemevector to be called" << std::endl;
		pmrg.PmergeMeVector(ac, av);
		// std::cout << "Before: " << pmrg.printNum() << std::endl;
		// std::cout << "After: " << pmrg.printNum() << std::endl;
	// 	std::cout << "Time to process a range of " << ac - 1 << "elements with std::vector : " << pmrg.getVectorTime() << std::endl;
	// 	std::cout << "Time to process a range of " << ac - 1 << "elements with std::deque : " << pmrg.getDequeTime() << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}