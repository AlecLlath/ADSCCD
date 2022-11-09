#!/usr/bin/bash

PUZZLE_URL="http://127.0.0.1:5000/puzzle"

echo "test a bad url"
curl http://127.0.0.1:5000/bad_url

echo "test a bad methods"
curl ${PUZZLE_URL}

echo "bad json integer interval"
curl -X POST ${PUZZLE_URL} -H 'Conten-Type: application/json' -d '{"piece": 71, "x_abscissa": 11, "y_ordinate": 7}'

echo "test post piece 71 at each place"
for x in `seq 1 10`
do
    for y in `seq 1 10`
    do
        json="{\"piece\": 71, \"x_abscissa\": ${x}, \"y_ordinate\": ${y}}"
        curl -X POST ${PUZZLE_URL} -H 'Content-Type: application/json' -d "$json"
    done
done


echo "put all pieces from 1 to 99"
for p in `seq 1 99`
do
    for x in `seq 1 10`
    do
        for y in `seq 1 10`
        do
            json="{\"piece\": ${p}, \"x_abscissa\": ${x}, \"y_ordinate\": ${y}}"
            curl -X POST ${PUZZLE_URL} -H 'Content-Type: application/json' -d "$json"
        done
    done
done

echo "put last piece 100"
for x in `seq 1 10`
do
    for y in `seq 1 10`
    do
        json="{\"piece\": 100, \"x_abscissa\": ${x}, \"y_ordinate\": ${y}}"
        curl -X POST ${PUZZLE_URL} -H 'Content-Type: application/json' -d "$json"
    done
done

