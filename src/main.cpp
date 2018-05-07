#include <iostream>
#include <fstream>
#include <string>
#include "json.hpp"

using namespace std;
class Acf {
  string data;

public:
  Acf(string data): data(data) {
    cropData();
    toJsonRaw();
    toJson();
  }

  void cropData() {
    std::size_t found = -1;
    string delims = "{}";
    int opened = 0;
    int indexOpen = -1, indexClose = -1;
    while((found = data.find_first_of(delims, found + 1)) != std::string::npos && indexClose == -1)
    {
      char c = data.at(found);

      if (c == '{') {
        opened++;

        if (indexOpen == -1)
          indexOpen = found;
      } else if (c == '}') {
        opened--;

        if (opened == 0) {
          indexClose = found + 1;
        }
      }
    }

    // Get key for table
    found = data.find_last_of("\"", indexOpen);
    found = data.find_last_of("\"", found - 1);
    indexOpen = found;

    data = data.substr(indexOpen, indexClose - indexOpen);
  }

  void toJsonRaw() {
    int indexQuote = 0;
    int index[4];
    int insertCommaNext = -1;
    string delims = "\"{}";
    std::size_t found = data.find_first_of(delims);
    while(found != std::string::npos)
    {
      int inc = 1;
      char c = data.at(found);

      if (c != '"') {
        if (c == '}')
          insertCommaNext = found + 1;
        else if (c == '{') {
          data.insert(index[1] + 1, ":");
          inc++;
        }

        indexQuote = 0;
      } else {
        if (insertCommaNext != -1) {
          data.insert(insertCommaNext, ",");
          inc++;
          insertCommaNext = -1;
        }

        if (indexQuote == 2) { // Join 'key: value'
          data.replace(index[1] + 1, 1, ":");
        } else if (indexQuote == 4) {
          indexQuote = 0;
          data.insert(index[3] + 1, ",");
          inc++;
        }

        index[indexQuote] = found;

        indexQuote++;
      }

      found = data.find_first_of(delims, found + inc);
    }

    data = "{" + data + "}";
  }

  void toJson() {
    picojson::value json;
    std::string err = picojson::parse(json, data);
    if (!err.empty())
      std::cerr << err << std::endl;

    data = json.serialize();
  }

  void print() {
    cout << data;
  }

  string get() {
    return data;
  }
};

int main(int argc, char* argv[])
{
  if (argc >= 2) { // Filename
    std::string filename(argv[1]);
    std::ifstream file;
    std::ios_base::iostate exceptionMask = file.exceptions() | std::ios::failbit;
    file.exceptions(exceptionMask);

    try {
      file.open(filename);
      std::string data((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));

      Acf acf(data);
      acf.print();
    }
    catch (std::ios_base::failure& e) {
      std::cerr << e.what() << '\n';
    }
  } else { // stdin
    std::string data = "";
    for (std::string line; std::getline(std::cin, line);)
      data += line;

    Acf acf(data);
    acf.print();
  }

  return 0;
}
