#include "PmergeMe.hpp"

PmergeMe::PmergeMe(void)
{
	// std::cout << "PmergeMe default constructor called" << std::endl;
}

PmergeMe::PmergeMe(const PmergeMe &copy_src)
{
	// std::cout << "PmergeMe copy constructor called" << std::endl;
	*this = copy_src;
}

PmergeMe	&PmergeMe::operator=(const PmergeMe &rhs)
{
	// std::cout << "PmergeMe copy assignment operator called" << std::endl;
	if (this != &rhs)
	{
		this->_vector = rhs._vector;
		this->_list = rhs._list;
	}
	return (*this);
}

PmergeMe::~PmergeMe(void)
{
	// std::cout << "PmergeMe destructor called" << std::endl;
}

static bool	ft_isAllDigits(const char *str)
{
	if (str == NULL || str[0] == '\0')
		return (false);// 文字列が空またはnullptrの場合は数字ではないと判断

	for (int i = 0; str[i] != '\0'; i++)
	{
		if (!std::isdigit(str[i]))
			return (false);// 数字以外の文字が見つかった場合は数字ではないと判断
	}

	return (true);
}

template <typename T>
static void setContainer(T &container, char **argv)//char** const&とすると、ポインタが指す先のデータ（文字列の配列）は変更可能
{
	int	n;

	for (int i = 0; argv[i] != '\0'; i++)
	{
		std::stringstream	ss(argv[i]);
		ss >> n;
		if (ss.fail() || ft_isAllDigits(argv[i]) == false)//左でオーバーフロー、右で負の値やアルファベットを弾く
			throw PmergeMe::InvalidArgumentException();
		container.push_back(n);
	}
}

PmergeMe::PmergeMe(char **argv)
{
	setContainer<std::vector<int> >(_vector, argv);
	setContainer<std::list<int> >(_list, argv);

	size_t	i = 1;
	while (i < _vector.size())
	{
		if (_vector[i] < _vector[i - 1])//無効なインデックスのアクセス防止
			break;
		i++;
	}
	if (i == _vector.size())
		throw PmergeMe::AlreadySortedException();;  // 隣接する要素が逆順になっている場合はソートされていないと判断
}

void	PmergeMe::ft_printFirstLine(void)
{
	std::cout << "Before:	";

	for (std::vector<int>::iterator it = _vector.begin(); it != _vector.end(); it++)
		std::cout << *it << " ";
	std::cout << std::endl;
}

void	PmergeMe::ft_printSecondLine(void)
{
	std::cout << "After:	";

	std::vector<int> tmp;
	tmp = _vector;
	std::sort(tmp.begin(), tmp.end());
	for (std::vector<int>::iterator it = tmp.begin(); it != tmp.end(); it++)
		std::cout << *it << " ";
	std::cout << std::endl;
}

static void	ft_pvMergeSort2(pVector &left, pVector &right, pVector &merged)
{
	size_t	i = 0;
	size_t	j = 0;
	size_t	k = 0;

	while (i < left.size() && j < right.size())
	{
		if (left[i].first < right[j].first)
			merged[k++] = left[i++];
		else
			merged[k++] = right[j++];
	}
	while (i < left.size())
		merged[k++] = left[i++];
	while (j < right.size())
		merged[k++] = right[j++];
}

static void	ft_pvMergeSort(pVector &pv)
{
	pVector left(pv.begin(), pv.begin() + pv.size() / 2);
	pVector right(pv.begin() + pv.size() / 2, pv.end());

	if (pv.size() <= 1)
		return ;
	ft_pvMergeSort(left);
	ft_pvMergeSort(right);
	ft_pvMergeSort2(left, right, pv);
}

void	PmergeMe::ft_vSetMainChain(pVector &sorted)
{
	_vector.clear();
	for (pVector::iterator it = sorted.begin(); it != sorted.end(); ++it)
	{
		_vector.push_back(it->first);
		// std::cout << _vector.back() << std::endl;//デバック
	}
}

static size_t	ft_getIndexFrom(size_t pvSize)
{
	static std::vector<size_t> num(2, 1);//static配列でヤコブスタール数を保持する
	num.push_back(num[num.size()-2]*2+num[num.size()-1]);
	return (num.back() > pvSize ? pvSize : num.back());
}

size_t	PmergeMe::ft_vbinarySearch(const int &value, size_t min, size_t max)
{
	if (max <= min)
		return (value > _vector[min] ? min + 1 : min);

	size_t mid = (min + max) / 2;

	if (value == _vector[mid])
		return (mid + 1);
	if (value > _vector[mid])
		return (ft_vbinarySearch(value, mid + 1, max));
	return ft_vbinarySearch(value, min, mid > 0 ? mid - 1 : mid);//indexが負の値にならないよう注意
}

