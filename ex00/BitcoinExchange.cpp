/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipuig-pa <ipuig-pa@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 11:42:00 by ipuig-pa          #+#    #+#             */
/*   Updated: 2025/06/27 14:50:42 by ipuig-pa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange()
	:_database()
{
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other)
{
	*this = other;
}

BitcoinExchange	&BitcoinExchange::operator=(const BitcoinExchange &other)
{
	if (this != &other)
	{
		this->_database = other._database;
	}
	return (*this);
}

BitcoinExchange::~BitcoinExchange()
{
}


void	BitcoinExchange::loadDatabase(const std::string &database)
{
	std::ifstream data(database);
	if (!data.is_open())
		throw std::runtime_error("Error: could not open the database.");
	std::string	line;
	bool header_line = true;
	while (std::getline(data, line)) {
		if (header_line) {
			header_line = false;
			continue ;
		}
		size_t comma_pos = line.find(",");
		if (comma_pos == std::string::npos) {
			std::cerr << "Error: bad database => " << line << std::endl;
			continue ;
		}
		std::string date = line.substr(0, comma_pos);
		std::string exchange_rate_str = line.substr(comma_pos + 1);
		double	exchange_rate = std::stod(exchange_rate_str);
		_database.insert({date, exchange_rate});
	}
}

void	BitcoinExchange::processInput(char *input)
{
	std::ifstream file(input);
	if (!file.is_open())
		throw std::runtime_error("Error: could not open the file.");
	std::string	line;
	bool header_line = true;
	while (std::getline(file, line)) {
		if (header_line) {
			header_line = false;
			continue ;
		}
		size_t pipe_pos = line.find("|");
		if (pipe_pos == std::string::npos) {
			std::cerr << "Error: bad input => " << line << std::endl;
			continue ;
		}
		std::string date = _trimSpaces(line.substr(0, pipe_pos));
		std::string val = _trimSpaces(line.substr(pipe_pos + 1));
		_processEntry(date, val);
	}
}

void	BitcoinExchange::_processEntry(std::string date, std::string val_str)
{
	try
	{
		_checkDate(date);
		double val = _checkVal(val_str);
		std::map<std::string, double>::iterator	it = _database.find(date);
		if (it != _database.end())
			std::cout << date << " => " << val << " = " << val * it->second << std::endl;
		else {
			it = _database.lower_bound(date);
			if (it == _database.begin()) {
				throw std::runtime_error("Error: date too early => " + date);
			}
			it--;
			std::cout << date << " => " << val << " = " << val * it->second << std::endl;
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void	BitcoinExchange::_checkDate(std::string date)
{
	if (date.length() != 10 || date[4] != '-' || date[7] != '-')
		throw std::runtime_error("Error: bad input => " + date);
	
	int year, month, day;

	try {
		year = std::stoi(date.substr(0, 4));
		month = std::stoi(date.substr(5, 2));
		day = std::stoi(date.substr(8, 2));
	}
	//catch invalid argument or out of range from stoi
	catch (const std::exception& e) {
		throw std::runtime_error("Error: bad input => " + date);
	}

	if (year < 2009 || year > 2025)
		throw std::runtime_error("Error: Invalid year => " + date);

	if (month < 1 || month > 12)
		throw std::runtime_error("Error: Invalid month => " + date);

	int last_valid_day = 31;
	if (month == 4 || month == 6 || month == 9 || month == 11)
		last_valid_day = 30;
	else if (month == 2 && year % 4 == 0)
		last_valid_day = 29;
	else if (month == 2 && year % 4 != 0)
		last_valid_day = 28;
	if (day < 1 || day > last_valid_day)
		throw std::runtime_error("Error: Invalid day => " + date);
}

double	BitcoinExchange::_checkVal(std::string val_str)
{
	try
	{
		double val = std::stod(val_str);
		if (val < 0)
			throw std::runtime_error("Error: not a positive number.");
		if (val > 1000)
			throw std::runtime_error("Error: too large a number.");
		return (val);
	}
	catch (const std::invalid_argument& e) {
		throw std::runtime_error("Error: invalid number format.");
	}
	catch (const std::out_of_range& e) {
		throw std::runtime_error("Error: number out of range.");
	}
}

std::string BitcoinExchange::_trimSpaces(const std::string& str) {
	size_t first = str.find_first_not_of(' ');
	if (first == std::string::npos) 
		return "";
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, (last - first + 1));
}