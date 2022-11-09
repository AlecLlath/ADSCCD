all: survey_pb2

survey_pb2: survey.proto
	protoc --python_out=. survey.proto

clean:
	rm survey_pb2.py survey_pb2.pyc