size_t	PmergeMe::ft_vfindFirstOccurrence(const int &value)
{
	for (size_t i = 0; i < _vector.size(); ++i)
	{
		if (_vector[i] == value)
			return (i); // 最初のインデックスを返す
	}
	return (0);
}

void	PmergeMe::ft_vInsertPendant(pVector &pv, int oddElement)
{
	_vector.insert(_vector.begin(), pv[0].second);//まずペンド要素の最初の要素をメインチェーンに挿入する
	// std::cout << pv[0].second << std::endl;//デバック  pvの要素はずっと変わらない！
	// std::cout << _vector[0] << std::endl;//デバック
	size_t	start = 1;
	size_t	from = 0;
	size_t	pos = 0;
	while (from < pv.size())
	{
		from = ft_getIndexFrom(pv.size());
		// std::cout << from << std::endl;//デバック
		for (size_t i = from - 1; i >= start; --i)
		{
			// std::cout << ft_findFirstOccurrence(pv[i].first) << std::endl;//デバック
			pos = ft_vbinarySearch(pv[i].second, 0, ft_vfindFirstOccurrence(pv[i].first));//探す範囲の最大値はペアのindex
			// std::cout << pv[1].second << std::endl;//デバック
			_vector.insert(_vector.begin() + pos, pv[i].second);
			// for (std::vector<int>::iterator it = _vector.begin(); it != _vector.end(); ++it)
			// 	std::cout << *it << " ";
			// std::cout << std::endl;//デバック
		}
		start = from;
	}
	if (oddElement != -1)//最後に余った要素を挿入して完成
	{
		// std::cout << oddElement << std::endl;//デバック
		pos = ft_vbinarySearch(oddElement, 0, _vector.size());
		// std::cout << pos << std::endl;//デバック
		_vector.insert(_vector.begin() + pos, oddElement);
	}
}

void	PmergeMe::ft_vMergeInsertSort(void)
{
	pVector	pv;
	int			oddElement = -1;
	if (_vector.size() % 2 != 0)
	{
		oddElement = _vector[_vector.size() - 1];
		// std::cout << oddElement << std::endl;//デバック
		_vector.pop_back();
	}
	for (size_t i = 0; i < _vector.size(); i = i + 2)
	{
		pv.push_back(std::make_pair(_vector[i], _vector[i + 1]));
		if (pv.back().first < pv.back().second)
			std::swap(pv.back().first, pv.back().second);
		// std::cout << pv.back().first << std::endl;//デバック
	}
	ft_pvMergeSort(pv);
	ft_vSetMainChain(pv);
	ft_vInsertPendant(pv, oddElement);
}

void	PmergeMe::ft_printThirdLine(void)
{
	struct timeval start, end;
	gettimeofday(&start, NULL);
	ft_vMergeInsertSort();
	gettimeofday(&end, NULL);
	long long startUs = start.tv_sec * 1000000LL + start.tv_usec;
	long long endUs = end.tv_sec * 1000000LL + end.tv_usec;
	double time = static_cast<double>(endUs - startUs);
	std::cout << "Time to process a range of " << _vector.size() << " elements with std::vector	: " << time << " us" << std::endl;
}

static void	ft_plMergeSort2(pList&  left, pList&  right, pList& merged)
{
	pList::iterator it1 = left.begin();
	pList::iterator it2 = right.begin();
	pList::iterator it = merged.begin();

	while (it1 != left.end() && it2 != right.end())
	{
		if (it1->first <= it2->first)
		{
			*it = *it1;
			++it1;
		}
		else
		{
			*it = *it2;
			++it2;
		}
		++it;
	}
	while (it1 != left.end())
	{
		*it = *it1;
		++it;
		++it1;
	}
	while (it2 != right.end())
	{
		*it = *it2;
		++it;
		++it2;
	}
}

static void	ft_plMergeSort(pList &pl)
{
	pList::iterator it = pl.begin();
	std::advance(it, pl.size() / 2);

	pList left(pl.begin(), it);
	pList right(it, pl.end());

	if (pl.size() <= 1)
		return ;
	ft_plMergeSort(left);
	ft_plMergeSort(right);
	ft_plMergeSort2(left, right, pl);
}

void	PmergeMe::ft_lSetMainChain(pList &sorted)
{
	_list.clear();
	for (pList::iterator it = sorted.begin(); it != sorted.end(); ++it)
	{
		_list.push_back(it->first);
		// std::cout << it->first << std::endl;//デバック
	}
}

