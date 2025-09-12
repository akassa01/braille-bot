import React from "react";

function Contact() {
    return (
        <div className="container">
            <h1>Contact Us</h1>
            <p>If you have any questions, feel free to reach out!</p>
            <form>
                <label>Name:</label>
                <input type="text" name="name" required />
                
                <label>Email:</label>
                <input type="email" name="email" required />
                
                <label>Message:</label>
                <textarea name="message" required></textarea>

                <button type="submit">Send</button>
            </form>
        </div>
    );
}

export default Contact;
