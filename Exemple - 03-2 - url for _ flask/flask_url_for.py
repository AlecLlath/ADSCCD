""" flask_url_for.py """

#!/usr/bin/env python3

from flask import Flask, url_for
from markupsafe import escape

app = Flask(__name__)

@app.route('/la_route/de_ma_fonction/<le_param>')
def ma_fonction(le_param):
    return f"Merci pour escape(le_param)"

with app.test_request_context():
    print(url_for('ma_fonction', le_param="voici_mon_param"))
