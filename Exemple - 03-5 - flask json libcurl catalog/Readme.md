# flask_json_catalog description
Un server flask utilisant du json permettant de construire un catalog et
d'effectuer des recherches sur ce catalog.


# Adding new references to the catalog

```http
POST /new_references Content-type="application/json"

[
    {
        "name": "<object name>",
        "price": <price>,
        "description": "<string describing the object>"
    },
    ...
]
```

Possible answers:

- 201 Created Content-type="application/json"
- 400 Bad Request

The 201 body answer is as follows:

```json
[
    {
 
        "reference": "<unique_object_identifier>"
    },
    ...
]
```

# Searching reference

```http
GET /search?ref=<reference>
```

Possible answers:

- 200 OK Content-type="application/json"
- 204 No Content
- 400 Bad Request

The "200 OK" body answer is as follows:

```json
{
    "name": "<object name>",
    "price": <price>,
    "description": "<string describing the object>",
    "reference": "<unique_object_identifier>"
}
```
