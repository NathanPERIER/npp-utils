#include "npp/string/diff.hh"

#include <vector>

#include <fmt/format.h>

#include "npp/error.hh"
#include "npp/string/split.hh"


namespace {

class sym_vec {

public:
	sym_vec(size_t half_size): _half_size(half_size), _buf(half_size*2 + 1) {}

	size_t operator[](int32_t i) const {
		return _buf[static_cast<size_t>(i + _half_size)];
	}

	size_t& operator[](int32_t i) {
		return _buf[static_cast<size_t>(i + _half_size)];
	}

private:
	size_t _half_size;
	std::vector<size_t> _buf;

};

} // anonymous namespaces


namespace npp {

/* Myers diff O(ND) algorithm
 *
 * let a: Array [0 .. N[
 * let b: Array [0 .. M[
 * let MAX in [0, M+N]
 *
 * v: Array [- MAX .. MAX]
 * v[1] <- 0
 * for d from 0 to MAX
 *  |  for k from -d to d steps 2
 *  |    |  if k == -d or k != d and v[k-1] < v[k+1]
 *  |    |    |  x <- v[k+1]
 *  |    |  else
 *  |    |    |  x <- v[k-1] + 1
 *  |    |  end_if
 *  |    |  y <- x-k
 *  |    |  while x < N and y < M and a[x+1] == b[y+1]
 *  |    |    |  x++
 *  |    |    |  y++
 *  |    |  end_while
 *  |    |  v[k] = x
 *  |    |  if x >= N and y >= M
 *  |    |    | yield // Length of an SES is d
 *  |    |  end_if
 *  |  end_for
 * end_for
 * fail // Length of an SES is greater than MAX
 * 
 * TN: SES => Shortest Edit Script
 */

std::stack<::sym_vec> myers_ses(const std::vector<std::string_view>& a, const std::vector<std::string_view>& b) {
	std::stack<::sym_vec> res;
	res.emplace(1);
	res.top()[-1] = 0; // Probably useless
	res.top()[0]  = 0; // Probably useless
	res.top()[1]  = 0;
	for(size_t d = 0; d <= a.size() + b.size(); d++) {
		const ::sym_vec& prev_v = res.top();
		::sym_vec& v = res.emplace(d == 0 ? 1 : d);
		for(int32_t k = -static_cast<int32_t>(d); k <= static_cast<int32_t>(d); k += 2) {
			size_t x;
			if(k == -static_cast<int32_t>(d) || (k != static_cast<int32_t>(d) && prev_v[k-1] < prev_v[k+1])) {
				x = prev_v[k+1];
			} else {
				x = prev_v[k-1] + 1;
			}
			size_t y = static_cast<size_t>(static_cast<int32_t>(x) - k);
			while(x < a.size() && y < b.size() && a[x] == b[y]) {
				x++;
				y++;
			}
			v[k] = x;
			if(x >= a.size() && y >= b.size()) {
				return res;
			}
		}
	}
	throw npp::unreachable();
}

std::stack<edit_op> myers_diff(const std::vector<std::string_view>& a, const std::vector<std::string_view>& b) {
	std::stack<::sym_vec> ses = myers_ses(a, b);

	(void) ses; // TODO unwind stack

	return std::stack<edit_op>();
}


std::stack<edit_op> edit_path(std::string_view original, std::string_view edited) {
	std::vector<std::string_view> a = npp::split(original, "\n").to<std::vector<std::string_view>>();
	std::vector<std::string_view> b = npp::split(edited,   "\n").to<std::vector<std::string_view>>();
	return myers_diff(a, b);
}

} // namespace npp
