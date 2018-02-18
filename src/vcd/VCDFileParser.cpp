/*!
@file
@brief Definition of the VCDFileParser class
*/

#include "VCDFileParser.hpp"

VCDFileParser::VCDFileParser() {

    this -> trace_scanning = false;
    this -> trace_parsing  = false;

}

VCDFileParser::~VCDFileParser() {

}


VCDFile * VCDFileParser::parse_file(const std::string &filepath) {

    this -> filepath = filepath;
    
    scan_begin();

    this -> fh = new VCDFile();
    VCDFile * tr = this -> fh;

    this -> fh -> root_scope = new VCDScope;
    this -> fh -> root_scope -> name = std::string("$root");
    this -> fh -> root_scope -> type = VCD_SCOPE_ROOT;
    
    this -> scopes.push(this -> fh -> root_scope);
    
    tr -> add_scope(scopes.top());

    VCDParser::parser parser(*this);

    parser.set_debug_level(trace_parsing);

    int result = parser.parse();
    
    scopes.pop();

    scan_end();

    if (result == 0 ) {
        this -> fh = nullptr;
        return tr;
    } else {
        tr = nullptr;
        delete this -> fh;
        return nullptr;
    }

}
        

void VCDFileParser::error(const VCDParser::location & l, const std::string & m){
    std::cerr << "line "<< l.begin.line 
              << std::endl;
    std::cerr << " : "<<m<<std::endl;
}

void VCDFileParser::error(const std::string & m){
    std::cerr << " : "<<m<<std::endl;
}



#ifdef VCD_PARSER_STANDALONE

/*!
@brief Standalone test function to allow testing of the VCD file parser.
*/
int main (int argc, char** argv){

    std::string infile (argv[1]);

    std::cout << "Parsing " << infile << std::endl;

    VCDFileParser parser;

    VCDFile * result = parser.parse_file(infile);

    if(result) {
        std::cout << "Parse successful." << std::endl;
        std::cout << "Version:       " << result -> version << std::endl;
        std::cout << "Date:          " << result -> date << std::endl;
        std::cout << "Signal count:  " << result -> signals.size() <<std::endl;
        std::cout << "Times Recorded:" << result -> times.size() << std::endl;

        delete result;
        
        return 0;
    } else {
        std::cout << "Parse Failed." << std::endl;
        return 1;
    }
}


#endif
