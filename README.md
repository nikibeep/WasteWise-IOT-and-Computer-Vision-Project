# Intelligent Waste Segregation System

## Overview

The Intelligent Waste Segregation System is a smart solution for automating waste segregation using computer vision and an ESP32 microcontroller. The system utilizes an IP camera, YOLOv8 object detection model, and ESP32 to classify waste and efficiently direct it to the appropriate bins.

## Workflow

1. **Garbage Entry Detection:**
   - LED lights and an IP camera detect the entering of garbage.
   - The camera captures an image of the garbage on a sliding door.

2. **Image Preparation:**
   - Images are manually taken and labeled using Roboflow.
   - The dataset is split, and augmentations are applied to prepare it for training.

3. **Training the Model:**
   - YOLOv8, implemented in PyTorch, is used for object detection.
   - Model training involves adjusting parameters such as image resolution, batch size, and epochs.

4. **Deployment:**
   - Python is used for deployment, leveraging ESP32.GPIO, PyTorch, OpenCV-Python, and YAML.
   - The ESP32 controls GPIO pins and communicates with the YOLOv8 model.

5. **Waste Disposal:**
   - Based on the detected waste type, the trap door opens, dropping the garbage into the appropriate sorting bin.

6. **Bin Status Monitoring:**
   - Ultrasonic sensors monitor the fill level of each waste bin.
   - LED indicators and notifications alert when a bin is full.

7. **Dashboard:**
   - A dynamic dashboard provides real-time information on bin status.
   - Web or mobile applications allow waste management authorities to make informed decisions.

## Dependencies

- Python 3
- OpenCV
- PyTorch
- ESP32.GPIO
- YAML

## Setup

1. Clone the repository:

    ```bash
    git clone https://github.com/your-username/your-repo.git
    cd your-repo
    ```

2. Install dependencies:

    ```bash
    pip install -r requirements.txt
    ```

3. Configure ESP32 IP address and other settings in the code.

## Usage

Run the main script:

```bash
python main.py
