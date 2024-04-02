#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange(void) : _data()//std::map の場合、これは空のマップを作成します。
{
	// std::cout << "BitcoinExchange default constructor called" << std::endl;
	this->_timeInfo.tm_sec = 0;
	this->_timeInfo.tm_min = 0;
	this->_timeInfo.tm_hour = 0;
	this->_timeInfo.tm_mday = 0;
	this->_timeInfo.tm_mon = 0;
	this->_timeInfo.tm_year = 0;
	this->_timeInfo.tm_wday = 0;
	this->_timeInfo.tm_yday = 0;
	this->_timeInfo.tm_isdst = 0;//すべてのメンバをゼロで初期化
}
// tm_gmtoffとtm_zoneは、C++98標準のstd::tm構造体には含まれていません

BitcoinExchange::BitcoinExchange(const BitcoinExchange &copy_src)
{
	// std::cout << "BitcoinExchange copy constructor called" << std::endl;
	*this = copy_src;
}

BitcoinExchange	&BitcoinExchange::operator=(const BitcoinExchange &rhs)
{
	// std::cout << "BitcoinExchange copy assignment operator called" << std::endl;
	if (this != &rhs)
	{
		this->_data = rhs._data;
		this->_timeInfo = rhs._timeInfo;
	}
	return (*this);
}

BitcoinExchange::~BitcoinExchange(void)
{
	// std::cout << "BitcoinExchange destructor called" << std::endl;
}

void	BitcoinExchange::ft_output_result(time_t total_sec, float value)
{
	std::map<time_t, float>::iterator	head = this->_data.begin();
	std::map<time_t, float>::iterator	end = this->_data.end();
	std::map<time_t, float>::iterator	current = head;

	long diff = -1;
	for (; head != end; head++)
	{
		if (total_sec - head->first < 0)//2009-01-02より前はここで抜ける＝０
			break ;
		if (diff == -1 || (total_sec - head->first < diff))
		{
			current = head;
			diff = total_sec - head->first;
		}
	}

	std::cout << this->_timeInfo.tm_year + 1900
		<< "-" << ((this->_timeInfo.tm_mon + 1 < 10) ? "0" : "") << this->_timeInfo.tm_mon + 1
		<< "-" << ((this->_timeInfo.tm_mday + 1 < 10) ? "0" : "") << this->_timeInfo.tm_mday;
	std::cout << " => " << value << " = " << value * current->second << std::endl;
}

static bool	ft_is_out_of_range(int year, int month, int day)
{
	if (year < 2009 || (year == 2009 && month == 1 && day == 1))
		return (true);
	if (year > 2022)
		return (true);
	if (year == 2022 && month > 3)
		return (true);
	if (year == 2022 && month == 3 && day > 29)
		return (true);

	return (false);
}

