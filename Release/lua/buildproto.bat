SETLOCAL 
cd proto
for %%i in (*.proto) do (  
"..\protoc.exe" --descriptor_set_out %%i.pb %%i
)
pause
ENDLOCAL 