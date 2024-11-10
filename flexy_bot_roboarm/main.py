import cv2
import tkinter as tk
from tkinter import messagebox
import numpy as np
import pygame  # Import pygame for sound
import serial  # Import serial library for Arduino communication

# Initialize pygame mixer
pygame.mixer.init()

# Load the siren sound
siren_sound = pygame.mixer.Sound("siren.wav")  # Ensure you have this file in the same directory

# Load YOLO
net = cv2.dnn.readNet("yolov3.weights", "yolov3.cfg")
classes = []
with open("coco.names", "r") as f:
    classes = [line.strip() for line in f.readlines()]  
layer_names = net.getLayerNames()
output_layers = [layer_names[i - 1] for i in net.getUnconnectedOutLayers()]

# Initialize serial communication with Arduino
try:
    ser = serial.Serial('COM12', 9600, timeout=1)  # Replace 'COM12' with your Arduino's serial port
except serial.SerialException as e:
    print(f"Error opening serial port: {e}")
    exit(1)

def detect_animals():
    # Initialize the webcam
    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        print("Error: Camera not accessible.")
        return

    flame_detected = False  # Flag to check if flame is detected
    siren_playing = False  # Flag to check if siren is already playing

    while True:
        ret, img = cap.read()
        if not ret:
            print("Error: Unable to read from camera")
            break

        height, width, channels = img.shape

        # Detecting objects
        blob = cv2.dnn.blobFromImage(img, 0.00392, (416, 416), (0, 0, 0), True, crop=False)
        net.setInput(blob)
        outs = net.forward(output_layers)

        # Showing informations on the screen
        class_ids = []
        confidences = []
        boxes = []
        for out in outs:
            for detection in out:
                scores = detection[5:]
                class_id = np.argmax(scores)
                confidence = scores[class_id]
                if confidence > 0.5:
                    # Object detected
                    center_x = int(detection[0] * width)
                    center_y = int(detection[1] * height)
                    w = int(detection[2] * width)
                    h = int(detection[3] * height)

                    # Rectangle coordinates
                    x = int(center_x - w / 2)
                    y = int(center_y - h / 2)

                    boxes.append([x, y, w, h])
                    confidences.append(float(confidence))
                    class_ids.append(class_id)

        indexes = cv2.dnn.NMSBoxes(boxes, confidences, 0.5, 0.4)
        animal_detected = False
        for i in range(len(boxes)):
            if i in indexes:
                x, y, w, h = boxes[i]
                label = str(classes[class_ids[i]])
                if label in ["dog", "cat", "bird", "horse", "cow", "sheep", "elephant", "bear", "zebra", "giraffe"]:
                    animal_detected = True
                cv2.rectangle(img, (x, y), (x + w, y + h), (0, 255, 0), 2)
                cv2.putText(img, label, (x, y + 30), cv2.FONT_HERSHEY_PLAIN, 3, (0, 0, 255), 3)

        # Check for flame detection from Arduino
        if ser.in_waiting > 0:
            flame_data = ser.readline().decode('utf-8').strip()
            print(f"Flame data: {flame_data}")  # Debugging statement
            if "Flame detected!" in flame_data:  # Ensure the message matches
                flame_detected = True
                if not siren_playing:
                    siren_sound.play()  # Play the siren sound
                    siren_playing = True
                print("Flame detected!")

        if animal_detected:
            messagebox.showinfo("Animal Detection", "Animal detected!")
            siren_sound.play()  # Play the siren sound
            print("Animal Detection")
            ser.write(b'Animal Confirmed\n')  # Send confirmation to Arduino
            # break

        # If flame is detected, keep checking for human detection
        if flame_detected :
            if animal_detected:
                messagebox.showinfo("Warning", "Please move⚠️⚠️⚠️!")
                # break  # Exit the loop if human is detected
            else:
                cv2.putText(img, "Flame detected! ", (10, 30), cv2.FONT_HERSHEY_PLAIN, 2, (0, 0, 255), 2)

        cv2.imshow('Image', img)
        if cv2.waitKey(1) == ord('q'):
            break

        root.update()  # Keep the GUI responsive

    cap.release()
    cv2.destroyAllWindows()
    if siren_playing:
        siren_sound.stop()  # Stop the siren sound if it was playing
    ser.close()  # Close the serial port

root = tk.Tk()
root.title("Flexy Bot Factory Animal Detection System")
root.geometry("800x600")  # Set the window size to 800x600

title_label = tk.Label(root, text="Flexy Bot Factory Animal Detection System", font=("Arial", 24))
title_label.pack(pady=20)

# Start animal detection immediately
detect_animals()

root.protocol("WM_DELETE_WINDOW", root.quit)  # Ensure the program exits gracefully
root.mainloop()