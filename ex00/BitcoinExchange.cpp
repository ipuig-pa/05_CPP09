/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipuig-pa <ipuig-pa@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 11:42:00 by ipuig-pa          #+#    #+#             */
/*   Updated: 2025/05/31 20:35:50 by ipuig-pa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

BitcoinExcange::BitcoinExcange()
{
	_setDatabase("data.csv");
}

//add the other members of Canonical Orthodox Form
//BitcoinExcange::BitcoinExcange

BitcoinExcange::~BitcoinExcange()
{
}


void	BitcoinExcange::_setDatabase(std::string database)
{
	std::ifstream data(database);
	if (!data.is_open())
		throw std::runtime_error("Error: could not open the database.");
	std::string	line;
	bool header_line = true;
	while (std::getline(file, line)) {
		if (header_line) {
			header_line = false;
			continue ;
		}
		size_t comma_pos = line.find(",");
		if (comma_pos == std::string::npos) {
			std::cerr << "Error: bad database => " << line << std::endl;
			continue ;
		}
		std::string date = line.substr(0, comma_pos - 1);
		std::string exchange_rate_str = line.substr(comma_pos + 1);
		double	exchange_rate = std::stod(exchange_rate_str);
		_database.insert({date, exchange_rate});
	}
}

void	BitcoinExcange::processInput(char *input)
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
		std::string date = line.substr(0, pipe_pos - 1);
		std::string val = line.substr(pipe_pos + 1);
		_processEntry(date, val);
	}
}

void	BitcoinExcange::_processEntry(std::string date, std::string val_str)
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
			it++;
			std::cout << date << " => " << val << " = " << val * it->second << std::endl;
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void	_checkDate(std::string date)
{
	size_t year_pos = date.find("-");
	if (year_pos == std::string::npos)
		throw std::runtime_error("Error: Invalid date format => " + date);
	std::string year_str = date.substr(0, year_pos);
	int	year = std::stoi(year_str);
	if (year < 2009 || year > 2025)
		throw std::runtime_error("Error: Invalid year => " + date);

	size_t month_pos = date.find("-", year_pos + 1);
	if (month_pos == std::string::npos)
		throw std::runtime_error("Error: Invalid date format => " + date);
	std::string month_str = date.substr(year_pos + 1, month_pos);
	int	month = std::stoi(month_str);
	if (month < 1 || month > 12)
		throw std::runtime_error("Error: Invalid month => " + date);

	if (month_pos == date.size() - 1)
		throw std::runtime_error("Error: Invalid date format => " + date);
	size_t day_pos = date.find(" ", month_pos + 1);
	if (day_pos == std::string::npos)
		day_pos == date.size() - 1;
	std::string day_str = date.substr(month_pos + 1, day_pos);
	int last_valid_day = 31;
	if (month == 4 | month == 6 | month == 9 | month == 11)
		last_valid_day = 30
	else if (month == 2 && year%4 == 0)
		last_valid_day = 29;
	else if (month == 2 && year%4 != 0)
		last_valid_day = 28;
	int	day = std::stoi(day);
	if (day < 1 || day > last_valid_day)
		throw std::runtime_error("Error: Invalid day => " + date);
}

double	_checkVal(std::string val_str)
{
	double val = std::stod(val_str);
	if (val < 0)
		throw std::runtime_error("Error: not a positive number.");
	if (val > 1000)
		throw std::runtime_error("Error: too large a number.");
	return (val);
}