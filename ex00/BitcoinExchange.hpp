/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipuig-pa <ipuig-pa@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 11:41:55 by ipuig-pa          #+#    #+#             */
/*   Updated: 2025/06/01 12:39:38 by ipuig-pa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <map>

class BitcoinExchange
{
private:
	std::map<std::string, double>	_database;

	void		_processEntry(std::string date, std::string val_str);
	void		_checkDate(std::string date);
	double		_checkVal(std::string val_str);
	std::string _trimSpaces(const std::string& str);

public:
	BitcoinExchange();
	BitcoinExchange(const BitcoinExchange &other);
	BitcoinExchange	&operator=(const BitcoinExchange &other);
	~BitcoinExchange();

	void	processInput(char *input); 
	void	loadDatabase(const std::string &database);
};