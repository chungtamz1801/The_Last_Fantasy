#include <cctype>
#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// ============================================================
// Trim whitespace
// ============================================================

string trim(const string &str) {
  size_t start = 0;
  size_t end = str.size();

  while (start < end && isspace(static_cast<unsigned char>(str[start]))) {
    ++start;
  }

  while (end > start && isspace(static_cast<unsigned char>(str[end - 1]))) {
    --end;
  }

  return str.substr(start, end - start);
}

// ============================================================
// Parse one CSV line
//
// Supports:
//
//   a,b,c
//   "hello, world",25,test
//   "hello ""world""",test
// ============================================================

vector<string> parseCSVLine(const string &line) {
  vector<string> fields;

  string field;
  bool insideQuotes = false;

  for (size_t i = 0; i < line.size(); ++i) {
    char c = line[i];

    if (c == '"') {
      if (insideQuotes) {
        // Escaped quote: ""
        if (i + 1 < line.size() && line[i + 1] == '"') {
          field += '"';
          ++i;
        } else {
          insideQuotes = false;
        }
      } else {
        insideQuotes = true;
      }
    } else if (c == ',' && !insideQuotes) {
      fields.push_back(field);
      field.clear();
    } else {
      field += c;
    }
  }

  fields.push_back(field);

  return fields;
}

// ============================================================
// Escape string for JSON
// ============================================================

string escapeJSON(const string &value) {
  string result;

  for (char c : value) {
    switch (c) {
    case '"':
      result += "\\\"";
      break;

    case '\\':
      result += "\\\\";
      break;

    case '\b':
      result += "\\b";
      break;

    case '\f':
      result += "\\f";
      break;

    case '\n':
      result += "\\n";
      break;

    case '\r':
      result += "\\r";
      break;

    case '\t':
      result += "\\t";
      break;

    default:
      // Control characters
      if (static_cast<unsigned char>(c) < 0x20) {
        char buffer[8];

        snprintf(buffer, sizeof(buffer), "\\u%04x",
                 static_cast<unsigned char>(c));

        result += buffer;
      } else {
        result += c;
      }

      break;
    }
  }

  return result;
}

// ============================================================
// Check if string is integer
// ============================================================

bool isInteger(const string &value) {
  if (value.empty())
    return false;

  size_t i = 0;

  if (value[0] == '-' || value[0] == '+')
    i = 1;

  if (i >= value.size())
    return false;

  for (; i < value.size(); ++i) {
    if (!isdigit(static_cast<unsigned char>(value[i])))
      return false;
  }

  return true;
}

// ============================================================
// Check if string is floating point
// ============================================================

bool isDouble(const string &value) {
  if (value.empty())
    return false;

  char *end = nullptr;

  errno = 0;

  const char *start = value.c_str();

  double number = strtod(start, &end);

  (void)number;

  if (errno == ERANGE)
    return false;

  return end != start && *end == '\0';
}

// ============================================================
// Convert CSV value to JSON value
//
// Examples:
//
//   123       -> 123
//   3.14      -> 3.14
//   true      -> true
//   false     -> false
//   null      -> null
//   hello     -> "hello"
// ============================================================

string toJSONValue(const string &originalValue) {
  string value = trim(originalValue);

  // Empty string
  if (value.empty()) {
    return "\"\"";
  }

  // Boolean
  if (value == "true" || value == "TRUE") {
    return "true";
  }

  if (value == "false" || value == "FALSE") {
    return "false";
  }

  // Null
  if (value == "null" || value == "NULL") {
    return "null";
  }

  // Integer
  if (isInteger(value)) {
    return value;
  }

  // Floating point
  if (isDouble(value)) {
    return value;
  }

  // String
  return "\"" + escapeJSON(originalValue) + "\"";
}

// ============================================================
// Write indentation
// ============================================================

void writeIndent(ofstream &output, int level) {
  for (int i = 0; i < level; ++i) {
    output << "    ";
  }
}

// ============================================================
// Main
// ============================================================

int main(int argc, char *argv[]) {
  // --------------------------------------------------------
  // File names
  // --------------------------------------------------------

  string inputFile = "input.csv";
  string outputFile = "output.json";

  if (argc >= 2) {
    inputFile = argv[1];
  }

  if (argc >= 3) {
    outputFile = argv[2];
  }

  // --------------------------------------------------------
  // Open CSV
  // --------------------------------------------------------

  ifstream csvFile(inputFile);

  if (!csvFile.is_open()) {
    cerr << "Error: Cannot open input file: " << inputFile << '\n';

    return 1;
  }

  // --------------------------------------------------------
  // Read header
  // --------------------------------------------------------

  string line;

  if (!getline(csvFile, line)) {
    cerr << "Error: CSV file is empty.\n";
    return 1;
  }

  // Remove UTF-8 BOM if present
  if (line.size() >= 3 && static_cast<unsigned char>(line[0]) == 0xEF &&
      static_cast<unsigned char>(line[1]) == 0xBB &&
      static_cast<unsigned char>(line[2]) == 0xBF) {
    line.erase(0, 3);
  }

  vector<string> headers = parseCSVLine(line);

  if (headers.empty()) {
    cerr << "Error: No CSV headers found.\n";
    return 1;
  }

  // --------------------------------------------------------
  // Open JSON output
  // --------------------------------------------------------

  ofstream jsonFile(outputFile);

  if (!jsonFile.is_open()) {
    cerr << "Error: Cannot create output file: " << outputFile << '\n';

    return 1;
  }

  // --------------------------------------------------------
  // Start JSON array
  // --------------------------------------------------------

  jsonFile << "[\n";

  size_t rowCount = 0;

  // --------------------------------------------------------
  // Read CSV rows
  // --------------------------------------------------------

  while (getline(csvFile, line)) {
    // Skip completely empty lines
    if (line.empty())
      continue;

    vector<string> values = parseCSVLine(line);

    // Comma between JSON objects
    if (rowCount > 0) {
      jsonFile << ",\n";
    }

    jsonFile << "    {\n";

    // ----------------------------------------------------
    // Write fields
    // ----------------------------------------------------

    for (size_t i = 0; i < headers.size(); ++i) {
      string header = trim(headers[i]);

      string value;

      if (i < values.size()) {
        value = values[i];
      } else {
        value = "";
      }

      jsonFile << "        \"" << escapeJSON(header)
               << "\": " << toJSONValue(value);

      if (i + 1 < headers.size()) {
        jsonFile << ",";
      }

      jsonFile << "\n";
    }

    jsonFile << "    }";

    ++rowCount;
  }

  // --------------------------------------------------------
  // End JSON array
  // --------------------------------------------------------

  if (rowCount > 0) {
    jsonFile << "\n";
  }

  jsonFile << "]\n";

  csvFile.close();
  jsonFile.close();

  // --------------------------------------------------------
  // Result
  // --------------------------------------------------------

  cout << "Conversion successful!\n";
  cout << "Input : " << inputFile << '\n';
  cout << "Output: " << outputFile << '\n';
  cout << "Rows  : " << rowCount << '\n';
  return 0;
}
