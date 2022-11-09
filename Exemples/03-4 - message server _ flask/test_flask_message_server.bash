#!/bin/bash

set -x

SLEEP=5

#Launch the server
./flask_message_server.py &
sleep $SLEEP

# Get home page
curl http://127.0.0.1:5000/
sleep $SLEEP

# Get message
curl http://127.0.0.1:5000/message
sleep $SLEEP

# Try to get set_message page
curl http://127.0.0.1:5000/set_message
sleep $SLEEP

# Post new message on set_message page
curl -d "Mon nouveau message" http://127.0.0.1:5000/set_message
sleep $SLEEP

# Get new message
curl http://127.0.0.1:5000/message
sleep $SLEEP

# Try to post a too long message
curl -d "Mon nouveau message vraiment trop long qui sera interdit" http://127.0.0.1:5000/set_message
sleep $SLEEP

kill %1
