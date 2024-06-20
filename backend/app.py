from flask import Flask, request, jsonify
from flask_cors import CORS
import subprocess
import os

app = Flask(__name__)
CORS(app)  # This will enable CORS for all routes

@app.route('/run_algorithm', methods=['POST'])
def run_algorithm():
    try:
        data = request.json
        algorithm = data['algorithm']
        num_processes = data['num_processes']
        processes = data['processes']
        time_quantum = data.get('time_quantum', None)

        # Write input to input.txt
        with open('input.txt', 'w') as f:
            f.write(f"{num_processes}\n")
            for process in processes:
                f.write(f"{process['pid']} {process['arrival_time']} {process['burst_time']}\n")
            if time_quantum:
                f.write(f"{time_quantum}\n")

        # Compile and run the selected algorithm
        compile_command = f'g++ algorithms/{algorithm}.cpp -o scheduler'
        run_command = './scheduler'
        
        # Compile the C++ file
        compile_result = subprocess.run(compile_command, shell=True, capture_output=True, text=True)
        if compile_result.returncode != 0:
            return jsonify({"error": compile_result.stderr}), 500

        # Run the compiled scheduler
        run_result = subprocess.run(run_command, shell=True, capture_output=True, text=True)
        if run_result.returncode != 0:
            return jsonify({"error": run_result.stderr}), 500

        # Read the output from output.txt
        if not os.path.exists('output.txt'):
            return jsonify({"error": "Output file not found"}), 500

        with open('output.txt', 'r') as f:
            output = f.read()

        return jsonify({"output": output})
    except Exception as e:
        return jsonify({"error": str(e)}), 500

if __name__ == '__main__':
    app.run(debug=True)
