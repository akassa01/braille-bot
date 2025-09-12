#ifndef PDF_TO_BRAILLE_H
#define PDF_TO_BRAILLE_H

#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <map>

// Boost headers
#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>

// Namespace usage
using namespace std;
using namespace boost::asio;

// Function prototypes
std::vector<char> read_file(std::string filename);
int main(int argc, char *argv[]);
vector<char> make_vector(istream &file);
void append_vector(vector<char> &vec, string elem);
vector<char> convert(vector<char> &text, map<char, string> &BRAILLE_MAPPINGS);
void create_map(map<char,string> &BRAILLE_MAPPINGS);
bool check_upper(vector<char> &text, int curr_spot);
vector<char> extract_text_from_pdf(std::string pdf_filename);

#endif 
