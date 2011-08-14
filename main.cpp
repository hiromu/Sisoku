#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>

using namespace std;

#define ANSWER 10

class fraction {
private:
	int l;
public:
	int m, n;
	fraction(): m(0), n(1){}
	fraction(int i): m(i), n(1) {}
	fraction(int i, int j) {
		if(j == 0) {
			m = 0;
			n = 1;
		} else {
			l = __gcd(i, j);
			m = i / l;
			n = j / l;
		}
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
/*
		cout << "m: " << m << endl;
		cout << "n: " << n << endl;
		cout << "o.m: " << o.m << endl;
		cout << "o.n: " << o.n << endl;
		cout << "i: " << m * o.n << endl;
		cout << "j: " << n * o.m << endl;
*/
		return (o.m * n == m * o.n);
	}
	fraction clone() {
		return fraction(m, n);
	}
	string intn() {
		stringstream ss;
		if(!n)
			return "0";
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

string get_string(vector<int> &mark, vector<fraction> &num)
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

fraction next_number(vector<int> &mark, vector<fraction> &num)
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
	if(num.size() > 2) {
		for(i = num.size() - 2; i > 0; i--) {
			mark[i]++;
			if(mark[i] == 4) {
				mark[i - 1]++;
				mark[i] = 0;
			}
		}
	} else if(num.size() == 2) {
		mark[0]++;
	}
	return res;
}

bool solve(vector<fraction> nums, fraction n, string &res, int first = 0)
{
	int i, j, count, flag;
	fraction m;
	string tmp[6];
	stringstream ss;
	vector<fraction> list, a, b, c;
#ifdef DEBUG
	for(i = 0; i < nums.size(); i++)
		cout << nums[i].intn() << " ";
	cout << endl;
	cout << n.intn() << endl;
#endif
	if(nums.size() == 2) {
		flag = true;
		res = "";
#ifdef PARALLEL
#pragma omp parallel for
#endif
		for(i = 0; i < 6; i++) {
			tmp[i] = "";
			if(flag) {
				if(nums[0] == n - nums[1])
					tmp[i] = "(" + nums[0].intn() + "+" + nums[1].intn() + ")";
				else if(nums[0] == n + nums[1])
					tmp[i] = "(" + nums[0].intn() + "-" + nums[1].intn() + ")";
				else if(!(nums[1] == fraction(0)) && nums[0] == n / nums[1])
					tmp[i] = "(" + nums[0].intn() + "*" + nums[1].intn() + ")";
				else if(!(nums[1] == fraction(0)) && nums[0] == n * nums[1])
					tmp[i] = "(" + nums[0].intn() + "/" + nums[1].intn() + ")";
				else if(nums[1] == n + nums[0])
					tmp[i] = "(" + nums[1].intn() + "-" + nums[0].intn() + ")";
				else if(!(nums[0] == fraction(0)) && nums[1] == n * nums[0])
					tmp[i] = "(" + nums[1].intn() + "/" + nums[0].intn() + ")";
				else
					continue;
				flag = false;
			}
		}
		if(!flag) {
			for(i = 0; i < 6; i++) {
				if(tmp[i] != "") {
					res = tmp[i];
					break;
				}
			}
			return true;
		}
	} else {
		list.assign(nums.begin(), nums.end());
		for(i = 1; i <= nums.size() / 2; i++) {
			nums.assign(list.begin(), list.end());
			do {
				a.assign(nums.begin(), nums.begin() + i);
				b.assign(nums.begin() + i, nums.end());
				vector<int> mark;
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
					flag = true;
					res = "";
					for(j = 0; j < 6; j++) {
						tmp[j] = "";
						if(flag) {
							if(j == 0 && solve(b, n - m, tmp[j]))
								tmp[j] = "(" + tmp[j] + "+" + get_string(mark, a) + ")";
							else if(j == 1 && solve(b, n + m, tmp[j]))
								tmp[j] = "(" + tmp[j] + "-" + get_string(mark, a) + ")";
							else if(j == 2 && !(m == fraction(0)) && solve(b, n / m, tmp[j]))
								tmp[j] = "(" + tmp[j] + "*" + get_string(mark, a) + ")";
							else if(j == 3 && !(m == fraction(0)) && solve(b, n * m, tmp[j]))
								tmp[j] = "(" + tmp[j] + "/" + get_string(mark, a) + ")";
							else if(j == 4 && solve(b, m - n, tmp[j]))
								tmp[j] = "(" + get_string(mark, a) + "-" + tmp[j] + ")";
							else if(j == 5 && !(m == fraction(0)) && solve(b, m / n, tmp[j]))
								tmp[j] = "(" + get_string(mark, a) + "/" + tmp[j] + ")";
							else
								continue;
							flag = false;
						}
					}
					if(!flag) {
						for(j = 0; j < 6; j++) {
							if(tmp[j] != "") {
								res = tmp[j];
								break;
							}
						}
						return true;
					}
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
	while(true) {
		vector<fraction> nums;
		cin >> n;
		if(cin.eof())
			break;
		for(i = 0; i < n; i++) {
			cin >> j;
			nums.push_back(fraction(j));
		}
		sort(nums.begin(), nums.end(), sortpattern());
		solve(nums, fraction(ANSWER), result, 1);
		cout << result << endl;
	}
	return 0;
}
