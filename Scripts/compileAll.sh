SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

g++ "$SCRIPT_DIR"/../src/FNRTest.cpp -o "$SCRIPT_DIR"/../fnrtest.out
g++ -std=c++11 "$SCRIPT_DIR"/../src/XTest.cpp -o "$SCRIPT_DIR"/../xtest.out
