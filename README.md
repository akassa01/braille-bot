# Text to Braille

**Text to Braille** is a full-stack project that converts digital text (from plain text files, PDFs, or user input) into Braille instructions, which can be transmitted to an Arduino-based Braille embossing device.

The system has three main parts:

1. **Web frontend (React)** – lets users input or upload text/PDFs.
2. **Backend server (Node/Express)** – handles API requests and routes text for conversion.
3. **C++ and Arduino components** – convert text/PDFs into Braille machine code and send instructions to a hardware Braille printer/embosser.

---

## Features

* Upload a **PDF** or **text file** and convert it into Braille.
* Real-time text-to-Braille conversion through the web interface.
* Serial communication with an **Arduino** to emboss Braille on paper.
* Handles formatting: capitalization, numbers, and spacing.
* Cross-platform design (Node.js for web, C++ for conversion, Arduino for hardware).

---

## Project Structure

```
/frontend
  ├── Home.js             # React component for UI
  ├── BrailleConverter.js # React component for conversion logic
/backend
  ├── server.js           # Express backend server
  ├── app.js              # Route setup
/cpp
  ├── pdf_to_braille.cpp  # C++ program for text/PDF to Braille conversion
/hardware
  ├── jumbohack.ino       # Arduino sketch for controlling Braille printer
```

---

## Installation & Setup

### 1. Clone the repository

```bash
git clone https://github.com/yourusername/text-to-braille.git
cd text-to-braille
```

### 2. Install dependencies (Frontend & Backend)

```bash
cd frontend
npm install
cd ../backend
npm install
```

### 3. Run the web app

```bash
# In backend
node server.js

# In frontend (separate terminal)
npm start
```

The app should now be running at `http://localhost:3000`.

---

## Using the C++ Converter

The C++ component (`pdf_to_braille.cpp`) handles file-to-Braille translation and communicates with the Arduino via serial.

### Compile:

```bash
g++ pdf_to_braille.cpp -o pdf_to_braille -lboost_system -lpthread
```

### Run:

```bash
# Convert a PDF
./pdf_to_braille -pdf example.pdf

# Convert a text file
./pdf_to_braille -textfile input.txt

# Pipe from stdin
echo "Hello World" | ./pdf_to_braille -
```

Optional: specify a serial port (default: `/dev/tty.usbmodem1101`)

```bash
./pdf_to_braille -pdf example.pdf /dev/ttyUSB0
```

---

## Arduino Setup

* Upload `jumbohack.ino` to your Arduino board.
* Connect the Arduino to your computer via USB.
* Ensure the serial port matches what’s specified in `pdf_to_braille.cpp`.

---

## Dependencies

* **Frontend**: React, JavaScript
* **Backend**: Node.js, Express
* **C++ Converter**: Boost.Asio (for serial communication), `pdftotext` (for PDF parsing)
* **Hardware**: Arduino IDE

---

## Future Improvements

* Support for contracted Braille (Grade 2).
* Cross-platform serial port configuration.
* Web-based visualization of Braille output.
* Mobile app integration.

---

## License

MIT License – feel free to use, modify, and distribute.

---
