#include "pdf_to_braille.h"

// translate into a list of instructions such as (stamp 1, 2 etc) (move) (stop)

int main(int argc, char *argv[])
{
        vector<char> characters;
        if (argc > 4 || argc < 3) {
                cerr << "Usage: ./pdf_to_braille [-pdf [pdf] || -textfile [textfile] || -] [optional: serial_port\n]" << endl;
                exit(1);
        } 

        string serialPort = "/dev/tty.usbmodem1101"; // Default port
        if (argc == 4) {  // If user provided a port argument
                serialPort = argv[3];
        }

        map<char, string> BRAILLE_MAPPINGS;
        create_map(BRAILLE_MAPPINGS);

        if (strcmp(argv[1],"-pdf") == 0) {
                characters = extract_text_from_pdf(argv[2]);
        } else if (strcmp(argv[1],"-textfile") == 0){
                characters = read_file(argv[2]);
        } else if (strcmp(argv[1], "-") == 0) {
                characters = make_vector(cin);
        } else {
                cerr << "Usage: ./pdf_to_braille [-pdf [pdf] || -textfile [textfile] || -]\n";
                exit(1);
        }
        // read in text file
        
        vector<char> machine_code = convert(characters, BRAILLE_MAPPINGS);
        
        try {
                // Create an io_service object
                io_context io;
                // Create and open the serial port (adjust the port name as needed)
                serial_port port(io, serialPort);
                // Set serial port parameters: baud rate, character size, parity, stop bits, and flow control.
                port.set_option(serial_port_base::baud_rate(9600));
                port.set_option(serial_port_base::character_size(8));
                port.set_option(serial_port_base::parity(serial_port_base::parity::none));
                port.set_option(serial_port_base::stop_bits(serial_port_base::stop_bits::one));
                port.set_option(serial_port_base::flow_control(serial_port_base::flow_control::none));
        
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));

                // Write the machine code to the serial port
                cout << "Using serial port: " << serialPort << endl;
                cout << "Sending to Arduino: " << machine_code.data() << endl;
                for (int i = 0; i < machine_code.size(); i++) {
                        boost::asio::write(port, boost::asio::buffer(&machine_code[i], 1));
                        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Small delay for processing
                }
                char newline = '\n';
                boost::asio::write(port, boost::asio::buffer(&newline, 1));  // Ensure Arduino gets a delimiter                         
                cout << "Braille instructions sent over serial port." << endl;
        
                // Close the port
                port.close();
            } catch (boost::system::system_error &error) {
                cerr << "Error: " << error.what() << endl;
                return 1;
            }
            
            return 0;
}


/*
 * name:      read_file
 * purpose:   read the file int an arr, with each line being a seperate string
 * arguments: the filename, and the address of an int as a second return
 * returns:   the populated arr and num_lines
 * effects:   populate arr and get num_lines
 */
vector<char> read_file(std::string filename) {
        std::ifstream file(filename);
        if (file.fail()) {
                std::cerr << "Error: could not open file " << filename;
                std::cerr << std::endl;
                exit(EXIT_FAILURE);
        }
        vector<char> characters = make_vector(file);
        file.close();
        return characters;
}

vector<char> make_vector(istream &file) {
        char next_char;
        vector<char> arr;   
        while (file.get(next_char)) {
                arr.push_back(next_char);
        }
        return arr; 
}

void append_vector(vector<char> &vec, string elem) {
        int size = elem.length();
        for (int i = 0; i < size; i++) {
                vec.push_back(elem[i]);
        }
}
 
vector<char> convert(vector<char> &text, map<char, string> &BRAILLE_MAPPINGS) {
        vector<char> translated_text;
        int size = text.size();
        bool last_char_is_num = false;
        bool do_caps = true;
        for (int i = 0; i < size; i++) {
                // check for change 
                if (last_char_is_num && !isdigit(text[i])) {
                        append_vector(translated_text, BRAILLE_MAPPINGS['@']);
                } else if (!last_char_is_num && isdigit(text[i])) {
                        append_vector(translated_text, BRAILLE_MAPPINGS['#']); 
                }
                
                // UPPER CASE LETTER CHECK
                if (isupper(text[i]) && do_caps) {
                        append_vector(translated_text, BRAILLE_MAPPINGS['|']);
                        if (check_upper(text, i)) {
                                append_vector(translated_text, BRAILLE_MAPPINGS['|']);
                                do_caps = false;
                        }
                } 

                append_vector(translated_text, BRAILLE_MAPPINGS[tolower(text[i])]);

                if (text[i] == ' ' || text[i] == '\n') {
                        do_caps = true;
                }

                if (isdigit(text[i])){
                        last_char_is_num = true;
                } else {
                        last_char_is_num = false;
                }
        }
        return translated_text;
}

