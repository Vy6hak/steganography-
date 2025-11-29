# Steganography Using C (LSB Technique)

This project implements **Steganography using the Least Significant Bit (LSB)** technique to hide secret text inside BMP image files and extract it back securely without visible distortion. It supports both **encoding** and **decoding** operations using command-line arguments.

---

## 🚀 Features
- Hide secret text inside a `.bmp` image using **LSB bit manipulation**
- Recover hidden text with decode mode
- **Magic string validation**, **file extension encoding**, **file size encoding**
- **Non-destructive embedding** — original image visually preserved
- User-friendly terminal output with status logs
- Input validation and error handling

---

## 🧠 How It Works
| Stage | Description |
|--------|-------------|
| **Encode** | Reads secret file & embeds bits into BMP pixel data |
| **Decode** | Extracts LSB bits and reconstructs secret file |
| **Validation** | Magic string verifies correct stego image |
| **Capacity Check** | Ensures image can store full hidden content |

---

## 📂 Project Structure
Steganography-LSB-in-C/
│── encode.c
│── decode.c
│── test_encode.c # main driver file
│── secret.txt # example hidden file
│── stego.bmp # output image after encoding
└── README.md


---

## 🛠 Tech Stack
- **C Programming**
- **File Handling**
- **Bitwise Operations**
- **BMP Image Manipulation**
- **Command Line Interface**

---

## 📌 Usage

### **Compile**
```bash
gcc *.c -o stego
Encode

./stego -e input.bmp secret.txt stego.bmp
Decode
bash
Copy code
./stego -d stego.bmp output.txt
📝 Example Output

You have choosed encoding
Read and Validate is Successfull
INFO: Files opened successfully
INFO: Header copied successfully
INFO: Magic string encoded
INFO: Encoded secret file extension size
INFO: Encoded secret file extension
INFO: Encoded secret file size
INFO: Encoded secret file data
------------------------------
|::  Encoding successfull  ::|
------------------------------
📦 Supported File Types
File Type	Description
.bmp	Container image used to hide data
.txt	Secret file to embed

📍 Limitations
Works only with 24-bit BMP images

Secret file size limited by image capacity

✨ Author
Vaishak S

If you found this helpful, please ⭐ star the repository!
