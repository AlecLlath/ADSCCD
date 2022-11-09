all: survey_proto protobuf_c_survey_server protobuf_c_survey_client

survey_proto: survey.proto
	protoc-c --c_out=. survey.proto

protobuf_c_survey_server: protobuf_c_survey_server.c socket_tcp_conf.h survey_proto
	gcc -g -o protobuf_c_survey_server protobuf_c_survey_server.c survey.pb-c.c -lprotobuf-c

protobuf_c_survey_client: protobuf_c_survey_client.c socket_tcp_conf.h survey_proto
	gcc -g -o protobuf_c_survey_client protobuf_c_survey_client.c survey.pb-c.c -lprotobuf-c

clean:
	rm survey.pb-c.c survey.pb-c.h protobuf_c_survey_server protobuf_c_survey_client
