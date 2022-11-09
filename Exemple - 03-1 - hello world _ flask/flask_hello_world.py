""" flask_hello_world.py """

#!/usr/bin/env python3

from flask import Flask
from markupsafe import escape

app = Flask(__name__)

@app.route('/simple_hello')
def simple_hello():
    return "Simple hello world !\n"

@app.route('/named_hello/')
@app.route('/named_hello/<name>')
def named_hello(name="Unknown"):
    return f"Hello \"{name}\" !\n"

@app.route('/repeat_hello/<int:nb>')
def repeat_hello(nb):
    msg=[]
    for _ in range(nb):
        msg.append("Hello !")

    return ' '.join(msg)
        
@app.route('/escape/<dangerous_param>')
def escape_dangerous_param(dangerous_param):
    return f"We take care of this param: {escape(dangerous_param)} !\n"

app.run()
