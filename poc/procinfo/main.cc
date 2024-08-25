
#include <fstream>

#include <unistd.h>

#include <fmt/format.h>

#include <npp/program/arguments.hh>
#include <npp/system/procinfo.hh>



void help(std::string_view executable) {
    fmt::print("usage: {} <pid>\n", executable);
}


void print_info(const pid_t pid) {
    fmt::print("PID: {}\n", pid);
    fmt::print("Executable: {}\n", npp::proc_exe(pid).string());
    fmt::print("Root: {}\n", npp::proc_root(pid).string());
    fmt::print("Working directory: {}\n", npp::proc_cwd(pid).string());
}


int main(int argc, char** argv) {
    npp::program_args args(argc, argv);

    if((args.size() > 0) && (args.drop("-h") || args.drop("--help"))) {
        help(args.executable());
        return 0;
    }

    if(args.size() != 1) {
        help(args.executable());
        return 1;
    }

    const std::string pid_repr = args.pop();
    uint64_t pid;
    if(pid_repr == "current") {
        pid = getppid();
    } else {
        std::size_t idx;
        pid = std::stoull(pid_repr, &idx);
        if(idx != pid_repr.size()) {
            fmt::print("Bad PID\n");
            return 1;
        }
    }

    print_info(pid);

    return 0;
}