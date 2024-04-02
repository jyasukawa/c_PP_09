#include "BitcoinExchange.hpp"

int	main(int argc, char **argv)
{
	if (argc == 1)
	{
		std::cout << "Error: could not open file." << std::endl;
		return (1);
	}
	if (argc > 2)
	{
		std::cout << "Usage: ./btc <filename>" << std::endl;
		return (1);
	}

	BitcoinExchange	btc;
	btc.ft_execute(argv[1]);

	return (0);
}
