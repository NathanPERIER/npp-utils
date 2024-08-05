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

/*
 * Basically, this is a change of frame. In the original algorithm, we lay the items of
 * the original string along the x axis and the items of the modified string along the
 * y axis. We then compute the SES as a path in that space.
 * 
 * The problem is that this search space is costly to construct (n * m cells). In order to
 * mitigate this effect, we introduce the variable k = x - y and the depth d, which counts
 * the number of edits on a given path. We then use a theorem to solve the problem in that
 * frame, as described in Eugene W. Myers' original paper. To put it simply, any SES of
 * length D is an SES of length D-1 followed by an edit and zero or more unchaged characters.
 * By storing the maximum value of x reachable for any path of lenght D-1, we can get the
 * longest path for length D, which corresponds to the SES. The search space is now a triangle,
 * the size of which is proportional to the number of edits.
 */

std::stack<::sym_vec> myers_ses(const std::vector<std::string_view>& a, const std::vector<std::string_view>& b) {
	std::stack<::sym_vec> res;
	res.emplace(1);
	res.top()[-1] = 0; // Probably useless
	res.top()[0]  = 0; // Probably useless
	res.top()[1]  = 0;
	for(size_t d = 0; d <= a.size() + b.size(); d++) {
		// fmt::print("======= d={} =========\n", d);
		const ::sym_vec& prev_v = res.top();
		::sym_vec& v = res.emplace(d == 0 ? 1 : d);
		for(int32_t k = -static_cast<int32_t>(d); k <= static_cast<int32_t>(d); k += 2) {
			// fmt::print("[k={}] ", k);
			size_t x;
			if(k == -static_cast<int32_t>(d) || (k != static_cast<int32_t>(d) && prev_v[k-1] < prev_v[k+1])) {
				// int32_t prev_x = static_cast<int32_t>(prev_v[k+1]);
				// fmt::print("({}, {})", prev_x, prev_x - (k+1));
				x = prev_v[k+1];
			} else {
				// int32_t prev_x = static_cast<int32_t>(prev_v[k-1]);
				// fmt::print("({}, {})", prev_x, prev_x - (k-1));
				x = prev_v[k-1] + 1;
			}
			size_t y = static_cast<size_t>(static_cast<int32_t>(x) - k);
			while(x < a.size() && y < b.size() && a[x] == b[y]) {
				x++;
				y++;
			}
			v[k] = x;
			// fmt::print(" -> ({}, {})\n", x, y);
			if(x >= a.size() && y >= b.size()) {
				return res;
			}
		}
		// fmt::print("\n");
	}
	throw npp::unreachable();
}

bool myers_diff(const std::vector<std::string_view>& a, const std::vector<std::string_view>& b, std::stack<edit_op>& path) {
	bool modified = false;
	std::stack<::sym_vec> ses = myers_ses(a, b);

	// fmt::print("Paths OK\n\n");

	int32_t x = static_cast<int32_t>(a.size());
	int32_t y = static_cast<int32_t>(b.size());
	while(x > 0 || y > 0) {
		int32_t d = static_cast<int32_t>(ses.size()) - 2;
		int32_t k = x - y;
		ses.pop();
		// fmt::print("[k={}, d={}] ", k, d);
		if(k == -d || (k != d && ses.top()[k-1] < ses.top()[k+1])) {
			k++;
		} else {
			k--;
		}
		int32_t prev_x = static_cast<int32_t>(ses.top()[k]);
		int32_t prev_y = prev_x - k;
		while(x > prev_x && y > prev_y) {
			x--;
			y--;
			path.push(edit_op::keep);
		}
		// fmt::print("({}, {}) -> ({}, {})\n", prev_x, prev_y, x, y);
		if(d > 0) {
			if(x == prev_x) {
				path.push(edit_op::add);
			} else {
				path.push(edit_op::del);
			}
			modified = true;
		}
		x = prev_x;
		y = prev_y;
	}
	return modified;
}


std::optional<std::stack<edit_op>> edit_path(const std::vector<std::string_view>& a, const std::vector<std::string_view>& b) {
	std::stack<edit_op> res;
	const bool modified = myers_diff(a, b, res);
	return modified ? std::optional(std::move(res)) : std::nullopt;
}

std::optional<std::stack<edit_op>> edit_path(std::string_view original, std::string_view edited) {
	std::vector<std::string_view> a = npp::split(original, "\n").to<std::vector<std::string_view>>();
	std::vector<std::string_view> b = npp::split(edited,   "\n").to<std::vector<std::string_view>>();
	return edit_path(a, b);
}

bool diff(std::string_view original, std::string_view edited) {
	std::vector<std::string_view> a = npp::split(original, "\n").to<std::vector<std::string_view>>();
	std::vector<std::string_view> b = npp::split(edited,   "\n").to<std::vector<std::string_view>>();
	std::optional<std::stack<edit_op>> path = edit_path(a, b);
	if(path == std::nullopt) {
		return false;
	}
	size_t a_idx = 0;
	size_t b_idx = 0;
	while(!path->empty()) {
		switch(path->top()) {
			case edit_op::keep: fmt::print(" {}\n", a[a_idx]); a_idx++; b_idx++; break;
			case edit_op::add:  fmt::print("+{}\n", b[b_idx]);          b_idx++; break;
			case edit_op::del:  fmt::print("-{}\n", a[a_idx]); a_idx++;          break;
			default: npp::unreachable();
		}
		path->pop();
	}
	return true;
}

} // namespace npp
