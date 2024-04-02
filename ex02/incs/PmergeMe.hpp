#ifndef PMERGEME_HPP
# define PMERGEME_HPP

# include <iostream>
# include <vector>
# include <list>
# include <sstream>
# include <sys/time.h>

typedef std::vector<std::pair<int, int> > pVector;
typedef std::list<std::pair<int, int> > pList;

class PmergeMe
{
	public:
		~PmergeMe(void);

		PmergeMe(char **argv);
		void	run(void);

		class InvalidArgumentException : public std::exception
		{
			public:
				virtual const char *what(void) const throw();
		};

		class AlreadySortedException : public std::exception
		{
			public:
				virtual const char *what(void) const throw();
		};

	private:
		PmergeMe(void);
		PmergeMe(const PmergeMe &copy_src);
		PmergeMe &operator=(const PmergeMe &rhs);

		std::vector<int>	_vector;
		std::list<int>		_list;

		void	ft_printFirstLine(void);
		void	ft_printSecondLine(void);

		void	ft_printThirdLine(void);
		void	ft_vMergeInsertSort(void);
		void	ft_vSetMainChain(pVector &pv);
		void	ft_vInsertPendant(pVector &pv, int oddElement);
		size_t	ft_vbinarySearch(const int &value, size_t min, size_t max);
		size_t	ft_vfindFirstOccurrence(const int &value);

		void	ft_printLastLine(void);
		void	ft_lMergeInsertSort(void);
		void	ft_lSetMainChain(pList &pl);
		void	ft_lInsertPendant(pList &pl, int oddElement);
		std::list<int>::iterator	ft_lbinarySearch(const int &value, std::list<int>::iterator min, std::list<int>::iterator max);
		std::list<int>::iterator	ft_lfindFirstOccurrence(const int &value);

};

#endif
