#include <fstream>
#include <iostream>
#include <string>

#include "./third_party/nlohmann/json.hpp"

using json = nlohmann::json;

int main() {
  const std::string inputFile = "input.json";
  const std::string outputFile = "output.json";

  try {
    // ============================================
    // Đọc JSON file
    // ============================================

    std::ifstream input("input/" + inputFile);

    if (!input.is_open()) {
      std::cerr << "Cannot open: " << inputFile << std::endl;

      return 1;
    }

    json data;
    input >> data;

    input.close();

    std::cout << "Read JSON successfully.\n";

    //============================================
    // Đọc dữ liệu
    //============================================
    for (const auto &item : data) {
      if (item.contains("Name")) {
        std::cout << "Name: " << item["Name"] << std::endl;
      }

      if (item.contains("ATK")) {
        std::cout << "ATK: " << item["ATK"] << std::endl;
      }
    }

    //
    //   // ============================================
    //   // Thay đổi dữ liệu
    //   // ============================================
    //
    //   data["name"] = "Bob";
    //   data["age"] = 30;
    //   data["city"] = "Dalat";
    //   data["active"] = true;
    //
    //   // ============================================
    //   // Ghi JSON file
    //   // ============================================
    //
    //   std::ofstream output(outputFile);
    //
    //   if (!output.is_open()) {
    //     std::cerr << "Cannot create: " << outputFile << std::endl;
    //
    //     return 1;
    //   }
    //
    //   // 4 = indentation
    //   output << data.dump(4);
    //
    //   output.close();
    //
    //   std::cout << "Write JSON successfully.\n";
    //   std::cout << "Output: " << outputFile << std::endl;
    // } catch (const json::parse_error &e) {
    //   std::cerr << "JSON parse error: " << e.what() << std::endl;
    //
    //   return 1;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;

    return 1;
  }

  return 0;
}
