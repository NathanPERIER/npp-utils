
#pragma once

#include <string_view>
#include <stack>


namespace npp {

enum class edit_op {
	keep,
	add,
	del
};

std::stack<edit_op> edit_path(std::string_view original, std::string_view edited);

} // namespace npp