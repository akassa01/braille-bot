import React from "react";
import { BrowserRouter as Router, Routes, Route, Link } from "react-router-dom";
import Home from "./pages/Home";
import Contact from "./pages/Contact";
import "./App.css";

function App() {
    return (
        <Router>
            <div className="navbar">
                <Link to="/">Home</Link>
                <Link to="/contact">Contact Us</Link>
            </div>
            <Routes>
                <Route path="/" element={<Home />} />
                <Route path="/contact" element={<Contact />} />
            </Routes>
        </Router>
    );
}

export default App;
