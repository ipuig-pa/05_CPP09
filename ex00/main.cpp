/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipuig-pa <ipuig-pa@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 12:25:24 by ipuig-pa          #+#    #+#             */
/*   Updated: 2025/05/31 20:36:47 by ipuig-pa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

int main(int ac, char **av)
{
	BitcoinExcange
	if (ac != 2)
		return 1;
	try
	{
		BitcoinExchange::parseInput(av[1]);
		BitcoinExcange
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	
}