static bool ft_isAhead(std::list<int>::iterator it1, std::list<int>::iterator it2, std::list<int>& list)
{
	if (it1 == it2)
		return (false);
	while (it1 != list.end())
	{
		if (it1 == it2)
			return (true);
		++it1;
	}
	return (false);
}

std::list<int>::iterator	PmergeMe::ft_lbinarySearch(const int &value, std::list<int>::iterator min, std::list<int>::iterator max)
{
	if (!ft_isAhead(min, max, _list))
		return (value > *min ? ++min : min);

	std::list<int>::iterator mid = min;
	std::advance(mid, std::distance(min, max) / 2);

	if (value == *mid)
		return (++mid);
	if (value > *mid)
		return (ft_lbinarySearch(value, ++mid, max));
	return (ft_lbinarySearch(value, min, mid != _list.begin() ? --mid : mid));
}

std::list<int>::iterator	PmergeMe::ft_lfindFirstOccurrence(const int &value)
{
	std::list<int>::iterator it = _list.begin();
	while (it != _list.end())
	{
		if (*it == value)
			return (it); // 最初のイテレータを返す
		++it;
	}
	return (_list.end()); // 見つからない場合は終端イテレータを返す
}

void	PmergeMe::ft_lInsertPendant(pList &pl, int oddElement)
{
	pList::iterator	start_it = pl.begin();
	_list.insert(_list.begin(), start_it->second);//まずペンド要素の最初の要素をメインチェーンに挿入する

	pList::iterator	from_it = start_it;

	std::list<int>::iterator	pos_it = _list.begin();
	while (from_it != --pl.end())//.endは終端を指す。--することで最後の要素を指す
	{
		from_it = pl.begin();
		std::advance(from_it, ft_getIndexFrom(pl.size()) - 1);
		for (pList::iterator it = from_it; it != start_it; --it)
		{
			pos_it = ft_lbinarySearch(it->second, _list.begin(), ft_lfindFirstOccurrence(it->first));//探す範囲の最大値はペアのindex
			_list.insert(pos_it, it->second);
		}
		start_it = from_it;
	}
	if (oddElement != -1)//最後に余った要素を挿入して完成
	{
		// std::cout << oddElement << std::endl;//デバック
		pos_it = ft_lbinarySearch(oddElement, _list.begin(), --_list.end());
		// std::cout << pos << std::endl;//デバック
		_list.insert(pos_it, oddElement);
	}
}

void	PmergeMe::ft_lMergeInsertSort(void)
{
	pList		pl;
	int			oddElement = -1;
	if (_list.size() % 2 != 0)
	{
		oddElement = _list.back();
		// std::cout << oddElement << std::endl;//デバック
		_list.pop_back();
	}
	for (std::list<int>::iterator it = _list.begin(); it != _list.end(); std::advance(it, 1))
	{
		int first = *it;
		std::advance(it, 1);//it++でも可
		int second = *it;
		if (first < second)
			std::swap(first, second);
		pl.push_back(std::make_pair(first, second));
		// std::cout << pl.back().first << std::endl;//デバック
	}
	ft_plMergeSort(pl);
	ft_lSetMainChain(pl);
	ft_lInsertPendant(pl, oddElement);
}

void	PmergeMe::ft_printLastLine(void)
{
	struct timeval start, end;
	gettimeofday(&start, NULL);
	ft_lMergeInsertSort();
	gettimeofday(&end, NULL);
	long long startUs = start.tv_sec * 1000000LL + start.tv_usec;
	long long endUs = end.tv_sec * 1000000LL + end.tv_usec;
	double time = static_cast<double>(endUs - startUs);
	std::cout << "Time to process a range of " << _vector.size() << " elements with std::list	: " << time << " us" << std::endl;
}

void	PmergeMe::run(void)
{
	ft_printFirstLine();
	ft_printSecondLine();
	ft_printThirdLine();
	// std::cout << "After:	";
	// std::vector<int> tmp;
	// tmp = _vector;
	// for (std::vector<int>::iterator it = tmp.begin(); it != tmp.end(); it++)
	// 	std::cout << *it << " ";
	// std::cout << std::endl;//デバック
	ft_printLastLine();
	// std::cout << "After:	";
	// std::list<int> tmp;
	// tmp = _list;
	// for (std::list<int>::iterator it = tmp.begin(); it != tmp.end(); it++)
	// 	std::cout << *it << " ";
	// std::cout << std::endl;//デバック
}

const char *PmergeMe::InvalidArgumentException::what(void) const throw()
{
	return ("Error: Invalid arguments.");
}

const char *PmergeMe::AlreadySortedException::what(void) const throw()
{
	return ("Error: Already sorted.");
}
