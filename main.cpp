#include <iostream>
#include <vector>
#include <string>
#include <fstream>

int main() {
    std::ifstream in("kobra.kb");
    std::string line;
    std::vector<std::string> lines;

    while (std::getline(in, line)) {
        // strip comments
        if (auto pos = line.find("//"); pos != std::string::npos)
            line = line.substr(0, pos);
        lines.push_back(line);
    }

    bool need_iostream = false;

    // detect println
    for (auto &l : lines)
        if (l.find("println") != std::string::npos)
            need_iostream = true;

    // translate println
    for (auto &l : lines) {
        size_t pos = l.find("println(");
        if (pos != std::string::npos) {
            size_t end = l.find(")", pos);
            std::string inside = l.substr(pos + 8, end - (pos + 8));
            l = l.substr(0, pos) + "std::cout << " + inside + " << '\\n'" + l.substr(end + 1);
        }
    }

    std::ofstream out("translated.cpp");

    if (need_iostream)
        out << "#include <iostream>\n";

    out << "\n";

    for (auto &l : lines)
        out << l << "\n";
}
