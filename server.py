from flask import Flask, request, jsonify
import sqlite3
from datetime import datetime

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

@app.route('/')
def index():
    return "Welcome to the Federated Learning Server", 200

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

if __name__ == '__main__':
    init_db()
    app.run(host='0.0.0.0', port=8080)
