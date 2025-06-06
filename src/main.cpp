#include "parser.hh"


int main(int argc, char** argv) {
    std::ifstream file("/home/discopse/Hpp/build/source.hhh", std::ios::binary);
    if (argc < 1) {
        std::cerr << "Please provide a file name." << std::endl;
        return 1;
    }
    std::string code;

    file.seekg(0, std::ios::end);

    int size = file.tellg();

    code.resize(size);

    file.seekg(0, std::ios::beg);

    file.read(&code[0],size);

    Tokenizer TTokenizer = Tokenizer(code);
    TTokenizer.Tokenize();
    std::vector<Token> tokens = TTokenizer.token();
    Parser parsers(tokens);
    auto asdf = parsers.parse();
    asdf->Print();
    parsers.PrintAST();
    std::cout << "Parsing complete!" << std::endl;
    for (const auto& token : tokens) {
        std::cout << "Token Type: " << static_cast<int>(token.type) << ", Value: " << token.value << std::endl;
    }
    return 0;
}