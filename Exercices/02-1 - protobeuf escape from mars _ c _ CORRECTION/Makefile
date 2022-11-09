all: earth_mars_proto_c earth_mars_proto_python protobuf_c_escape_mars_ground_control_station \
     protobuf_c_escape_mars_captain

earth_mars_proto_c: earth_mars.proto
	protoc-c --c_out=. earth_mars.proto

earth_mars_proto_python: earth_mars.proto
	protoc --python_out=. earth_mars.proto

protobuf_c_escape_mars_ground_control_station: \
        protobuf_c_escape_mars_ground_control_station.c socket_tcp.c \
        socket_tcp.h earth_mars_proto_c
	gcc -g -o protobuf_c_escape_mars_ground_control_station \
        protobuf_c_escape_mars_ground_control_station.c socket_tcp.c \
        earth_mars.pb-c.c -lprotobuf-c

protobuf_c_escape_mars_captain: protobuf_c_escape_mars_captain.c \
                                socket_tcp.c socket_tcp.h earth_mars_proto_c
	gcc -g -o protobuf_c_escape_mars_captain \
        protobuf_c_escape_mars_captain.c socket_tcp.c earth_mars.pb-c.c \
		-lprotobuf-c

clean:
	rm earth_mars.pb-c.c earth_mars.pb-c.h \
        protobuf_c_escape_mars_ground_control_station \
        protobuf_c_escape_mars_captain
