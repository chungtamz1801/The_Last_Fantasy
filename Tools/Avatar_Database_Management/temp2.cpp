#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

using namespace std;

// ============================================================
// JSON Value
// ============================================================

class JsonValue {
public:
  using Object = map<string, JsonValue>;
  using Array = vector<JsonValue>;

  variant<nullptr_t, bool, double, string, Array, Object> value;

  JsonValue() : value(nullptr) {}
  JsonValue(nullptr_t) : value(nullptr) {}
  JsonValue(bool v) : value(v) {}
  JsonValue(double v) : value(v) {}
  JsonValue(int v) : value(static_cast<double>(v)) {}
  JsonValue(const string &v) : value(v) {}
  JsonValue(const char *v) : value(string(v)) {}
  JsonValue(const Array &v) : value(v) {}
  JsonValue(const Object &v) : value(v) {}
};

// ============================================================
// JSON Parser
// ============================================================

class JsonParser {
private:
  string text;
  size_t pos = 0;

public:
  JsonParser(const string &input) : text(input) {}

  JsonValue parse() {
    skipWhitespace();

    JsonValue result = parseValue();

    skipWhitespace();

    if (pos != text.size()) {
      throw runtime_error("Unexpected characters after JSON");
    }

    return result;
  }

private:
  void skipWhitespace() {
    while (pos < text.size() &&
           isspace(static_cast<unsigned char>(text[pos]))) {
      ++pos;
    }
  }

  char peek() {
    if (pos >= text.size())
      return '\0';

    return text[pos];
  }

  char get() {
    if (pos >= text.size())
      throw runtime_error("Unexpected end of JSON");

    return text[pos++];
  }

  void expect(char c) {
    if (get() != c) {
      throw runtime_error("Unexpected character");
    }
  }

  JsonValue parseValue() {
    skipWhitespace();

    char c = peek();

    if (c == '{')
      return parseObject();

    if (c == '[')
      return parseArray();

    if (c == '"')
      return JsonValue(parseString());

    if (c == 't') {
      parseLiteral("true");
      return JsonValue(true);
    }

    if (c == 'f') {
      parseLiteral("false");
      return JsonValue(false);
    }

    if (c == 'n') {
      parseLiteral("null");
      return JsonValue(nullptr);
    }

    if (c == '-' || isdigit(static_cast<unsigned char>(c))) {
      return JsonValue(parseNumber());
    }

    throw runtime_error("Invalid JSON value");
  }

  void parseLiteral(const string &literal) {
    for (char c : literal) {
      if (get() != c) {
        throw runtime_error("Invalid JSON literal");
      }
    }
  }

  string parseString() {
    expect('"');

    string result;

    while (true) {
      char c = get();

      if (c == '"')
        break;

      if (c == '\\') {
        char escaped = get();

        switch (escaped) {
        case '"':
          result += '"';
          break;

        case '\\':
          result += '\\';
          break;

        case '/':
          result += '/';
          break;

        case 'b':
          result += '\b';
          break;

        case 'f':
          result += '\f';
          break;

        case 'n':
          result += '\n';
          break;

        case 'r':
          result += '\r';
          break;

        case 't':
          result += '\t';
          break;

        default:
          throw runtime_error("Unsupported JSON escape sequence");
        }
      } else {
        result += c;
      }
    }

    return result;
  }

  double parseNumber() {
    size_t start = pos;

    if (peek() == '-')
      ++pos;

    while (isdigit(static_cast<unsigned char>(peek())))
      ++pos;

    if (peek() == '.') {
      ++pos;

      while (isdigit(static_cast<unsigned char>(peek())))
        ++pos;
    }

    if (peek() == 'e' || peek() == 'E') {
      ++pos;

      if (peek() == '+' || peek() == '-')
        ++pos;

      while (isdigit(static_cast<unsigned char>(peek())))
        ++pos;
    }

    return stod(text.substr(start, pos - start));
  }

  JsonValue parseArray() {
    expect('[');

    JsonValue::Array array;

    skipWhitespace();

    if (peek() == ']') {
      get();
      return JsonValue(array);
    }

    while (true) {
      array.push_back(parseValue());

      skipWhitespace();

      if (peek() == ']') {
        get();
        break;
      }

      expect(',');
    }

    return JsonValue(array);
  }

