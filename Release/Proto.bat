@echo off
echo "生成Protobuf->--python_out=.\Proto"
..\..\protoc -I=.\ --python_out=.\ .\test.proto
pause