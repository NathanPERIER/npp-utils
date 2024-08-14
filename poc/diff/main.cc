
#include <fstream>

#include <npp/program/arguments.hh>
#include <npp/string/diff.hh>

#include <fmt/format.h>


void help(std::string_view executable) {
    fmt::print("usage: {} <original> <modified>\n", executable);
}


std::string read_file(const std::string& filepath) {
    std::ifstream in(filepath, std::ios::in | std::ios::binary);
    if (in.fail()) {
        throw std::runtime_error(fmt::format("Couldn't read file {} (errno={})", filepath, errno));
    }
    std::string res;
    in.seekg(0, std::ios::end);
    res.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(res.data(), res.size());
    res.resize(in.gcount());
    in.close();
    return res;
}


int main(int argc, char** argv) {
    npp::program_args args(argc, argv);

    if((args.size() > 0) && (args.drop("-h") || args.drop("--help"))) {
        help(*argv);
        return 0;
    }

    if(args.size() < 2) {
        help(*argv);
        return 1;
    }

    std::string original = read_file(args.pop());
    std::string modified = read_file(args.pop());

    return (npp::diff(original, modified) ? 1 : 0);
}