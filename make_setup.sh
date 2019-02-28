#!/bin/bash
echo "::: Choose makefile option :::"
echo ""
echo "0 : Makefile with windows command prompt."
echo "1 : Makefile with bash linux on windows."
echo "2 : Clean makefile."
echo ""
read -p 'Enter makefile type: ' id
if [ "$id" == "0" ]
then
	cp makes/makefile_cmd makefile
elif [ "$id" == "1" ]
then
	cp makes/makefile_bash_linux makefile
elif [ "$id" == "2" ]
then
	rm -rf makefile
else
	cp makes/makefile_cmd makefile
fi
