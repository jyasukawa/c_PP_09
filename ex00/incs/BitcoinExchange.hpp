#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <iostream>
# include <fstream>
# include <sstream>
# include <map>

# define LEFT 1
# define RIGHT 2

class BitcoinExchange
{
	public:
		BitcoinExchange(void);
		BitcoinExchange(const BitcoinExchange &copy_src);
		BitcoinExchange &operator=(const BitcoinExchange &rhs);
		~BitcoinExchange(void);

		void	ft_execute(const std::string &filename);

	private:
		std::map<std::time_t, float>	_data;
		std::tm												_timeInfo;

		void		ft_store_data_csv(void);
		time_t	ft_stot(const std::string &date);
		void		ft_output_result(time_t total_sec, float value);
};

#endif

// map および multimapは、STLコンテナの一種で、ソートされた集合を提供します。map は、同じキーを持ったデータが重複することを許さず、multimap は許すという違いがあります。キーというのは、ソートの条件に使用する値のことを指しています。
// 今回は”data.csv”に重複がないためmapで大丈夫
// 今回はソート基準を特別に定義していないので、デフォルト（time_tの小さい順）に並ぶ