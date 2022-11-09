#!/bin/bash

NB_CLIENT=`grep NB_CLIENT socket_tcp_conf.h | cut -f 3 -d ' '`

time ./protobuf_c_shipment_server &
sleep 1

i=1
while [ $i -le $NB_CLIENT ]; do
    echo client $i
    ./protobuf_c_shipment_client
    i=$(($i+1))
done
