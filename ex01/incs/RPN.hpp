#ifndef RPN_HPP
# define RPN_HPP

# include <iostream>
# include <stack>

class RPN
{
	public:
		RPN(void);
		RPN(const RPN &copy_src);
		RPN &operator=(const RPN &rhs);
		~RPN(void);

		void	ft_execute(const std::string &arg);
		void	ft_handle_operation(const char &op);

		class InvalidCharacterException : public std::exception
		{
			public:
				virtual const char *what(void) const throw();
		};

		class IncompleteWorkException : public std::exception
		{
			public:
				virtual const char *what(void) const throw();
		};

		class InvalidExpressionException : public std::exception
		{
			public:
				virtual const char *what(void) const throw();
		};

		class DivideByZeroException : public std::exception
		{
			public:
				virtual const char *what(void) const throw();
		};

	private:
		std::stack<int>	_stack;
};

#endif
