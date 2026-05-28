const express = require("express");
const mongoose = require("mongoose");
const cors = require("cors");

const app = express();

app.use(cors());
app.use(express.json());

// MongoDB Atlas Connection
const mongoURI = "mongodb+srv://jyotibrahmanandy:jyotibrahmanandy@jyoti.29sztwi.mongodb.net/noiseDB?retryWrites=true&w=majority";

mongoose.connect(mongoURI)
  .then(() => console.log("✅ MongoDB Atlas Connected"))
  .catch(err => console.error("❌ MongoDB Connection Error:", err));

// Schema
const noiseSchema = new mongoose.Schema({
  sound: Number,
  status: String,
  time: String,
  createdAt: { type: Date, default: Date.now }
});

const Noise = mongoose.model("Noise", noiseSchema);

// POST API (ESP32 will send data here)
app.post("/data", async (req, res) => {
  console.log("Received data from ESP32:", req.body);
  try {
    const newData = new Noise(req.body);
    await newData.save();
    res.json({ message: "Data saved to MongoDB Atlas" });
  } catch (err) {
    console.error("Error saving to MongoDB:", err);
    res.status(500).json({ error: "Failed to save data" });
  }
});

// GET API (React will fetch data)
app.get("/data", async (req, res) => {
  try {
    const data = await Noise.find().sort({ createdAt: -1 });
    res.json(data);
  } catch (err) {
    res.status(500).json({ error: "Failed to fetch data" });
  }
});

const PORT = 5001;
app.listen(PORT, () => {
  console.log(`Server running on port ${PORT}`);
  console.log("Connected to MongoDB Atlas");
});