#include <iostream>
#include <string>
#include <vector>

#include <map>
#include <unordered_map>

struct CityRecord {
	std::string Name;
	uint64_t Population;
	double Latitude, Longitude;
};

int main() {
	std::vector<CityRecord> cities;

	cities.emplace_back("Melbourne", 5000000, 2.4, 9.4);
	cities.emplace_back("Lol-town", 5000000, 2.4, 9.4);
	cities.emplace_back("Berlin", 5000000, 2.4, 9.4);
	cities.emplace_back("Paris", 5000000, 2.4, 9.4);
	cities.emplace_back("London", 5000000, 2.4, 9.4);

	for (const auto& city : cities)
	{
		if (city.Name == "Berlin")
		{
			city.Population;
			break;
		}
	}

	std::map<std::string, CityRecord> cityMap;

	cityMap["Melbourne"] = CityRecord{ "Melbourne", 5000000, 2.4, 9.4 };
	cityMap["Lol-town"] = CityRecord{ "Lol-town", 5000000, 2.4, 9.4 };
	cityMap["Berlin"] = CityRecord{ "Berlin", 5000000, 2.4, 9.4 };
	cityMap["Paris"] = CityRecord{ "Paris", 5000000, 2.4, 9.4 };
	cityMap["London"] = CityRecord{ "London", 5000000, 2.4, 9.4 };

	CityRecord& berlinData = cityMap["Berlin"];
	std::cout << berlinData.Population;

	return 0;
}