if [ -f "code/crazy-tennis" ]
then
	cd config; ../code/crazy-tennis;
else
	echo "Project not built, first run:"
	echo "$ sh ./autogen.sh"
	echo "$ make"
fi
