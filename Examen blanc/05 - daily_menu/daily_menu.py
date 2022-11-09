#!/usr/bin/env python3
from flask import Flask
from werkzeug.exceptions import BadRequest

app = Flask(__name__)

menu={}
menu["monday"]="burger"
menu["tuesday"]="pizza"
menu["wednesday"]="salad"
menu["thursday"]="burger"
menu["friday"]="pizza"
menu["saturday"]="salad"
menu["sunday"]="pasta"

# adding a route using the decorator "route"
@app.route('/menu/<day>')
def per_day_menu(day):
    if not day in menu:
        raise BadRequest(f"Day \"{day}\" is not a day of the week")

    print(f"Menu of {day} was asked")
    return menu[day]

@app.route('/rating/<dish>/<int:rate>')
def rating(dish, rate):
    print(f"Rate {rate} registered for dish {dish}")
    return f"Rate {rate} registered for dish {dish}"

if __name__ == '__main__':
    # running the app
    app.run()
