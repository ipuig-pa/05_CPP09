/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipuig-pa <ipuig-pa@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 12:56:42 by ipuig-pa          #+#    #+#             */
/*   Updated: 2025/06/04 12:22:12 by ipuig-pa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

RPN::RPN()
	:_stack()
{
}

RPN::RPN(const RPN &other)
{
	*this = other;
}

RPN &RPN::operator=(const RPN &other)
{
	if (this != &other)
	{
		this->_stack = other._stack;
	}
	return (*this);
}

RPN::~RPN()
{
}

int	RPN::processInput(std::string input)
{
	std::istringstream iss(input);
	std::string token;

	while (iss >> token)
	{
		if (token.length() == 1 && isdigit(token[0]))
			_stack.push(token[0] - '0');
		else if (token.length() == 1 && _isoperator(token[0]))
			_operate(token[0]);
		else
			throw std::runtime_error("Error");
	}
	if (_stack.size() != 1)
		throw std::runtime_error("Error: missing operators");
	return (_stack.top());
}

bool	RPN::_isoperator(char c)
{
	return (std::string("+-/*").find(c) != std::string::npos);
}


void	RPN::_operate(char o)
{
	if (_stack.size() < 2)
		throw std::runtime_error("Error: missing operands to perform operation");
	int	operand = _stack.top();
	_stack.pop();
	switch(o)
	{
		case '+':
			_stack.top() += operand;
			break;
		case '-':
			_stack.top() -= operand;
			break;
		case '/':
			if (operand == 0)
				throw std::runtime_error("Error: division by zero");
			_stack.top() /= operand;
			break;
		case '*':
			_stack.top() *= operand;
			break;
		default:
			throw std::runtime_error("Error: non-valid operator");
	}
}
