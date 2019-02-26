@echo off
echo "::: Choose makefile option :::"
echo.
echo "0 : Makefile with windows command prompt."
echo "1 : Makefile with bash linux on windows."
echo "2 : Clean makefile."
echo.
set /p id="Enter makefile type: "
if %id% == 0 (
	copy makes\makefile_cmd makefile
) else (
	if %id% == 1 (
		copy makes\makefile_bash_linux makefile
	) else (
		if %id% == 2 (
			del /q makefile
		) else (
			copy makes\makefile_cmd makefile
		)
	)
)
