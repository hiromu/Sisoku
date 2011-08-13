#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

#define DEBUG 0
#define ANSWER 10

class fraction {
private:
	int l;
public:
	int m, n;
	fraction(): m(0), n(1){}
	fraction(int i): m(i), n(1) {}
	fraction(int i, int j) {
		l = __gcd(i, j);
		m = i / l;
		n = j / l;
	}
	fraction operator+(const fraction & o) {
		return fraction(m * o.n + o.m * n, n * o.n);
	}
	fraction operator-(const fraction & o) {
		return fraction(m * o.n - o.m * n, n * o.n);
	}
	fraction operator*(const fraction & o) {
		return fraction(m * o.m, n * o.n);
	}
	fraction operator/(const fraction & o) {
		return fraction(m * o.n, n * o.m);
	}
	bool operator<(const fraction & o) {
		return (m * o.n < o.m * n);
	}
	bool operator>(const fraction & o) {
		return (m * o.n > o.m * n);
	}
	bool operator==(const fraction & o) {
		return (o.m * n == m * o.n);
	}
	fraction clone() {
		return fraction(m, n);
	}
	string intn() {
		stringstream ss;
		ss << ((double)m / (double)n);
		return ss.str();
	} 
};

class sortpattern {
public:
	bool operator()(const fraction &l, const fraction &r) const{
		return l.m * r.n < r.m * l.n;
	}
};

typedef vector<fraction>::iterator FI;

bool make_combination(FI first1, FI last1, FI first2, FI last2)
{
 	if((first1 == last1) || (first2 == last2))
		return false;
	FI m1 = last1;
	FI m2 = last2;
	--m2;
	while(--m1 != first1 && !(*m1 < *m2));
	bool result = (m1 == first1) && !(*first1 < *m2);
	if(!result) {
		while(first2 != m2 && !(*m1 < *first2))
		      ++first2 ;
		first1 = m1;
		iter_swap(first1, first2);
		++first1;
		++first2;
	}
	if((first1 != last1) && (first2 != last2)) {
		m1 = last1;
		m2 = first2;
		while((m1 != first1) && (m2 != last2)) {
			iter_swap(--m1, m2);
			++m2;
		}
		reverse(first1, m1);
		reverse(first1, last1);
		reverse(m2, last2);
		reverse(first2, last2);
	}
	return !result ;
}

bool next_combination(FI a, FI b, FI c)
{
	return make_combination(a, b, b, c);
}

string get_string(vector<int> mark, vector<fraction> num)
{
	int i;
	string str;
	if(num.size() > 1)
		str = "(";
	else
		str = "";
	str += num[0].intn();
	for(i = 1; i < num.size(); i++) {
		if(mark[i - 1] == 0)
			str += "+";
		else if(mark[i - 1] == 1)
			str += "-";
		else if(mark[i - 1] == 2)
			str += "*";
		else if(mark[i - 1] == 3)
			str += "/";
		str += num[i].intn();
	}
	if(num.size() > 1)
		str += ")";
	return str;
}

fraction next_number(vector<int> &mark, vector<fraction> num)
{
	int i;
	fraction res;
	res = num[0].clone();
	for(i = 1; i < num.size(); i++) {
		if(mark[i - 1] == 0) {
			res = res + num[i];
		} else if(mark[i - 1] == 1) {
			res = res - num[i];
		} else if(mark[i - 1] == 2) {
			res = res * num[i];
		} else if(mark[i - 1] == 3) {
			res = res / num[i];
		}
	}
	if(num.size() > 1) {
		for(i = num.size() - 1; i > 0; i--) {
			mark[i]++;
			if(mark[i] == 4) {
				mark[i - 1]++;
				mark[i] = 0;
			}
		}
	}
	return res;
}

bool solve(vector<fraction> nums, fraction n, string &res)
{
	int i, j, count;
	fraction m;
	string tmp;
	stringstream ss;
	vector<fraction> list, a, b;
	vector<int> mark;
#if DEBUG
	for(i = 0; i < nums.size(); i++)
		cout << nums[i].intn() << " ";
	cout << endl;
	cout << n.intn() << endl;
#endif
	if(nums.size() == 2) {
		res = "";
		if(nums[0] == n - nums[1])
			res = "(" + nums[0].intn() + "+" + nums[1].intn() + ")";
		else if(nums[0] == n + nums[1])
			res = "(" + nums[0].intn() + "-" + nums[1].intn() + ")";
		else if(nums[0] == n / nums[1])
			res = "(" + nums[0].intn() + "*" + nums[1].intn() + ")";
		else if(nums[0] == n * nums[1])
			res = "(" + nums[0].intn() + "/" + nums[1].intn() + ")";
		else if(nums[1] == n + nums[0])
			res = "(" + nums[1].intn() + "-" + nums[0].intn() + ")";
		else if(nums[1] == n * nums[0])
			res = "(" + nums[1].intn() + "/" + nums[0].intn() + ")";
		if(res != "")
			return true;
	} else {
		list.assign(nums.begin(), nums.end());
		for(i = 1; i <= nums.size() / 2; i++) {
			nums.assign(list.begin(), list.end());
			do {
				a.assign(nums.begin(), nums.begin() + i);
				b.assign(nums.begin() + i, nums.end());
				for(j = 0; j < a.size() - 1; j++)
					mark.push_back(0);
				count = 0;
				while(true) {
					if(a.size() > 1) {
						m = next_number(mark, a);
						if(mark[0] == 4)
							break;
					} else {
						if(count > 0)
							break;
						m = a[0];
					}
					res = "";
					if(solve(b, n - m, tmp))
						res = "(" + tmp + "+" + get_string(mark, a) + ")";
					else if(solve(b, n + m, tmp))
						res = "(" + tmp + "-" + get_string(mark, a) + ")";
					else if(solve(b, n / m, tmp))
						res = "(" + tmp + "*" + get_string(mark, a) + ")";
					else if(solve(b, n * m, tmp))
						res = "(" + tmp + "/" + get_string(mark, a) + ")";
					else if(solve(b, m - n, tmp))
						res = "(" + get_string(mark, a) + "-" + tmp + ")";
					else if(solve(b, m / n, tmp))
						res = "(" + get_string(mark, a) + "/" + tmp + ")";
					if(res != "")
						return true;
					count++;
				}
			} while(next_combination(nums.begin(), nums.begin() + i, nums.end()));
		}
	}
	return false;
}

int main(void)
{
	int i, j, n;
	string result;
	vector<fraction> nums;
	cin >> n;
	for(i = 0; i < n; i++) {
		cin >> j;
		nums.push_back(fraction(j));
	}
	sort(nums.begin(), nums.end(), sortpattern());
	solve(nums, fraction(ANSWER), result);
	cout << result << endl;
	return 0;
}
