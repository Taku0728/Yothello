# coding: utf-8
from flask import Flask, render_template

app = Flask(__name__)

@app.route("/")
def hello():
    return "Hello, Flask!"

@app.route("/othello") #アプリケーション/indexにアクセスが合った場合
def index():
   return render_template('othello.html')

if __name__ == "__main__":
    app.run()
