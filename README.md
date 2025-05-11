# Cloud-Based-MCB-Trip-Monitor-IOT-ML-Predictive-Analysis-in-Flutter
## Project Overview
This project integrates Internet of Things (IoT), Machine Learning (ML), and Flutter to monitor and predict the status of a Miniature Circuit Breaker (MCB). Using IoT sensors (temperature, humidity, water detection), data is collected and sent to a cloud service like Adafruit IO. The collected data is then analyzed using a machine learning model, which predicts whether the MCB will trip based on the environmental factors. The results are displayed in a Flutter mobile app, giving real-time insights to the user.
## Features
Real-Time Data Collection:  Collect sensor data (temperature, humidity, water detection) from IoT devices.

Predictive Analysis:  Use a machine learning model to predict the likelihood of MCB tripping.

Mobile App Interface: Display the MCB trip prediction in a user-friendly Flutter app.

Cloud Integration: Integrates with Adafruit IO for data storage and retrieval.

Auto-Refresh: Periodically fetches updated data from the cloud.

## Technologies Used
IoT: Arduino for sensor data collection and transmission.

Machine Learning: Model trained in Google Colab.

Flutter: Mobile app for displaying predictions and monitoring MCB status.

Adafruit IO: Cloud service for storing and retrieving sensor data.

## Getting Started
Prerequisites
Before running the project, ensure you have the following:

IoT Device: Arduino with sensors (temperature, humidity, water detection).

Flutter: Install Flutter on your machine. [Flutter Installation Guide](https://medium.com/@blup-tool/step-by-step-guide-to-installing-flutter-and-dart-on-windows-b30a631e7583)

Adafruit IO: Create an account and get an AIO username and key at Adafruit IO.

Google Colab: You will use Google Colab to train the ML model.

Python & Libraries: If running the ML training locally, install required libraries.

## Setup
Set up IoT Device:

Connect the sensors to your IoT device (Arduino).
Upload the IoT code to your device (refer to the IoT code section).
Ensure your IoT device is connected to the internet and sending data to Adafruit IO.

Train the ML Model on Google Colab:

Open Google Colab and upload the dataset for training the model.
Use Google Colab to train the model using pandas, numpy, and tensorflow or another ML library.
Save the trained model using .h5 format (or a suitable model format).

Flutter App Setup:

Clone the repository or download the project.
Update api_service.dart with your Adafruit IO username and AIO key.

### Run the Flutter app using:
      flutter run
The app will fetch data from Adafruit IO and display the MCB trip prediction.

## Usage
IoT Device:
The IoT device continuously collects data from sensors and sends it to Adafruit IO.

Machine Learning:
The ML model receives environmental data (temperature, humidity, water) and predicts whether the MCB will trip.
The prediction is either MCB Trip or No Trip.

Flutter App:
The app fetches the latest data from Adafruit IO and displays the prediction on the screen.
Users can manually refresh the data by pressing the refresh button in the app.

## License
This project is licensed under the [Apache 2.0](https://github.com/RANJITHASARAVANAN315/Cloud-Based-MCB-Trip-Monitor-IOT-ML-Predictive-Analysis-in-Flutter/blob/main/LICENSE) License.

## Acknowledgements
Flutter: For building the mobile app.

Adafruit IO: For providing an easy-to-use cloud platform for IoT.

Google Colab: For training machine learning models in the cloud.

## Conclusion
This project combines IoT, ML, and Flutter to create an intelligent monitoring system for MCBs, using real-time data for predictive maintenance. The model is trained in Google Colab, and the results are displayed in a Flutter app, providing a comprehensive solution for monitoring MCB trips.
