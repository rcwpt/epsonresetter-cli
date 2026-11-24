#include "rcwpt/common.hpp"

namespace rcwpt::cli {
    int handle_cli(int argc, char* argv[]);
}

int main(int argc, char* argv[]) {
    return rcwpt::cli::handle_cli(argc, argv);
}