bool check_upper(vector<char> &text, int curr_spot) {
        int size = text.size();
        while (curr_spot < size) {
                if (text[curr_spot] == '\n' || text[curr_spot] == ' ') {
                        return true;
                }
                if (islower(text[curr_spot])) {
                        return false;
                }
                curr_spot++;
        }
        return true;
}

void create_map(map<char,string> &BRAILLE_MAPPINGS) {
        /* This mapping maps a character to a string denoting all stamps
        O       O               1       4

        O       O       -->     2       5

        O       O               3       6

        */
        BRAILLE_MAPPINGS['a']= "1M";
        BRAILLE_MAPPINGS['b']= "12M"
        ;
        BRAILLE_MAPPINGS['c']= "14M";
        BRAILLE_MAPPINGS['d']= "145M";
        BRAILLE_MAPPINGS['e']= "15M";
        BRAILLE_MAPPINGS['f']= "124M";
        BRAILLE_MAPPINGS['g']= "1245M";
        BRAILLE_MAPPINGS['h']= "125M";
        BRAILLE_MAPPINGS['i']= "24M";
        BRAILLE_MAPPINGS['j']= "26M";
        BRAILLE_MAPPINGS['k']= "13M";
        BRAILLE_MAPPINGS['l']= "123M";
        BRAILLE_MAPPINGS['m']= "134M";
        BRAILLE_MAPPINGS['n']= "1346M";
        BRAILLE_MAPPINGS['o']= "135M";
        BRAILLE_MAPPINGS['p']= "1234M";
        BRAILLE_MAPPINGS['q']= "12346M";
        BRAILLE_MAPPINGS['r']= "1235M";
        BRAILLE_MAPPINGS['s']= "234M";
        BRAILLE_MAPPINGS['t']= "2345M";
        BRAILLE_MAPPINGS['u']= "1356M";
        BRAILLE_MAPPINGS['v']= "12356M";
        BRAILLE_MAPPINGS['w']= "256M";
        BRAILLE_MAPPINGS['x']= "1345M";
        BRAILLE_MAPPINGS['y']= "13456M";
        BRAILLE_MAPPINGS['z']= "1356M";

        BRAILLE_MAPPINGS['1']= "1M";
        BRAILLE_MAPPINGS['2']= "12M";
        BRAILLE_MAPPINGS['3']= "14M";
        BRAILLE_MAPPINGS['4']= "145M";
        BRAILLE_MAPPINGS['5']= "15M";
        BRAILLE_MAPPINGS['6']= "1245M";
        BRAILLE_MAPPINGS['7']= "1245M";
        BRAILLE_MAPPINGS['8']= "125M";
        BRAILLE_MAPPINGS['9']= "24M";
        BRAILLE_MAPPINGS['0']= "245M";

        BRAILLE_MAPPINGS[' ']= "M";
        BRAILLE_MAPPINGS['\n']= "N";
        BRAILLE_MAPPINGS['|']= "6M";
        BRAILLE_MAPPINGS['#'] = "1236M";
        BRAILLE_MAPPINGS['@'] = "56M";
}

vector<char> extract_text_from_pdf(std::string pdf_filename) {
        std::string cmd = "pdftotext " + pdf_filename + " -";
        FILE* pipe = popen(cmd.c_str(), "r");
        if (!pipe) {
            std::cerr << "Error running pdftotext" << std::endl;
            exit(EXIT_FAILURE);
        }
    
        vector<char> characters;
        char buffer[128];
        while (fgets(buffer, sizeof(buffer), pipe)) {
            characters.insert(characters.end(), buffer, buffer + strlen(buffer));
        }
    
        pclose(pipe);
        return characters;
    }