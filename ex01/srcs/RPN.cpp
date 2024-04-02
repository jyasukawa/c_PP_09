#include "RPN.hpp"

RPN::RPN(void) : _stack()
{
	// std::cout << "RPN default constructor called" << std::endl;
}

RPN::RPN(const RPN &copy_src)
{
	// std::cout << "RPN copy constructor called" << std::endl;
	*this = copy_src;
}

RPN	&RPN::operator=(const RPN &rhs)
{
	// std::cout << "RPN copy assignment operator called" << std::endl;
	if (this != &rhs)
		this->_stack = rhs._stack;
	return (*this);
}

RPN::~RPN(void)
{
	// std::cout << "RPN destructor called" << std::endl;
}

void	RPN::ft_handle_operation(const char &op)
{
	if (this->_stack.size() < 2)
		throw RPN::InvalidExpressionException();//ft_execute関数まで伝播する

	int a = this->_stack.top();
	this->_stack.pop();
	int b = this->_stack.top();
	this->_stack.pop();
	switch (op)
	{
		case '+':
			this->_stack.push(b + a);
			break;
		case '-':
			this->_stack.push(b - a);//a - bではないことに注意
			break;
		case '*':
			this->_stack.push(b * a);
			break;
		case '/':
			if (a == 0)
				throw RPN::DivideByZeroException();
			this->_stack.push(b / a);
			break;
	}
}

void	RPN::ft_execute(const std::string &arg)
{
	for (size_t i = 0; i < arg.length(); i++)
	{
		if (arg[i] >= '0' && arg[i] <= '9')
			_stack.push(arg[i] - '0');
		else if (arg[i] == '+' || arg[i] == '-' || arg[i] == '*' || arg[i] == '/')
		{
			ft_handle_operation(arg[i]);
		}
		else if (std::isspace(arg[i]) == 0)//返り値が0以外の場合は、指定された文字が空白文字であることを示す
			throw RPN::InvalidCharacterException();
	}
	if (_stack.size() != 1)
		throw RPN::IncompleteWorkException();
	std::cout << _stack.top() << std::endl;
}

const char *RPN::InvalidCharacterException::what(void) const throw()
{
	return ("Error");
}

const char *RPN::IncompleteWorkException::what(void) const throw()
{
	return ("Error: Incomplete work");
}

const char *RPN::InvalidExpressionException::what(void) const throw()
{
	return ("Error: Invalid expression");
}

const char *RPN::DivideByZeroException::what(void) const throw()
{
	return ("Error: An number divided by zero");
}
