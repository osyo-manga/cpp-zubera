#include "../lib/zubera.hpp"
#include <cstdlib>
#include <thread>
#include <string>
#include <cassert>


int
main(){
	using table_t = zubera::vector<zubera::vector<bool>>;
	table_t table = {
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	};

	auto width = [](auto const& table){
		return table.first()->count();
	};

	auto height = [](auto const& table){
		return table.count();
	};

	auto at_cell = [&](auto const& table, auto x, auto y){
		return x < 0 || width(table)  <= x ? 0
			 : y < 0 || height(table) <= y ? 0
			 : table[y][x];
	};

	auto dead_or_alive = [&](auto table, auto x, auto y){
		auto cell  = at_cell(table, x, y);
		auto count = at_cell(table, x - 1, y - 1)
				   + at_cell(table, x - 0, y - 1)
				   + at_cell(table, x + 1, y - 1)
				   + at_cell(table, x + 1, y - 0)
				   + at_cell(table, x + 1, y + 1)
				   + at_cell(table, x - 0, y + 1)
				   + at_cell(table, x - 1, y + 1)
				   + at_cell(table, x - 1, y - 0);
		return cell ? count == 3 || count == 2 : count == 3;
	};

	auto update = [&](auto const& table){
		return table.map().with_index([&](auto const& row, auto y){
			return row.map().with_index([&](auto, auto x){
				return dead_or_alive(table, x, y);
			});
		});
	};
	
	auto print = [&](auto const& table){
		std::system("clear");
		
		std::cout << std::string(width(table) + 2, '-') << "\n";
		table.each([](auto const& row){
			std::cout << "|";
			row.each([](auto cell){
				std::cout << (cell ? "*" : " ");
			});
			std::cout << "|\n";
		});
		std::cout << std::string(width(table) + 2, '-') << "\n";
		std::cout << std::endl;
	};

	while(1){
		print(table);
		table = update(table);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	return 0;
}
