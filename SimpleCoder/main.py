from flask import Flask

app = Flask(__name__)

@app.route('/')
def home():
    return "<h1>Welcome to the Server</h1><p>The server is running successfully!</p>"

@app.route('/status')
def status():
    return {"status": "online", "port": 5000}

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
