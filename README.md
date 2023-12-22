![](https://github.com/nikibeep/WasteWise-IOT-and-Computer-Vision-Project/raw/main/Bin23.gif)
# WasteWise : IOT and Computer Vision based automatic waste segregation Project
## Table of Contents
- [Overview](#overview)
- [Workflow](#workflow)
- [Architecture Diagram](#architecture-diagram)
- [Image Preparation](#image-preparation)
- [Dependencies](#dependencies)
- [Setup](#setup)
- [Usage](#usage)

## Overview

The Intelligent Waste Segregation System is a smart solution for automating waste segregation using computer vision and an ESP32 microcontroller. The system utilizes an IP camera, YOLOv8 object detection model, and ESP32 to classify waste and efficiently direct it to the appropriate bins for Plastic, Metal, Biodegradable. Monitoring of the bins are conducted using a dashboard for live data collections from the ultrasonic sensors.

## Workflow

1. **Garbage Entry Detection:**
   - LED lights and an IP camera detect the entering of garbage.
   - The camera captures an image of the garbage on a trap door.

2. **Image Preparation:**
   - Images are manually taken and labeled using Roboflow.
   - The dataset is split, and augmentations are applied to prepare it for training.

3. **Training the Model:**
   - custom YOLOv8, implemented in PyTorch, is used for object detection.
   - Model training involves adjusting parameters such as image resolution of 640*480, batch size of 32, and epochs of 50.

4. **Deployment:**
   - Python is used for deployment, leveraging ESP32.GPIO, PyTorch, OpenCV-Python.
   - The ESP32 controls GPIO pins and communicates with the YOLOv8 model running on the edge computer.

5. **Waste Disposal:**
   - Based on the detected waste type, the trap door opens, dropping the garbage into the appropriate sorting bin by falling on the sliding door mounted on the 360 stepper motor.

6. **Bin Status Monitoring:**
   - Ultrasonic sensors monitor the fill level of each waste bin.
   - LED indicators and notifications alert when a bin is full.

7. **Dashboard:**
   - A dynamic dashboard provides real-time information on bin status.
   - Web or mobile applications allow waste management authorities to make informed decisions.

## Architecture Diagram 

<a href="https://ibb.co/p4nw6Bw"><img src="https://i.ibb.co/WBs5N45/wastewise-arch.png" alt="wastewise-arch" border="0"></a>

## Image Preparation



## Dependencies

- Python 3
- OpenCV
- PyTorch
- ESP32.GPIO

## Setup

1. Clone the repository:

    ```bash
    git clone https://github.com/nikibeep/WasteWise-IOT-and-Computer-Vision-Project.git
    cd WasteWise-IOT-and-Computer-Vision-Project
    ```

2.  Create a conda environment and Install dependencies:

    ```bash
    pip install -r requirements.txt
    ```

3. Configure ESP32 IP address and other settings in the code.

## Usage

Run the .ipynb
