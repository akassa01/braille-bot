import React from "react";
import BrailleConverter from "../components/BrailleConverter"; // Import the BrailleConverter component

function Home() {
    return (
        <div className="container">
            <h1>Welcome to Text to Braille Converter</h1>
            <h3>Convert your text into Braille instantly with our easy-to-use tool.</h3>
            <p>Our website helps you seamlessly translate any text into Braille, making written content more accessible.</p>

            {/* Include the BrailleConverter component */}
            <BrailleConverter />
        </div>
    );
}

export default Home;