static int	ft_check_date(int year, int month, int day)
{
	int	leap;
	int calendar[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	if (year < 1)
		return (-1);
	if (month < 1 || month > 12)
		return (-1);
	if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
		leap = 1;
	else
		leap = 0;
	calendar[1] += leap;//うるう年なら29日まで
	if (day < 1 || day > calendar[month - 1])
		return (-1);

	if (ft_is_out_of_range(year + 1900, month, day) == true)
		return (-1);

	return (0);
}

static int	ft_atoi(const char *str)
{
	size_t		i;
	long long	res;

	i = 0;
	res = 0;
	if (str[i] == '\0')//valueが空なら弾く
		return (-1);
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	if (str[i] == '\0')//valueがスペース、タブのみなら弾く
		return (-1);
	while (str[i])
	{
		if (str[i] == ' ' || str[i] == '\t')
		{
			while (str[i] == ' ' || str[i] == '\t')
				i++;
			if (str[i] != '\0')
				return (-1);
			else 
				break;
		}
		if (str[i] >= '0' && str[i] <= '9')
		{
			if ((INT_MAX - (str[i] - '0')) / 10 < res)
				return (-1);
			res = (res * 10) + (str[i] - '0');
			i++;
		}
		else
			return (-1);
	}
	return ((int)res);
}

static float	ft_stof(const std::string &string)
{
	float	value;
	std::stringstream	ss(string);
	std::streampos pos = ss.tellg();// 現在の文字列位置を記憶しておく

	if (ss >> value)
	{
		std::streampos endPos = ss.tellg();// 変換後の文字列長を取得し、現在の位置をリセットする
		std::string::size_type convertedLength = static_cast<std::string::size_type>(endPos - pos);
		if (convertedLength != string.length())
			return (value);
		std::cerr << "Error: invalid input. exit" << std::endl;
		std::exit(EXIT_FAILURE);
	}
	std::cerr << "Error: invalid input. exit" << std::endl;
	std::exit(EXIT_FAILURE);
}

time_t	BitcoinExchange::ft_stot(const std::string &date)
{
	std::stringstream	input(date);
	std::string				info;

	int	i = 1;
	while (std::getline(input, info, '-'))
	{
		switch (i)
		{
			case 1:
				this->_timeInfo.tm_year = ft_atoi(info.c_str()) - 1900;//1900年からの経過年数を表す tm_year を設定
				if (this->_timeInfo.tm_year == -1901)
				{
					std::cerr << "Error: invalid input. exit" << std::endl;
					std::exit(EXIT_FAILURE);
				}
				break;
			case 2:
				this->_timeInfo.tm_mon = ft_atoi(info.c_str()) - 1;//構造体の tm_mon は0から11までの範囲で月を表現する
				if (this->_timeInfo.tm_mon == -2)
				{
					std::cerr << "Error: invalid input. exit" << std::endl;
					std::exit(EXIT_FAILURE);
				}
				break;
			case 3:
				this->_timeInfo.tm_mday = ft_atoi(info.c_str());
				if (this->_timeInfo.tm_mday == -1)
				{
					std::cerr << "Error: invalid input. exit" << std::endl;
					std::exit(EXIT_FAILURE);
				}
				break;
			default:
				std::exit(EXIT_FAILURE);
		}
		i++;
	}
	if (!input.eof())//終端に達する前に読み込みエラーが発生した場合
		std::exit(EXIT_FAILURE);

	if (ft_check_date(this->_timeInfo.tm_year, this->_timeInfo.tm_mon + 1, this->_timeInfo.tm_mday) == -1)
	{
		std::cerr << "Error: bad input => " 
			<< this->_timeInfo.tm_year + 1900 
			<< "-" << ((this->_timeInfo.tm_mon + 1 < 10) ? "0" : "") << this->_timeInfo.tm_mon + 1 
			<< "-" << ((this->_timeInfo.tm_mday + 1 < 10) ? "0" : "") << this->_timeInfo.tm_mday << std::endl;
		return (-1);
	}

	return (std::mktime(&this->_timeInfo));
}

void	BitcoinExchange::ft_store_data_csv(void)
{
	std::ifstream	ifs("data.csv");//openメンバ関数でもオープンできるが、初めて行うオープン以外のビットフラグはクリアされない注意
	if (!ifs)//オープンに失敗している
	{
		std::cerr << "Error: could not open file." << std::endl;
		std::exit(EXIT_FAILURE);
	}

	std::string	newline;
	if (std::getline(ifs, newline))//冒頭の一行はいらないからとばす
		;
	else
		std::exit(EXIT_FAILURE);

	while(std::getline(ifs, newline))//newlineは新しい行で上書きされ続ける
	{
		std::stringstream	input(newline);//読み込み専用のistringstreamでも可
		std::string				pair;
		time_t						total_sec;
		float							exchange_rate;

		int	i = LEFT;
		while (std::getline(input, pair, ','))//改行までではなく、カンマ（含まない）までを読み込む
		{
			switch (i)
			{
				case LEFT:
					total_sec = ft_stot(pair);//data.csvは無効な値を考えなくていい
					break;
				case RIGHT:
					exchange_rate = ft_stof(pair);//data.csvは無効な値を考えなくていい
					break;
				default:
					std::exit(EXIT_FAILURE);
			}
			i = RIGHT;//rightが終わると行終端に達する
		}
		if (!input.eof())
			std::exit(EXIT_FAILURE);
		this->_data.insert(std::make_pair(total_sec, exchange_rate));
	}
	if (!ifs.eof())
		std::exit(EXIT_FAILURE);
}
//mapは内部でソートを行うので、挿入位置を指定することに意味がない。insertはあるが、挿入位置は指定しないようになっている

void	BitcoinExchange::ft_execute(const std::string &filename)
{
	this->ft_store_data_csv();

	std::ifstream	ifs(filename);
	if (!ifs)
	{
		std::cerr << "Error: could not open file." << std::endl;
		std::exit(EXIT_FAILURE);
	}

	std::string	newline;
	if (std::getline(ifs, newline))
		;
	else
	{
		std::cerr << "Error: empty file." << std::endl;
		std::exit(EXIT_FAILURE);
	}

	while(std::getline(ifs, newline))
	{
		std::stringstream	input(newline);
		std::string				pair;
		time_t						total_sec;
		float							value;

		int i = 1;
		while (std::getline(input, pair, '|'))
		{
			switch (i)
			{
				case 1:
					total_sec = ft_stot(pair);
					break;
				case 2:
						value = ft_stof(pair);
					break;
				default:
					break;
			}
			i++;
		}
		if (!input.eof())
			std::exit(EXIT_FAILURE);
		if (total_sec == -1)
			continue;
		else if (value < 0)
			std::cerr << "Error: not a positive number." << std::endl;
		else if (value > 1000)
			std::cerr << "Error: too large a number." << std::endl;
		else
			this->ft_output_result(total_sec, value);
	}
	if (!ifs.eof())
		std::exit(EXIT_FAILURE);
}