  JsonValue parseObject() {
    expect('{');

    JsonValue::Object object;

    skipWhitespace();

    if (peek() == '}') {
      get();
      return JsonValue(object);
    }

    while (true) {
      skipWhitespace();

      if (peek() != '"')
        throw runtime_error("JSON object key must be a string");

      string key = parseString();

      skipWhitespace();
      expect(':');

      JsonValue value = parseValue();

      object[key] = value;

      skipWhitespace();

      if (peek() == '}') {
        get();
        break;
      }

      expect(',');
    }

    return JsonValue(object);
  }
};

// ============================================================
// JSON Writer
// ============================================================

class JsonWriter {
public:
  static void write(const JsonValue &json, ostream &output, int indent = 0) {
    writeValue(json, output, indent);
  }

private:
  static void writeIndent(ostream &output, int indent) {
    for (int i = 0; i < indent; ++i)
      output << "    ";
  }

  static string escapeString(const string &str) {
    string result;

    for (char c : str) {
      switch (c) {
      case '"':
        result += "\\\"";
        break;

      case '\\':
        result += "\\\\";
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
        result += c;
        break;
      }
    }

    return result;
  }

  static void writeValue(const JsonValue &json, ostream &output, int indent) {
    visit([&](const auto &value) { writeType(value, output, indent); },
          json.value);
  }

  static void writeType(nullptr_t, ostream &output, int) { output << "null"; }

  static void writeType(bool value, ostream &output, int) {
    output << (value ? "true" : "false");
  }

  static void writeType(double value, ostream &output, int) { output << value; }

  static void writeType(const string &value, ostream &output, int) {
    output << '"' << escapeString(value) << '"';
  }

  static void writeType(const JsonValue::Array &array, ostream &output,
                        int indent) {
    output << "[\n";

    for (size_t i = 0; i < array.size(); ++i) {
      writeIndent(output, indent + 1);

      writeValue(array[i], output, indent + 1);

      if (i + 1 < array.size())
        output << ",";

      output << "\n";
    }

    writeIndent(output, indent);
    output << "]";
  }

  static void writeType(const JsonValue::Object &object, ostream &output,
                        int indent) {
    output << "{\n";

    size_t count = 0;

    for (const auto &[key, value] : object) {
      writeIndent(output, indent + 1);

      output << '"' << escapeString(key) << "\": ";

      writeValue(value, output, indent + 1);

      if (++count < object.size())
        output << ",";

      output << "\n";
    }

    writeIndent(output, indent);
    output << "}";
  }
};

// ============================================================
// Read JSON file
// ============================================================

JsonValue readJsonFile(const string &filename) {
  ifstream file(filename);

  if (!file.is_open()) {
    throw runtime_error("Cannot open file: " + filename);
  }

  stringstream buffer;
  buffer << file.rdbuf();

  return JsonParser(buffer.str()).parse();
}

// ============================================================
// Write JSON file
// ============================================================

void writeJsonFile(const string &filename, const JsonValue &json) {
  ofstream file(filename);

  if (!file.is_open()) {
    throw runtime_error("Cannot create file: " + filename);
  }

  JsonWriter::write(json, file, 0);

  file << '\n';
}

// ============================================================
// Main
// ============================================================

int main() {
  try {
    // Read
    JsonValue data = readJsonFile("input.json");

    cout << "JSON file read successfully.\n";

    // ----------------------------------------------------
    // Example: access the root array
    // ----------------------------------------------------

    auto &array = get<JsonValue::Array>(data.value);

    cout << "Number of records: " << array.size() << '\n';

    // ----------------------------------------------------
    // Example: modify data
    // ----------------------------------------------------

    if (!array.empty()) {
      auto &object = get<JsonValue::Object>(array[0].value);

      object["country"] = JsonValue("Vietnam");

      object["score"] = JsonValue(95.5);

      object["verified"] = JsonValue(true);
    }

    // ----------------------------------------------------
    // Write
    // ----------------------------------------------------

    writeJsonFile("output.json", data);

    cout << "JSON file written successfully.\n";
  } catch (const exception &e) {
    cerr << "Error: " << e.what() << '\n';

    return 1;
  }

  return 0;
}
