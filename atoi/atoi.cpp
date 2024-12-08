#include <iostream>
#include <climits>

int atoi(const char* str) {
	if (!str) return 0;

	while (std::isspace(*str)) ++str;

	int sign = 1;
	if (*str == '+' || *str == '-') {
		sign = (*str == '-') ? -1 : 1;
		++str;
	}

	int result = 0;
	while (std::isdigit(*str)) {
		int digit = *str - '0';

		if (result > (INT_MAX - digit) / 10) {
			return (sign == 1) ? INT_MAX : INT_MIN;
		}
		result = result * 10 + digit;
		++str;
	}
	return result * sign;
}

int main() {
	const char* str = "42";
	std::cout << atoi(str);
}
