#!/usr/bin/env python3
from flask import Flask, request, current_app, make_response
from jsonschema import validate, ValidationError
from random import seed, sample
from werkzeug.exceptions import BadRequest

PUZZLE_HEIGHT = 10
PUZZLE_WIDTH = 10
PUZZLE_NB_PIECE = PUZZLE_HEIGHT * PUZZLE_WIDTH

class PuzzleTemplateFlask(Flask):
    def __init__(self, *args, **kwargs):
        super(PuzzleTemplateFlask, self).__init__(*args, **kwargs)
        seed(71)
        self.template = sample(range(1, PUZZLE_NB_PIECE + 1), PUZZLE_NB_PIECE)
        self.already_post = {}

app = PuzzleTemplateFlask(__name__)

PUZZLE_POST_JSON_SCHEMA = {
        "type": "object",
        "properties": {
            "piece": {"type": "integer", "minimum": 1, "maximum": PUZZLE_NB_PIECE},
            "x_abscissa": {"type": "integer", "minimum": 1, "maximum": PUZZLE_HEIGHT},
            "y_ordinate": {"type": "integer", "minimum": 1, "maximum": PUZZLE_WIDTH},
            },
        "required": ["piece", "x_abscissa", "y_ordinate"],
        }

def validate_puzzle_post_json_schema(piece_post):
    try:
        validate(instance=piece_post, schema=PUZZLE_POST_JSON_SCHEMA)
    except ValidationError as exc:
        raise BadRequest(str(exc))

# adding a route using the decorator "route"
@app.route('/puzzle', methods=['POST'])
def puzzle_piece_post():
    piece_post = request.get_json()
    print(piece_post)
    validate_puzzle_post_json_schema(piece_post)
    if piece_post["piece"] in current_app.already_post:
        return BadRequest(f'Piece {piece_post["piece"]} was already posted.\n')

    if current_app.template[(piece_post["y_ordinate"] - 1) * PUZZLE_WIDTH + piece_post["x_abscissa"] - 1] != piece_post["piece"]:
        return make_response(f'Bad position for piece {piece_post["piece"]}\n', 202)

    current_app.already_post[piece_post["piece"]] = "OK"
    if len(current_app.already_post) != PUZZLE_NB_PIECE:
        return make_response(f'Well done. Piece {piece_post["piece"]} is now at the rigth place.\n {len(current_app.already_post)} pieces are now well placed.\n', 201)
    else:
        return make_response(f'Congratulations! You successfully finished this beautiful horse.\n', 200)

if __name__ == '__main__':
    # running the app
    app.run()
