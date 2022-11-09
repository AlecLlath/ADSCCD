#!/bin/bash
set -x

SERVER_URL="http://127.0.0.1:5000"

echo "Trying to get an unknown reference:"
curl -i ${SERVER_URL}/search?ref=unknown_ref

echo "Add a new object and get the new ref:"
NEW_REF=`curl -H "Content-Type: application/json" -d "[{\"name\": \"tutu\", \"price\":134, \"description\": \"un premier objet\"}]" ${SERVER_URL}/new_references | grep reference | cut -d'"' -f4`

echo "Get back new object:"
curl ${SERVER_URL}/search?ref=${NEW_REF}
