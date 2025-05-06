from flask import Flask, request, jsonify
import pickle
import numpy as np
import os

app = Flask(__name__)

# Load the ML model from pickle file
MODEL_PATH = 'plant_model.pkl'  # Update this path to your model file

# Check if model exists
if not os.path.exists(MODEL_PATH):
    print(f"ERROR: Model file {MODEL_PATH} not found!")
else:
    try:
        # Load the model
        with open(MODEL_PATH, 'rb') as file:
            model = pickle.load(file)
        print(f"Model loaded successfully from {MODEL_PATH}")
    except Exception as e:
        print(f"Error loading model: {e}")
        model = None

# Define normalization function (should match your training preprocessing)
def normalize_features(data):
    # Adjust min/max values based on your sensor ranges and training data
    soil_moisture = (data['soil_moisture'] - 1000) / (3000 - 1000)
    light = data['light'] / 4095
    air_quality = data['air_quality'] / 4095
    temperature = (data['temperature'] - 15) / (35 - 15)
    humidity = (data['humidity'] - 20) / (90 - 20)
    
    # Ensure values are within 0-1 range
    soil_moisture = max(0, min(1, soil_moisture))
    light = max(0, min(1, light))
    air_quality = max(0, min(1, air_quality))
    temperature = max(0, min(1, temperature))
    humidity = max(0, min(1, humidity))
    
    # Return array in the same order as your model expects
    return np.array([[soil_moisture, light, air_quality, temperature, humidity]])

# Define a fallback rule-based classifier in case the model fails
def rule_based_classify(data):
    soil_moisture = (data['soil_moisture'] - 1000) / (3000 - 1000)
    light = data['light'] / 4095
    air_quality = data['air_quality'] / 4095
    temperature = (data['temperature'] - 15) / (35 - 15)
    humidity = (data['humidity'] - 20) / (90 - 20)
    
    # Rules (similar to earlier rule-based logic)
    if soil_moisture > 0.7:  # Dry soil
        return 1  # Needs water
    elif temperature > 0.8 or temperature < 0.2 or humidity < 0.3 or air_quality > 0.7:
        return 2  # Unhealthy
    elif light < 0.2:  # Too dark
        return 2  # Unhealthy
    else:
        return 0  # Happy

@app.route('/predict', methods=['POST'])
def predict():
    try:
        # Get sensor data from request
        data = request.json
        print(f"Received data: {data}")
        
        # Check if we have all required sensor readings
        required_fields = ['soil_moisture', 'light', 'air_quality', 'temperature', 'humidity']
        if not all(field in data for field in required_fields):
            missing = [field for field in required_fields if field not in data]
            return jsonify({'error': f'Missing required fields: {missing}'}), 400
        
        # Process the data and make prediction
        if model is not None:
            try:
                # Normalize features
                features = normalize_features(data)
                
                # Make prediction using model
                prediction = int(model.predict(features)[0])
                probability = model.predict_proba(features)[0]
                
                # Log results
                print(f"Prediction: {prediction}, Probabilities: {probability}")
                
                return jsonify({
                    'prediction': prediction,
                    'probabilities': probability.tolist(),
                    'message': ['Happy', 'Needs Water', 'Unhealthy'][prediction]
                })
            except Exception as e:
                print(f"Error during prediction: {e}")
                # Fall back to rule-based classification
                prediction = rule_based_classify(data)
                return jsonify({
                    'prediction': prediction,
                    'message': ['Happy', 'Needs Water', 'Unhealthy'][prediction],
                    'note': 'Used fallback rule-based classification due to model error'
                })
        else:
            # Use rule-based classification if model wasn't loaded
            prediction = rule_based_classify(data)
            return jsonify({
                'prediction': prediction,
                'message': ['Happy', 'Needs Water', 'Unhealthy'][prediction],
                'note': 'Used rule-based classification (no model available)'
            })
            
    except Exception as e:
        print(f"Server error: {e}")
        return jsonify({'error': str(e)}), 500

if __name__ == '__main__':
    # Run server on all available interfaces (0.0.0.0) for external access
    app.run(host='0.0.0.0', port=5050, debug=True)