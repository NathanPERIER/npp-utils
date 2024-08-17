
#include <fstream>

#include <npp/program/arguments.hh>
#include <npp/system/subprocess.hh>

#include <fmt/format.h>


void help(std::string_view executable) {
    fmt::print("usage: {} <program> [args...]\n", executable);
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
        help(args.executable());
        return 0;
    }

    if(args.size() < 1) {
        help(args.executable());
        return 1;
    }

    std::string prog_name = args.pop();

    npp::subprocess proc = npp::subprocess::spawn(prog_name, args);
    proc.join();
    std::optional<int> retcode = proc.retcode();
    if(retcode.has_value()) {
        fmt::print("Process {} ({}) exited with return code {}\n", proc.pid(), proc.executable().string(), retcode.value());
        return retcode.value();
    }
    retcode = proc.termsig();
    if(retcode.has_value()) {
        fmt::print("Process {} ({}) exited with signal {}\n", proc.pid(), proc.executable().string(), retcode.value());
        return 1;
    }
    fmt::print("Process {} ({}) exited abnormally\n", proc.pid(), proc.executable().string());
    return 1;
}