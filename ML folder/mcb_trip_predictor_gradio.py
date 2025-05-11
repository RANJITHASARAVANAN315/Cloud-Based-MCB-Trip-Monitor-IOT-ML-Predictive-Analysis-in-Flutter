import pandas as pd
import numpy as np
import pickle
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score, classification_report
import gradio as gr

# Load the new dataset
data = pd.read_csv('dataset_reduced_accuracy.csv')  # Ensure the file is in the working directory

# Convert timestamp to datetime format and extract features
data['Timestamp'] = pd.to_datetime(data['Timestamp'])
data['Hour'] = data['Timestamp'].dt.hour
data['Day'] = data['Timestamp'].dt.day
data['Month'] = data['Timestamp'].dt.month

# Define feature columns (matching new dataset)
X = data[['Temperature_Celsius', 'Humidity_Percentage', 'Water_Detected', 'Hour', 'Day', 'Month']]
y = data['MCB_Trip'].map({'NO': 0, 'YES': 1})  # Convert categorical values to numerical

# Split data into training and testing sets
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Train the RandomForest model
model = RandomForestClassifier(random_state=42)
model.fit(X_train, y_train)

# Evaluate accuracy
accuracy = accuracy_score(y_test, model.predict(X_test))
print(f'Model Accuracy: {accuracy:.2f}')
print('Classification Report:\n', classification_report(y_test, model.predict(X_test)))

# Save the trained model
with open('mcb_trip_model.pkl', 'wb') as f:
    pickle.dump(model, f)

# Define prediction function
def predict_trip(temp, humidity, water, hour, day, month):
    water = 1 if water.lower() == 'yes' else 0
    input_features = np.array([[temp, humidity, water, hour, day, month]])
    prediction = model.predict(input_features)[0]
    return 'MCB Trip' if prediction == 1 else 'No Trip'

# Create Gradio Interface
iface = gr.Interface(
    fn=predict_trip,
    inputs=[
        gr.Number(label='Temperature (°C)'),
        gr.Number(label='Humidity (%)'),
        gr.Textbox(label='Water Detected (Yes/No)'),
        gr.Number(label='Hour'),
        gr.Number(label='Day'),
        gr.Number(label='Month'),
    ],
    outputs='text',
    title='MCB Trip Prediction',
    description='Enter environmental conditions to predict if MCB will trip.'
)

iface.launch()
