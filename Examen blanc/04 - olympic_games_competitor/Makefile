all: registration_proto_c registration_proto_python registration_desk

registration_proto_c: olympic_games_registration.proto
	protoc-c --c_out=. olympic_games_registration.proto

registration_proto_python: olympic_games_registration.proto
	protoc --python_out=. olympic_games_registration.proto

registration_desk: registration_desk.c socket_conf.h registration_proto_c
	gcc -o registration_desk registration_desk.c olympic_games_registration.pb-c.c -lprotobuf-c

clean:
	rm olympic_games_registration_pb2.py olympic_games_registration.pb-c.h olympic_games_registration.pb-c.c registration_desk

