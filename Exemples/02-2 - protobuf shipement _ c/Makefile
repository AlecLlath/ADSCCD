all: shipment_proto protobuf_c_shipment_server protobuf_c_shipment_client

shipment_proto: shipment.proto
	protoc-c --c_out=. shipment.proto

protobuf_c_shipment_server: protobuf_c_shipment_server.c socket_tcp_conf.h shipment_proto
	gcc -g -o protobuf_c_shipment_server protobuf_c_shipment_server.c shipment.pb-c.c -lprotobuf-c

protobuf_c_shipment_client: protobuf_c_shipment_client.c socket_tcp_conf.h shipment_proto
	gcc -g -o protobuf_c_shipment_client protobuf_c_shipment_client.c shipment.pb-c.c -lprotobuf-c

clean:
	rm shipment.pb-c.c shipment.pb-c.h protobuf_c_shipment_server protobuf_c_shipment_client
