@echo off
echo "����Protobuf->--python_out=.\Proto"
..\..\protoc -I=.\ --python_out=.\ .\addressbook.proto
pause