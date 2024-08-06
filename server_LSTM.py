from flask import Flask, request, jsonify
import sqlite3
from datetime import datetime
import numpy as np
import tensorflow as tf
from tensorflow.keras.models import Sequential  # type: ignore
from tensorflow.keras.layers import LSTM, Dense # type: ignore
import pandas as pd # type: ignore

app = Flask(__name__)

DATABASE = 'federated_learning.db'

def init_db():
    with sqlite3.connect(DATABASE) as conn:
        cursor = conn.cursor()
        cursor.execute('''
            CREATE TABLE IF NOT EXISTS model_updates (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                weight REAL NOT NULL,
                timestamp TEXT NOT NULL
            )
        ''')
        conn.commit()

@app.route('/update', methods=['POST'])
def receive_model_update():
    data = request.form
    weight = float(data['weight'])
    timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    
    with sqlite3.connect(DATABASE) as conn:
        cursor = conn.cursor()
        cursor.execute('INSERT INTO model_updates (weight, timestamp) VALUES (?, ?)', (weight, timestamp))
        conn.commit()

    return jsonify({'status': 'success', 'message': 'Model update received'})

@app.route('/aggregate', methods=['GET'])
def aggregate_model_updates():
    with sqlite3.connect(DATABASE) as conn:
        cursor = conn.cursor()
        cursor.execute('SELECT weight FROM model_updates')
        weights = [row[0] for row in cursor.fetchall()]

    if weights:
        aggregated_weight = sum(weights) / len(weights)  # Simple average aggregation
        return jsonify({'status': 'success', 'aggregated_weight': aggregated_weight})
    else:
        return jsonify({'status': 'error', 'message': 'No model updates available'})

def prepare_data():
    with sqlite3.connect(DATABASE) as conn:
        cursor = conn.cursor()
        cursor.execute('SELECT weight FROM model_updates')
        weights = [row[0] for row in cursor.fetchall()]

    if not weights:
        return None

    # Convert to DataFrame
    df = pd.DataFrame(weights, columns=['weight'])

    # Normalize data
    df['weight'] = (df['weight'] - df['weight'].mean()) / df['weight'].std()

    # Prepare sequences for LSTM
    sequence_length = 10  # Example sequence length
    result = []
    for index in range(len(df) - sequence_length):
        result.append(df['weight'][index: index + sequence_length].values)

    result = np.array(result)

    # Split into input and output
    X = result[:, :-1]
    y = result[:, -1]

    # Reshape input to be [samples, time steps, features]
    X = np.reshape(X, (X.shape[0], X.shape[1], 1))

    return X, y

def build_model(input_shape):
    model = Sequential()
    model.add(LSTM(50, return_sequences=True, input_shape=input_shape))
    model.add(LSTM(50, return_sequences=False))
    model.add(Dense(1))
    model.compile(optimizer='adam', loss='mean_squared_error')
    return model

@app.route('/train', methods=['GET'])
def train_model():
    data = prepare_data()
    if data is None:
        return jsonify({'status': 'error', 'message': 'Not enough data to train the model'})

    X, y = data

    model = build_model((X.shape[1], 1))

    model.fit(X, y, epochs=5, batch_size=1, verbose=2)  # Example training parameters

    model.save('lstm_model.h5')

    return jsonify({'status': 'success', 'message': 'Model trained and saved'})

@app.route('/predict', methods=['POST'])
def predict():
    data = request.get_json()
    recent_data = data['recent_data']

    model = tf.keras.models.load_model('lstm_model.h5')

    # Normalize input data
    recent_data = np.array(recent_data)
    recent_data = (recent_data - recent_data.mean()) / recent_data.std()

    # Prepare input data for prediction
    recent_data = np.reshape(recent_data, (1, len(recent_data), 1))

    prediction = model.predict(recent_data)

    return jsonify({'status': 'success', 'prediction': prediction[0][0]})

if __name__ == '__main__':
    init_db()
    app.run(host='0.0.0.0', port=8080)
