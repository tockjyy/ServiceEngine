@echo off
echo "Éú³ÉProtobuf->--python_out=.\Proto"
..\..\protoc -I=.\ --python_out=.\ .\addressbook.proto
pause