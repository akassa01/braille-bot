import React, { useState } from "react";

export default function BrailleConverter() {
    const [text, setText] = useState("");
    const [file, setFile] = useState(null);
    const [port, setPort] = useState(""); // New state for serial port
    const [message, setMessage] = useState("");

    const handleSubmit = async (e) => {
        e.preventDefault();
        setMessage("Processing...");

        const formData = new FormData();
        if (file) {
            formData.append("pdf", file);
        } else {
            formData.append("text", text);
        }
        formData.append("port", port); // Add port to the request

        try {
            const response = await fetch("http://localhost:5001/convert", {
                method: "POST",
                body: formData,
            });
            const result = await response.json();
            setMessage(result.message);
        } catch (error) {
            setMessage("Error: Could not connect to the server.");
        }
    };

    return (
        <div className="container">
            <h1>Text to Braille</h1>
            <form onSubmit={handleSubmit}>
                <textarea
                    placeholder="Enter text here"
                    value={text}
                    onChange={(e) => setText(e.target.value)}
                    disabled={file !== null}
                />
                <input
                    type="file"
                    accept="application/pdf"
                    onChange={(e) => setFile(e.target.files[0])}
                    disabled={text !== ""}
                />
                <input
                    type="text"
                    placeholder="Enter Arduino port (e.g., /dev/ttyUSB0)"
                    value={port}
                    onChange={(e) => setPort(e.target.value)}
                />
                <button type="submit">Convert to Braille</button>
            </form>
            {message && <p>{message}</p>}
        </div>
    );
}
