""" flask_json_catalog.py """

#!/usr/bin/env python3

import jsonschema
import uuid

from flask import Flask, current_app, request
from flask.json import jsonify
from werkzeug.exceptions import BadRequest

# Defining the Catalog app
class CatalogFlask(Flask):
    def __init__(self, *args, **kwargs):
        super(CatalogFlask, self).__init__(*args, **kwargs)
        # catalog is a dict ref:(name, price, description)
        self.catalog = {"34":("toto", 352, "un peu cher")}

app = CatalogFlask(__name__)

#############
# Adding new references to the catalog
#############
NEW_OBJECT_JSON_SCHEMA = {
    "type": "object",
    "properties": {
        "name": {"type": "string"},
        "price": {"type": "number"},
        "description": {"type": "string"},
    },
    "required": ["name", "price", "description"],
}

NEW_OBJECTS_JSON_SCHEMA = {
    "type": "array",
    "items": NEW_OBJECT_JSON_SCHEMA,
}

def validate_new_object_schema(new_objects):
    try:
        jsonschema.validate(instance=new_objects, schema=NEW_OBJECTS_JSON_SCHEMA)
    except jsonschema.ValidationError as exc:
        raise BadRequest(str(exc))

@app.route("/new_references", methods=['POST'])
def add_new_objects():
    new_objects = request.get_json()
    validate_new_object_schema(new_objects)
    new_refs = []
    for new_object in new_objects:
        new_ref = uuid.uuid4().hex
        current_app.catalog[new_ref] = (new_object["name"], new_object["price"],
                                        new_object["description"])
        new_refs.append(new_ref)

    return jsonify([{"reference": ref} for ref in new_refs]), 201

#############
# Searching reference
#############

@app.route("/search", methods=['GET'])
def get_reference():
    reference = request.args.get('ref')
    if not reference:
        raise BadRequest("Get request needs a ref query attribute")

    if reference in current_app.catalog:
        (name, price, description) = current_app.catalog[reference]
        found_object_json = {
            "name": name,
            "price": price,
            "description": description,
            "reference": reference,
        }
        return jsonify(found_object_json), 200
    
    return ("", 204)
app.add_url_rule('/search', 'search_reference', get_reference)

if __name__ == '__main__':
    app.run()
