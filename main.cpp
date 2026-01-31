#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <limits>

static std::string trim(const std::string &s) {
    size_t start = s.find_first_not_of(" \t");
    size_t end = s.find_last_not_of(" \t");
    if (start == std::string::npos) return "";
    return s.substr(start, end - start + 1);
}

int main() {
    std::ifstream in("kobra.kb");
    std::string line;
    std::vector<std::string> lines;

    while (std::getline(in, line)) {
        if (auto pos = line.find("//"); pos != std::string::npos)
            line = line.substr(0, pos);
        lines.push_back(line);
    }

    bool need_iostream = false;

    // detect println, print, input, inputln
    for (auto &l : lines)
        if (l.find("println(") != std::string::npos ||
            l.find("print(")   != std::string::npos ||
            l.find("input(")   != std::string::npos ||
            l.find("inputln(") != std::string::npos)
            need_iostream = true;

    for (auto &l : lines) {

        // println(...)
        if (size_t pos = l.find("println("); pos != std::string::npos) {
            size_t end = l.find(")", pos);
            std::string inside = l.substr(pos + 8, end - (pos + 8));
            l = l.substr(0, pos) + "std::cout << " + inside + " << '\\n'" + l.substr(end + 1);
        }

        // print(...)
        if (size_t pos = l.find("print("); pos != std::string::npos &&
            l.find("println(") == std::string::npos) {
            size_t end = l.find(")", pos);
            std::string inside = l.substr(pos + 6, end - (pos + 6));
            l = l.substr(0, pos) + "std::cout << " + inside + ";" + l.substr(end + 1);
        }

        // input(prompt, var)
        if (size_t pos = l        }

        // input(prompt, var)
        if (size_t pos = l.find("input("); pos != std::string::npos) {
            size_t start = pos + 6;
            size_t comma = l.find(",", start);
            size_t end = l.find(")", comma);

            std::string prompt = trim(l.substr(start, comma - start));
            std::string var = trim(l.substr(comma + 1, end - (comma + 1)));

            l = l.substr(0, pos)
              + "std::cout << " + prompt + "; std::cin >> " + var + ";"
              + l.substr(end + 1);
        }

        // inputln(prompt, var)
        if (size_t pos = l.find("inputln("); pos != std::string::npos) {
            size_t start = pos + 8;
            size_t comma = l.find(",", start);
            size_t end = l.find(")", comma);

            std::string prompt = trim(l.substr(start, comma - start));
            std::string var = trim(l.substr(comma + 1, end - (comma + 1)));

            l = l.substr(0, pos)
              + "std::cout << " + prompt + "; "
              + "std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\\n'); "
              + "std::getline(std::cin, " + var + ");"
              + l.substr(end + 1);
        }

        // exit(code)
        if (size_t pos = l.find("exit("); pos != std::string::npos) {
            size_t end = l.find(")", pos);
            std::string code = trim(l.substr(pos + 5, end - (pos + 5)));

            l = l.substr(0, pos) + "return " + code + ";" + l.substr(end + 1);
        }
    }

    std::ofstream out("translated.cpp");

    if (need_iostream)
        out << "#include <iostream>\n#include <limits>\n";

    out << "#include \"macro.h\"\n";

    out << "\n";

    for (auto &l : lines)
        out << l << "\n";
}