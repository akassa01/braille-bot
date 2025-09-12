const express = require("express");
const multer = require("multer");
const cors = require("cors");
const fs = require("fs");
const { exec } = require("child_process");

const app = express();
const upload = multer({ dest: "uploads/" });

app.use(cors());
app.use(express.json());

// Route for text or PDF input
app.post("/convert", upload.single("pdf"), (req, res) => {
        let command = "../cpp/braille";
        let serialPort = req.body.port || "/dev/tty.usbmodem1101"; // Default port
    
        if (req.file) {
            command += ` -pdf ${req.file.path} ${serialPort}`;
        } else if (req.body.text) {
            const tempFilePath = "../cpp/temp.txt";
            fs.writeFileSync(tempFilePath, req.body.text);
            command += ` -textfile ${tempFilePath} ${serialPort}`;
        } else {
            return res.status(400).json({ message: "No input provided." });
        }
    
        console.log(`Running command: ${command}`);
        exec(command, (error, stdout, stderr) => {
            if (error) {
                console.error(`Error running pdf_to_braille: ${stderr}`);
                return res.status(500).json({ message: `Error processing Braille conversion: ${stderr}` });
            }
            res.json({ message: "Braille conversion sent to printer.", output: stdout });
        });
});
    

app.listen(5001, () => console.log("Backend server running on port 5001"));
