// src/App.tsx
import React, { useEffect, useState } from "react";
import "./App.css";

const App: React.FC = () => {
  const [message, setMessage] = useState<string>("");
  const [sentMessage, setSentMessage] = useState<string>("");

  useEffect(() => {
    const ws = new WebSocket("ws://localhost:8080");

    ws.onmessage = (event) => {
      const data = event.data;
      const msgCode = data.charCodeAt(0);
      const dataLength =
        (data.charCodeAt(1) << 16) |
        (data.charCodeAt(2) << 8) |
        data.charCodeAt(3);
      const messageData = data.slice(4, 4 + dataLength);

      setMessage(messageData);
    };

    ws.onopen = () => {
      console.log("WebSocket connection established");

      // Construct the message
      const messageCode = 10; // Message code 10
      const username = "Itamar";
      const password = "Itamar";
      const data = JSON.stringify({ username, password });

      // Convert message data to binary
      const encodedData = new TextEncoder().encode(data);

      // Construct the buffer to send
      const buffer = new ArrayBuffer(4 + encodedData.byteLength);
      const view = new DataView(buffer);

      // Set the first byte as message code 10
      view.setUint8(0, messageCode);

      for (let i = 0; i < encodedData.byteLength; i++) {
        view.setUint8(4 + i, encodedData[i]);
      }

      // Convert buffer to hexadecimal string
      const hexString = Array.from(new Uint8Array(buffer))
        .map((b) => b.toString(16).padStart(2, "0"))
        .join(" ");

      // Log the message being sent
      setSentMessage(hexString);

      // Send the message
      ws.send(buffer);
    };

    ws.onerror = (error) => {
      console.error("WebSocket error:", error);
    };

    ws.onclose = () => {
      console.log("WebSocket connection closed");
    };

    return () => {
      ws.close();
    };
  }, []);

  return (
    <div className="App">
      <header className="App-header">
        <h1>Sent Message (Hex): {sentMessage}</h1>
        <h2>Received Message: {message}</h2>
      </header>
    </div>
  );
};

export default App;
